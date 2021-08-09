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

#define MIN(a,b) (a<b)?a:b
#define MAX_USERS 4

static struct driver *leddrv;
static struct driver *pindrv;
static struct driver *timerdrv;

#ifdef LED_GREEN
unsigned int amber=LED_GREEN;
#endif
#ifdef LED_RED
unsigned int red=LED_RED;
#endif


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

struct obd8192_data {
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
	volatile unsigned char buf[4][64];
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
	prev_pin_stat: -1,
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
			sys_printf("callback for user %d\n", i);
			user_data160[i].callback(&user_data160[i].dh,ev&user_data160[i].events,user_data160[i].userdata);
		}
	}
	return 0;
}

// on pin irq, look for transition to low, this is how a start bit looks
static int pin_irq(struct device_handle *dh, int ev, void *dum) {

	if (obd_comm_speed==160) {
		int pin_stat;
		int uSecSample=2000;
		struct obd160_data *obd=(struct obd160_data *)dum;
		unsigned int left;

		pindrv->ops->control(obd->pin_dh,GPIO_SENSE_PIN,&pin_stat,sizeof(pin_stat));
		if (pin_stat==obd->prev_pin_stat) return 0;
		obd->prev_pin_stat=pin_stat;

		if (pin_stat==0) {
			// start bit
			left=timerdrv->ops->control(obd->timer_dh,HR_TIMER_CANCEL, 0, 0);
			if (obd->bnum==25) {
				obd->bnum=0;
				sys_printf("interframe spacing %d uS\n", 1000000-left);
			}
			obd->rx_bstate=RX_BIT_START;
			timerdrv->ops->control(obd->timer_dh,HR_TIMER_SET,&uSecSample,sizeof(uSecSample));
		}
	}


	return 0;
}

static int obd160_timeout(struct device_handle *dh, int ev, void *dum) {
	int pin_stat;
	struct obd160_data *obd=(struct obd160_data *)dum;
	unsigned int uSectout=6200-2000;

	if (obd->rx_bstate==RX_BIT_START) {
		int bit;
		obd->rx_bstate=RX_SAMPLE;
		pindrv->ops->control(obd->pin_dh,GPIO_SENSE_PIN,&pin_stat,sizeof(pin_stat));
		timerdrv->ops->control(obd->timer_dh,HR_TIMER_SET,&uSectout,sizeof(uSectout));
		bit=pin_stat?0:1;

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
			leddrv->ops->control(obd->led_dh,LED_CTRL_DEACTIVATE,&red,sizeof(red));
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
					if (obd->i>(obd->o+3)) {
						obd->o=obd->i-3;
					}
					wakeup_users160(obd,EV_READ|EV_WRITE);
					leddrv->ops->control(obd->led_dh,LED_CTRL_ACTIVATE,&red,sizeof(red));
				}
			}
		}
	} else {
		if (obd->bnum==25) {
			unsigned int interframe_timer=1000000;
			timerdrv->ops->control(obd->timer_dh,HR_TIMER_SET,&interframe_timer,sizeof(interframe_timer));
		}
	}

	return 0;
}

static int obd8192_timeout(struct device_handle *dh, int ev, void *dum) {
#if 0
	int pin_stat;
	struct obd8192_data *obd=(struct obd8192_data *)dum;
	unsigned int uSectout=6200-2000;

	if (obd->rx_bstate==RX_BIT_START) {
		int bit;
		obd->rx_bstate=RX_SAMPLE;
		pindrv->ops->control(obd->pin_dh,GPIO_SENSE_PIN,&pin_stat,sizeof(pin_stat));
		timerdrv->ops->control(obd->timer_dh,HR_TIMER_SET,&uSectout,sizeof(uSectout));
		bit=pin_stat?0:1;

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
			leddrv->ops->control(obd->led_dh,LED_CTRL_DEACTIVATE,&red,sizeof(red));
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
					wakeup_users160(obd,EV_READ|EV_WRITE);
					leddrv->ops->control(obd->led_dh,LED_CTRL_ACTIVATE,&red,sizeof(red));
				}
			}
		}
	} else {
		if (obd->bnum==25) {
			unsigned int interframe_timer=1000000;
			timerdrv->ops->control(obd->timer_dh,HR_TIMER_SET,&interframe_timer,sizeof(interframe_timer));
		}
	}
#endif

	return 0;
}


/******** handlers for different state *************/


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
			if (!(od->i-od->o)) {
				u->events|=EV_READ;
				return -DRV_AGAIN;
			}
			u->events&=~EV_READ;
			len=MIN(size,64);
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

static int obd8192_drv_init(void *inst) {
	return 0;
}

static int obd8192_drv_start(void *inst) {
	struct obd8192_data *obd=(struct obd8192_data *)inst;

	/* Open High Resolution timer for pulse meassurements */
	if (!timerdrv) timerdrv=driver_lookup(HR_TIMER);
	if (!timerdrv) goto out2;
//	obd->timer_dh=timerdrv->ops->open(timerdrv->instance,obd8192_timeout,(void *)obd);
//	if (!obd->timer_dh) goto out2;

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

	/* Open Led driver so we can flash the leds a bit */
	if (!leddrv) leddrv=driver_lookup(LED_DRV);
	if (!leddrv) return 0;
	obd160->led_dh=leddrv->ops->open(leddrv->instance,0,0);
	if (!obd160->led_dh) return 0;

	obd8192->led_dh=obd160->led_dh;

	/* Open gpio driver, this will be our cable to the OBD1 pin E */
	if (!pindrv) pindrv=driver_lookup(GPIO_DRV);
	if (!pindrv) goto out1;
	obd160->pin_dh=pindrv->ops->open(pindrv->instance,pin_irq,(void *)obd160);
	if (!obd160->pin_dh) goto out1;
	obd8192->pin_dh=obd160->pin_dh;

	/* Open High Resolution timer for pulse meassurements */
	if (!timerdrv) timerdrv=driver_lookup(HR_TIMER);
	if (!timerdrv) goto out2;
	obd160->timer_dh=timerdrv->ops->open(timerdrv->instance,obd160_timeout,(void *)obd160);
	if (!obd160->timer_dh) goto out2;

	if (obd160==&obd160_data_0) {
		pin=OBD1_PIN;
	} else {
		sys_printf("OBD1 protocol driver: error no pin assigned for driver\n");
		goto out3;
	}

	/* Program pin to be open drain, pull down, irq */
	rc=pindrv->ops->control(obd160->pin_dh,GPIO_BIND_PIN,&pin,sizeof(pin));
	if (rc<0) goto out3;

	flags=GPIO_DIR(0,GPIO_INPUT);
	flags=GPIO_DRIVE(flags,GPIO_PULLDOWN);
	flags=GPIO_SPEED(flags,GPIO_SPEED_SLOW);
	flags=GPIO_IRQ_ENABLE(flags);
	rc=pindrv->ops->control(obd160->pin_dh,GPIO_SET_FLAGS,&flags,sizeof(flags));
	if (rc<0) {
		sys_printf("OBD1 reader driver: pin_flags update failed\n");
		goto out3;
	}
	sys_printf("OBD1 protocol driver: Started\n");

	return 0;

out3:
	sys_printf("OBD1: failed to bind pin to GPIO\n");
	timerdrv->ops->close(obd160->timer_dh);

out2:
	sys_printf("OBD1: failed to open HR_TIMER\n");
	pindrv->ops->close(obd160->pin_dh);

out1:
	sys_printf("OBD1: failed to open GPIO_DRV\n");
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
