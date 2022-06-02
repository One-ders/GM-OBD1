/* $OBD1 drv: , v1.1 2014/04/07 21:44:00 anders Exp $ */

/*
 * Copyright (c) 2021, Anders Franzen.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @(#)obd160_drv.c
 */
#include <sys.h>
#include <io.h>
#include <string.h>
#include <led_drv.h>
#include <hr_timer.h>
#include <gpio_drv.h>

#include "obd1_drv.h"

#define NEW_2T_ITF

#define MIN(a,b) (a<b)?a:b
#define MAX_USERS 4

static struct driver *leddrv;
static struct driver *pindrv;
static struct driver *timerdrv;

static int zero=0;
static int one=1;

#define BAUD_8192 8192

#ifdef LED_GREEN
unsigned int green=LED_GREEN;
#endif
#ifdef LED_RED
unsigned int red=LED_RED;
#endif
#ifdef LED_BLUE
unsigned int blue=LED_BLUE;
#endif

static int interframe_space;

#define OBD160_STATE_IDLE	0

#define RX_BIT_START		1
#define RX_SAMPLE		2

struct obd160_data {
	struct device_handle *led_dh;
	struct device_handle *pin_dh;
	struct device_handle *timer_dh;
	volatile int	prev_pin_stat;
	unsigned int	rx_bstate;
	unsigned int	rx_state;
	unsigned int	b1cnt;
	unsigned int	bcnt;
	unsigned int	byte;
	unsigned int    bnum;
	volatile int	i;
	volatile int	o;
	volatile unsigned char buf[4][32];
};

#define RX_IDLE		0
#define RX_START_HUNT	1
#define RX_SAMPLE	2
#define RX_STOPBIT	3

struct obd8192_data {
	struct device_handle *led_dh;
	struct device_handle *pin_dh;
	struct device_handle *txpin_dh;
	struct device_handle *timer_dh;
	// Rx data
	unsigned int	rx_bstate;
	unsigned int	rx_bcnt;
	unsigned int 	rx_len;
	unsigned char	rx_buf[4][128];
	unsigned int	rx_i;
	unsigned int	rx_o;
	unsigned int	rx_bits_recd;
	unsigned int	rx_byte;
	// Tx data
	unsigned int	tx_run;
	unsigned char	tx_buf[32];
	unsigned int	tx_len;
	unsigned int	tx_i;
	unsigned int	tx_sent_bit;
	unsigned int	tx_char;
	struct blocker_list wblocker_list;
};

struct user_data160 {
	struct device_handle dh;
	DRV_CBH callback;
	void	*userdata;
	int	events;
	int	in_use;
	struct	obd160_data *obd160_data;
};

struct user_data8192 {
	struct device_handle dh;
	DRV_CBH callback;
	void	*userdata;
	int	events;
	int	in_use;
	struct	obd8192_data *obd8192_data;
};



static struct obd8192_data obd8192_data_0 = {
};

static struct obd160_data obd160_data_0 = {
	prev_pin_stat: -1,
};

static int obd_comm_speed=160;

static struct user_data8192 user_data8192[MAX_USERS];
static struct user_data160 user_data160[MAX_USERS];

static struct user_data8192 *get_user_data8192(void) {
	int i;
	for(i=0;i<MAX_USERS;i++) {
		if (!user_data8192[i].in_use) {
			user_data8192[i].in_use=1;
			return &user_data8192[i];
		}
	}
	return 0;
}

static int wakeup_users8192(struct obd8192_data *obd,int ev) {
	int i;
	for(i=0;i<MAX_USERS;i++) {
		if ((user_data8192[i].in_use) &&
			(user_data8192[i].obd8192_data==obd) &&
			(user_data8192[i].events&ev) &&
			(user_data8192[i].callback)) {
			user_data8192[i].callback(&user_data8192[i].dh,ev&user_data8192[i].events,user_data8192[i].userdata);
		}
	}
	return 0;
}


static struct user_data160 *get_user_data160(void) {
	int i;
	for(i=0;i<MAX_USERS;i++) {
		if (!user_data160[i].in_use) {
			user_data160[i].in_use=1;
			return &user_data160[i];
		}
	}
	return 0;
}

static int wakeup_users160(struct obd160_data *obd,int ev) {
	int i;
	for(i=0;i<MAX_USERS;i++) {
		if ((user_data160[i].in_use) &&
			(user_data160[i].obd160_data==obd) &&
			(user_data160[i].events&ev) &&
			(user_data160[i].callback)) {
			user_data160[i].callback(&user_data160[i].dh,ev&user_data160[i].events,user_data160[i].userdata);
		}
	}
	return 0;
}

// on pin irq, look for transition to low, this is how a start bit looks
static int pin_irq(struct device_handle *dh, int ev, void *dum) {

	if (obd_comm_speed==160) {
		int pin_stat;
		int uSecSample=2500;
		struct obd160_data *obd=(struct obd160_data *)dum;
		unsigned int left;

		pindrv->ops->control(obd->pin_dh,GPIO_SENSE_PIN,&pin_stat,sizeof(pin_stat));
		if (pin_stat==obd->prev_pin_stat) return 0;
		obd->prev_pin_stat=pin_stat;

		if (pin_stat!=0) {
			// start bit
			left=timerdrv->ops->control(obd->timer_dh,HR_TIMER_CANCEL, 0, 0);
			if (obd->bnum==25) {
				obd->bnum=0;
				sys_printf("interframe spacing %d uS\n", 1000000-left);
				interframe_space=0;
			}
			obd->rx_bstate=RX_BIT_START;
			timerdrv->ops->control(obd->timer_dh,HR_TIMER_SET,&uSecSample,sizeof(uSecSample));
		}
	} else if (obd_comm_speed==8192) {
		int pin_stat;
//		int uSecSample=10;
		int uSecSample=40;
		struct obd8192_data *obd=&obd8192_data_0;

		if (!obd->tx_run) {
			if (obd->rx_bstate==RX_IDLE) {

				pindrv->ops->control(obd->pin_dh,GPIO_SENSE_PIN,&pin_stat,sizeof(pin_stat));

				if (pin_stat==0 ) {
					sys_printf("hunting for start bit\n");
					return 0;
				}

				timerdrv->ops->control(obd->timer_dh,HR_TIMER_CANCEL, 0, 0);

				obd->rx_bstate=RX_START_HUNT;
				timerdrv->ops->control(obd->timer_dh,HR_TIMER_SET,&uSecSample,sizeof(uSecSample));
			}
		}
	}

	return 0;
}

static int obd8192_timeout(struct device_handle *dh, int ev, void *dum) {
	int pin_stat;
	struct obd8192_data *obd=(struct obd8192_data *)dum;

	if (obd->tx_run) {
		if (obd->tx_run==2) {
			obd->tx_run=0;
			return 0;
		}
		unsigned int bit_time=1000000/BAUD_8192; // 122 us
		// put out a bit
		if (obd->tx_char&1) {
			pindrv->ops->control(obd->txpin_dh, GPIO_SET_PIN,&one,4);
#if defined(LED_GREEN)
			leddrv->ops->control(obd->led_dh,LED_CTRL_ACTIVATE,&green,sizeof(green));
#elif defined(LED_BLUE)
			leddrv->ops->control(obd->led_dh,LED_CTRL_ACTIVATE,&blue,sizeof(blue));
#endif
		} else {
			pindrv->ops->control(obd->txpin_dh, GPIO_SET_PIN,&zero,4);
#if defined(LED_GREEN)
			leddrv->ops->control(obd->led_dh,LED_CTRL_DEACTIVATE,&green,sizeof(green));
#elif defined(LED_BLUE)
			leddrv->ops->control(obd->led_dh,LED_CTRL_DEACTIVATE,&blue,sizeof(blue));
#endif
		}
		timerdrv->ops->control(obd->timer_dh,HR_TIMER_SET,&bit_time,sizeof(bit_time));
		obd->tx_sent_bit++;
		if (obd->tx_sent_bit>=10) { // one byte done
			obd->tx_sent_bit=0;
			obd->tx_i++;
			if (obd->tx_i>=obd->tx_len) {
				obd->tx_run=2;
				obd->rx_len=3;       // ???
			} else {
				obd->tx_char=(((~obd->tx_buf[obd->tx_i])&0xff)<<1)|1;
			}
		} else {
			obd->tx_char>>=1;
		}
	} else { // rx
		if (obd->rx_bstate==RX_START_HUNT) {
			int uSecSample=1000000/BAUD_8192;
			pindrv->ops->control(obd->pin_dh,GPIO_SENSE_PIN,&pin_stat,sizeof(pin_stat));

			if (pin_stat==0) {
				// sample of potential start bit, proved to be wrong
				obd->rx_bstate=RX_IDLE;
				sys_printf("valid Startbit not recived\n");
				return 0;
			}
			obd->rx_bstate=RX_SAMPLE;
			obd->rx_byte=0;
			obd->rx_bits_recd=0;
			timerdrv->ops->control(obd->timer_dh,HR_TIMER_SET,&uSecSample,sizeof(uSecSample));
		} else if (obd->rx_bstate==RX_SAMPLE) {
			int uSecSample=1000000/BAUD_8192;
			unsigned int bit;

			pindrv->ops->control(obd->pin_dh,GPIO_SENSE_PIN,&pin_stat,sizeof(pin_stat));
			bit=pin_stat?0:1;

			obd->rx_byte=(obd->rx_byte>>1)|(bit<<7);
			obd->rx_bits_recd++;
			if (obd->rx_bits_recd>=8) {
				obd->rx_bstate=RX_STOPBIT;
			}
			timerdrv->ops->control(obd->timer_dh,HR_TIMER_SET,&uSecSample,sizeof(uSecSample));

			return 0;
		} else if (obd->rx_bstate==RX_STOPBIT) {
			pindrv->ops->control(obd->pin_dh,GPIO_SENSE_PIN,&pin_stat,sizeof(pin_stat));
			if (pin_stat==1) {
				sys_printf("valid Stopbit not recived, discard data\n");
				obd->rx_bstate=RX_IDLE;
				return 0;
			}
			obd->rx_bstate=RX_IDLE;
			obd->rx_buf[obd->rx_i&0x3][obd->rx_bcnt]=obd->rx_byte;
			obd->rx_bcnt++;
			if (obd->rx_bcnt==2) {
				obd->rx_len=(obd->rx_byte-0x55)+3;
			}
//			sys_printf("got byte nr %d:  %x\n", obd->rx_bcnt, obd->rx_byte);
			if (obd->rx_bcnt>=obd->rx_len) {
//				sys_printf("got complete msg\n");
				obd->rx_bcnt=0;
				obd->rx_len=2;
				obd->rx_i++;
				if ((obd->rx_i-obd->rx_o)>3) {
					obd->rx_o=obd->rx_i-3;
				}
//				sys_printf("calling wakeup for 8192 usr\n");
				wakeup_users8192(obd,EV_READ|EV_WRITE);
			}
		} else {
			sys_printf("got timeout in 8192 rx, with bad state: %d\n", obd->rx_bstate);
			obd->rx_bstate=RX_IDLE;
			return 0;
		}
	}

	return 0;
}

static int obd160_timeout(struct device_handle *dh, int ev, void *dum) {
	int pin_stat;
	struct obd160_data *obd=(struct obd160_data *)dum;
	unsigned int uSectout=6200-2500;

	if (obd->rx_bstate==RX_BIT_START) {
		int bit;
		obd->rx_bstate=RX_SAMPLE;
		pindrv->ops->control(obd->pin_dh,GPIO_SENSE_PIN,&pin_stat,sizeof(pin_stat));
		timerdrv->ops->control(obd->timer_dh,HR_TIMER_SET,&uSectout,sizeof(uSectout));
		bit=pin_stat?1:0;

		if (bit) {
			obd->b1cnt++;
		} else {
			obd->b1cnt=0;
		}

		if (obd->b1cnt==9) {
			obd->byte=0;
			obd->bcnt=0;
			obd->bnum=0;
//			sys_printf("got sync\n");
#ifdef LED_RED
			leddrv->ops->control(obd->led_dh,LED_CTRL_DEACTIVATE,&red,sizeof(red));
#elif defined(LED_BLUE)
			leddrv->ops->control(obd->led_dh,LED_CTRL_DEACTIVATE,&blue,sizeof(blue));
#endif
		} else {
			obd->byte=(obd->byte<<1)|bit;
			obd->bcnt++;
			if (obd->bcnt==9) {
				obd->buf[obd->i&0x3][obd->bnum]=obd->byte;
				obd->bnum++;
//				sys_printf("got byte nr %d:  %x\n", obd->bnum, obd->byte);
				obd->byte=0;
				obd->bcnt=0;
				if (obd->bnum==25) {
					obd->i++;
					if ((obd->i-obd->o)>3) {
						obd->o=obd->i-3;
					}
					wakeup_users160(obd,EV_READ|EV_WRITE);
#ifdef LED_RED
					leddrv->ops->control(obd->led_dh,LED_CTRL_ACTIVATE,&red,sizeof(red));
#elif defined(LED_BLUE)
					leddrv->ops->control(obd->led_dh,LED_CTRL_ACTIVATE,&blue,sizeof(blue));
#endif
				}
			}
		}
	} else {
		if (obd->bnum==25) {
			unsigned int interframe_timer=1000000;
			timerdrv->ops->control(obd->timer_dh,HR_TIMER_SET,&interframe_timer,sizeof(interframe_timer));
			interframe_space=1;
			if (obd8192_data_0.wblocker_list.first) {
				sys_wakeup_from_list(&obd8192_data_0.wblocker_list);
			}
		}
	}

	return 0;
}

static int in_interframe_space() {
	return interframe_space;
}


/******** handlers for different state *************/

static void obd8192_startTx(struct obd8192_data *od) {
	unsigned int tstartBit=1000000/BAUD_8192; // 122 us

	od->tx_run=1;
	timerdrv->ops->control(od->timer_dh,HR_TIMER_CANCEL,0,0);

	if (od->tx_char&1) {
		pindrv->ops->control(od->txpin_dh, GPIO_SET_PIN,&one,4);
	} else {
		pindrv->ops->control(od->txpin_dh, GPIO_SET_PIN,&zero,4);
	}
	od->tx_char>>=1;
	od->tx_sent_bit++;
	timerdrv->ops->control(od->timer_dh,HR_TIMER_SET, &tstartBit, sizeof(tstartBit));
}

static int obd8192_wrstr(struct obd8192_data *od, const unsigned char *buf, int len) {
	if ((!od->tx_run) || (od->rx_bstate!=RX_IDLE)) {
		memcpy(od->tx_buf,buf,len);
		od->tx_len=len;
		od->tx_i=0;
		od->tx_sent_bit=0;
		od->tx_char=(((~od->tx_buf[0])&0xff)<<1)|1;    // add a 0 start bit and a 1 stopbit

		obd8192_startTx(od);
		return len;
	} else {
		sys_printf("ignoring write, busy\n");
	}
	return -1;
}

/*****  Driver API *****/

static struct device_handle *obd8192_drv_open(void *inst, DRV_CBH cb, void *udata) {
	struct user_data8192 *u=get_user_data8192();

	if (!u) return 0;
	u->obd8192_data=(struct obd8192_data *)inst;
	u->callback=cb;
	u->userdata=udata;
	u->events=0;
	return &u->dh;
}

static int obd8192_drv_close(struct device_handle *dh) {
	struct user_data8192 *u=(struct user_data8192 *)dh;
	if (!u) {
		return 0;
	}
	if (u) {
		u->in_use=0;
		u->obd8192_data=0;
	}
	return 0;
}

static int obd8192_drv_control(struct device_handle *dh, int cmd, void *arg, int size) {
	struct user_data8192 *u=(struct user_data8192 *)dh;
	struct obd8192_data *od;

	if (!u) {
		return -1;
	}
	od=u->obd8192_data;

	switch(cmd) {
		case RD_CHAR: {
			int len;
			unsigned char *buf=(unsigned char *)arg;
			if (!(od->rx_i-od->rx_o)) {
				u->events|=EV_READ;
				return -DRV_AGAIN;
			}
			u->events&=~EV_READ;
			len=MIN(size,67);
			memcpy(buf,((const void *)od->rx_buf[od->rx_o&0x3]),len);
			od->rx_o++;
			return len;
			break;
		}
		case WR_CHAR: {
			unsigned char *buf=(unsigned char *)arg;
			if (obd_comm_speed==160) {
				struct obd160_data *obd160=&obd160_data_0;
				unsigned long int cpu_flags;
again:
				cpu_flags=disable_interrupts();
				if (!interframe_space) {
					if (!sys_sleepon_update_list(&current->blocker, &od->wblocker_list)) {
						restore_cpu_flags(cpu_flags);
						return -1;
					}
					restore_cpu_flags(cpu_flags);
					goto again;
				}
				restore_cpu_flags(cpu_flags);
				timerdrv->ops->control(obd160->timer_dh,HR_TIMER_CANCEL,0,0);
			}

			obd_comm_speed=8192;
			obd8192_wrstr(od,buf,size);

			return size;
			break;
		}
		case IO_POLL: {
			unsigned int events=(unsigned int)arg;
			unsigned int revents=0;
			if (EV_READ&events) {
				if (od->rx_i-od->rx_o) {
					revents|=EV_READ;
				} else {
					u->events|=EV_READ;
				}
			}
			return revents;
			break;
		}
	}
	return -1;
}

static int obd8192_drv_init(void *inst) {
	return 0;
}

static int obd8192_drv_start(void *inst) {
	struct obd8192_data *obd=(struct obd8192_data *)inst;

	/* Open High Resolution timer for pulse meassurements */
	if (!timerdrv) timerdrv=driver_lookup(HR_TIMER);
	if (!timerdrv) goto out2;
	obd->timer_dh=timerdrv->ops->open(timerdrv->instance,obd8192_timeout,(void *)obd);
	if (!obd->timer_dh) goto out2;
	obd->wblocker_list.is_ready=in_interframe_space;

	sys_printf("OBD 8192 protocol driver: Started\n");

out2:
	return 0;
}


static struct driver_ops obd8192_drv_ops = {
	obd8192_drv_open,
	obd8192_drv_close,
	obd8192_drv_control,
	obd8192_drv_init,
	obd8192_drv_start,
};

static struct driver obd8192_drv = {
	"obd8192_0",
	&obd8192_data_0,
	&obd8192_drv_ops,
};

//---------------------------------- 160 baud interface --------------------------
static struct device_handle *obd160_drv_open(void *inst, DRV_CBH cb, void *udata) {
	struct user_data160 *u=get_user_data160();

	if (!u) return 0;
	u->obd160_data=(struct obd160_data *)inst;
	u->callback=cb;
	u->userdata=udata;
	u->events=0;
	return &u->dh;
}

static int obd160_drv_close(struct device_handle *dh) {
	struct user_data160 *u=(struct user_data160 *)dh;
	if (!u) {
		return 0;
	}
	if (u) {
		u->in_use=0;
		u->obd160_data=0;
	}
	return 0;
}

static int obd160_drv_control(struct device_handle *dh, int cmd, void *arg, int size) {
	struct user_data160 *u=(struct user_data160 *)dh;
	struct obd160_data *od;

	if (!u) {
		return -1;
	}
	od=u->obd160_data;

	switch(cmd) {
		case RD_CHAR: {
			int len;
			unsigned char *buf=(unsigned char *)arg;
			if (!(od->i-od->o)) {
				u->events|=EV_READ;
				return -DRV_AGAIN;
			}
			u->events&=~EV_READ;
			len=MIN(size,25);
			memcpy(buf,((const void *)od->buf[od->o&0x3]),len);
			od->o++;

			return len;
			break;
		}
		case IO_POLL: {
			unsigned int events=(unsigned int)arg;
			unsigned int revents=0;
			if (EV_READ&events) {
				if (od->i-od->o) {
					revents|=EV_READ;
				} else {
					u->events|=EV_READ;
				}
			}
			return revents;
			break;
		}
	}
	return -1;
}

static int obd160_drv_init(void *inst) {
	return 0;
}

static int obd160_drv_start(void *inst) {
	struct obd160_data *obd160=(struct obd160_data *)inst;
	struct obd8192_data *obd8192=&obd8192_data_0;
	int flags;
	int rc;
	int pin;
	int txpin;

	/* Open Led driver so we can flash the leds a bit */
	if (!leddrv) leddrv=driver_lookup(LED_DRV);
	if (!leddrv) return 0;
	obd160->led_dh=leddrv->ops->open(leddrv->instance,0,0);
	if (!obd160->led_dh) return 0;

	obd8192->led_dh=obd160->led_dh;

	/* Open gpio driver, this will be our cable to the OBD1 pin E */
	if (!pindrv) pindrv=driver_lookup(GPIO_DRV);
	if (!pindrv) {
		sys_printf("OBD1: missing GPIO_DRV\n");
		goto out1;
	}

	obd160->pin_dh=pindrv->ops->open(pindrv->instance,pin_irq,(void *)obd160);
	if (!obd160->pin_dh) {
		sys_printf("OBD1: could not open GPIO_DRV\n");
		goto out1;
	}
	obd8192->pin_dh=obd160->pin_dh;

	/* bind txpin */
	obd8192->txpin_dh=pindrv->ops->open(pindrv->instance,0,(void *)obd8192);
	if (!obd8192->txpin_dh) {
		sys_printf("OBD1: could not open second inst. GPIO_DRV\n");
		goto out2;
	}

	/* Open High Resolution timer for pulse meassurements */
	if (!timerdrv) timerdrv=driver_lookup(HR_TIMER);
	if (!timerdrv) {
		sys_printf("OBD1: missing HR_TIMER\n");
		goto out3;
	}
	obd160->timer_dh=timerdrv->ops->open(timerdrv->instance,obd160_timeout,(void *)obd160);
	if (!obd160->timer_dh) {
		sys_printf("OBD1: could not open HR_TIMER\n");
		goto out3;
	}

	if (obd160==&obd160_data_0) {
		pin=OBD1_PIN;
		txpin=OBD1_TXPIN;
	} else {
		sys_printf("OBD1 protocol driver: error no pin assigned for driver\n");
		goto out4;
	}

	/* Program rx pin to be open drain, pull down, irq */
	rc=pindrv->ops->control(obd160->pin_dh,GPIO_BIND_PIN,&pin,sizeof(pin));
	if (rc<0) {
		sys_printf("OBD1 protocol driver: failed to bind rx pin\n");
		goto out4;
	}

	flags=GPIO_DIR(0,GPIO_INPUT);
	flags=GPIO_DRIVE(flags,GPIO_PULLDOWN);
	flags=GPIO_SPEED(flags,GPIO_SPEED_HIGH);
	flags=GPIO_IRQ_ENABLE(flags);
	rc=pindrv->ops->control(obd160->pin_dh,GPIO_SET_FLAGS,&flags,sizeof(flags));
	if (rc<0) {
		sys_printf("OBD1 reader driver: pin_flags update failed\n");
		goto out4;
	}

	/* Program tx pin to be  drain, pull down, irq */
	rc=pindrv->ops->control(obd8192->txpin_dh,GPIO_BIND_PIN,&txpin,sizeof(txpin));
	if (rc<0) {
		sys_printf("OBD1 reader driver: txpin bind failed\n");
		goto out4;
	}

	flags=GPIO_DIR(0,GPIO_OUTPUT);
	flags=GPIO_DRIVE(flags,GPIO_PUSHPULL);
	flags=GPIO_SPEED(flags,GPIO_SPEED_HIGH);
	rc=pindrv->ops->control(obd8192->txpin_dh,GPIO_SET_FLAGS,&flags,sizeof(flags));
	if (rc<0) {
		sys_printf("OBD1 reader driver: txpin_flags update failed\n");
		goto out4;
	}
	pindrv->ops->control(obd8192->txpin_dh, GPIO_SET_PIN,&zero,4);

	sys_printf("OBD1 protocol driver: Started\n");

	return 0;

out4:
	sys_printf("OBD1: failed to bind pin to GPIO\n");
	timerdrv->ops->close(obd160->timer_dh);

out3:
	pindrv->ops->close(obd8192->txpin_dh);

out2:
	pindrv->ops->close(obd160->pin_dh);

out1:
	leddrv->ops->close(obd160->led_dh);
	return 0;
}


static struct driver_ops obd160_drv_ops = {
	obd160_drv_open,
	obd160_drv_close,
	obd160_drv_control,
	obd160_drv_init,
	obd160_drv_start,
};

static struct driver obd160_drv = {
	"obd160_0",
	&obd160_data_0,
	&obd160_drv_ops,
};

void init_obd1(void) {
	driver_publish(&obd160_drv);
	driver_publish(&obd8192_drv);
}

INIT_FUNC(init_obd1);
