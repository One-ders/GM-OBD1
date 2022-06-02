/* $OBD1_GW: main.c, v1.1 2014/04/07 21:44:00 anders Exp $ */

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
 * @(#)main.c
 */

#include "sys.h"
#include "sys_env.h"
#include "io.h"
#include "asynchio.h"

#include <string.h>
#include <ctype.h>

#include "panellib/panel.h"

#include "menu.h"
#include "obd1_drv.h"

typedef int (*Initf)(void);
typedef int (*Updatef)(int fd, void *);

struct pdata {
        struct Panel *panel;
        Initf   initf;
        Updatef updatef;
        struct pdata *next;
};

struct pdata *currentP;
struct pdata pdata[4];

int menumode=1;
extern unsigned int last_line, last_col;

static int send_request(int argc, char **argv, struct Env *env);

static struct cmd cmds[] = {
	{"help", generic_help_fnc},
	{"sendreq", send_request},
	{0,0}
};

static struct cmd_node cmn = {
	"obd1",
	cmds,
};

#define RSP 0x01
#define RP  0x04
#define HBV 0x40
#define AMP 0x80

#define MV2_DIAG_FACTORY_TEST	0x08
#define MV2_DIAG_DIAG_TEST	0x10
#define MV2_DIAG_ALDL_TEST	0x20

#define MODE_160B	0
#define MODE_8192B	1

struct obd_data {
	volatile int fd_obd;
	volatile int fd_serial;
	int		mode;
};


static struct obd_data obdd;

static unsigned char mode10_packet[]={ 0x80, 0x56, 0x01, 0x29 };

static int send_req=0;
static int handle_timeout(int dumfd, int dumevent, void *uref) {
	if (obdd.mode==MODE_8192B) {
		if (send_req) {
			int rc;
			send_req=0;
			rc=io_write(obdd.fd_obd, mode10_packet, sizeof(mode10_packet));
			if (rc!=sizeof(mode10_packet)) {
				printf("failed to send mode10_packet\n");
			}
		}
	}
	return 0;
}

static int send_mode1_req() {
	if (obdd.mode==MODE_8192B) {
		send_req=0;
		io_write(obdd.fd_obd, mode10_packet, sizeof(mode10_packet));
	}
	return 0;
}

static int send_request(int argc, char **argv, struct Env *env) {
	send_req=1;
	return 0;
}

static void dump_bytes(int fd, void *buf, int size) {
        int i;
        char asc[32];
        printf("=========================================================\n");
        for (i=0;i<size;i++) {
                if (!(i%16)) {
                        printf("%08x: ", i);
                }
                printf("%02x", ((char *)buf)[i] & 0xff);
                asc[i%16] = isprint(((char *)buf)[i])?((char *)buf)[i]:'.';
                if (((i+1)%16)&&((i+1)<size)) {
                        printf(",");
                } else {
                        if ((i+1)>=size) {
                                int j;
                                for(j=(i%16)+1;j<16;j++) {
                                        printf("   ");
                                        asc[j]=' ';
                                }
                        }
                        printf(" ");
                        asc[16]=0;
                        printf(" :%s:\n",asc);
                }

        }
        printf("=========================================================\n");
}

static int handle_obd8192_data(int fd, int event, void *uref);

static int handle_serial_data(int fd, int event, void *uref) {
	char buf[128];
	int rc;

//	printf("got event callback for serial data\n");
	rc=io_read(fd,buf,sizeof(buf));
	buf[rc]=0;
//	printf("hej, read returned %d\n",rc);

	if (obdd.mode==MODE_160B) {
		if (strcmp(buf,"s")==0) {
			int fd_obd;
			printf("got an s command\n");
			register_event(obdd.fd_obd, EV_READ, 0, 0);
			io_close(obdd.fd_obd);
			fd_obd=io_open("obd8192_0");
			if (fd_obd<0) {
				printf("failed to open obd8192\n");
				return 0;
			}
			obdd.fd_obd=fd_obd;
			obdd.mode=MODE_8192B;
			register_event(obdd.fd_obd, EV_READ, handle_obd8192_data, &obdd);
			send_mode1_req();
			return 0;
		} else if (strcmp(buf,"m")==0) {
			printf("toggle menu\n");
			if (menumode) {
				menumode=0;
			} else {
				menumode=1;
			}
		}

		if (menumode) {
			currentP=currentP->next;
			rc=io_control(fd, F_SETFL, 0, 0);
			currentP->initf();
			draw_static_win(fd,currentP->panel);
			rc=io_control(fd, F_SETFL, (void *)O_NONBLOCK, 0);
		}
	}

	return 0;
}

static int endit=0;
//==============================================================================

static int handle_obd8192_data(int fd, int event, void *uref) {
	char buf[128];
	char obuf[512];
	int dlen;
	int rc;

	rc=io_read(fd,buf,sizeof(buf));
	printf("%t: mode=obd8192: read returned %d bytes\n",rc);
	dlen=rc;
//	dump_data8192(buf);
	if (obdd.fd_serial) {
		int rc;
		char *p=obuf;
		int i;
		rc=sprintf(p,"%t:");
		p+=rc;
		for(i=0;i<(dlen-1);i++) {
			rc=sprintf(p,"0x%02x,", buf[i]);
			p+=rc;
		}
		rc=sprintf(p,"0x%02x\n",buf[dlen-1]);
		p+=rc;

		io_control(obdd.fd_serial, F_SETFL, (void*)0, 0);
		rc=io_write(obdd.fd_serial, obuf, p-obuf);
		io_control(obdd.fd_serial, F_SETFL, (void*)O_NONBLOCK, 0);
		if (rc<0) {
			printf("failed to write obd data to serial\n");
		}
		send_req=1;
	}
	return 0;
}


//==============================================================================

static void dump_data160(const char *buf) {

	char *dm;
	if (buf[0]&MV2_DIAG_FACTORY_TEST) {
		dm="Factory test";
	} else if (buf[0]&MV2_DIAG_DIAG_TEST) {
		dm="Diagnostic test";
	} else if (buf[0]&MV2_DIAG_ALDL_TEST) {
		dm="Aldl test";
	} else {
		dm="---";
	}
	printf("\n\n\nData Dump\n");
	printf("Mode Word 2: %x, %cRoad_speed_pulse, %cref_pulse, DM %s, %cHigh_bat_volt, %cAir_meter_pulse\n",
		buf[0], buf[0]&RSP?'+':'-', buf[0]&RP?'+':'-', dm, buf[0]&HBV?'+':'-', buf[0]&AMP?'+':'-');
	printf("PromId: %d, ISSPMP (iac step): %d, Coolant temp: %d (Conv %d c)\n",
			(buf[1]<<8)|buf[2], buf[3], buf[4], (buf[4]*75)-4000);

	printf("MPH: %d, EGR dc: %d, RPM: %d (conv %d), TPS: %d (conv %d %), pulse corr cl: %d, Oxy: %d\n",
		buf[5], buf[6], buf[7], buf[7]*25, buf[8], (buf[8]*100)>>8, buf[9], buf[10]);

	printf("MALFFLG1: %x, (%cSpeedSensor, %cMAT Low, %cTPS Low, %cTPS High, %cCoolant low temp, %cCoolant high temp, %cOxy sensor, %cNo ref pulse\n",
		buf[11], buf[11]&1?'+':'-', buf[11]&2?'+':'-', buf[11]&4?'+':'-', buf[11]&8?'+':'-',
		buf[11]&0x10?'+':'-', buf[11]&0x20?'+':'-', buf[11]&0x40?'+':'-', buf[11]&0x80?'+':'-');

	printf("MALFFLG2: %x, (%cSpark tim err, %cCylSel err, %cMAF low, %cMAF high, %cEgr, %cIntake air temp high\n",
		buf[12], buf[12]&1?'+':'-', buf[12]&2?'+':'-', buf[12]&8?'+':'-',
		buf[12]&0x10?'+':'-', buf[12]&0x20?'+':'-', buf[12]&0x80?'+':'-');

	printf("MALFFLG3: %x, (%cADU err, %cFuel pump err, %cOverVolt, %cCalpack missing, %cPromErr, %cOXY Sens rich, %cOxySens Lean, %cESC failure\n",
		buf[13], buf[13]&0x01?'+':'-', buf[13]&0x02?'+':'-', buf[13]&0x04?'+':'-',
		buf[13]&0x08?'+':'-',buf[13]&0x10?'+':'-', buf[13]&0x20?'+':'-',
		buf[13]&0x40?'+':'-',buf[13]&0x80?'+':'-');

	printf("MALFFLG4: %x\n", buf[14]);

	printf("Fuel/Air: %x\n", buf[15]);

	printf("MAT: %x (conv %d c)\n", buf[16], (buf[16]*75)-4000);

	printf("MCU stat: %x, (%cP/N, %cCruise, %cTCC, %cPS high press)\n",
		buf[17], buf[17]&0x01?'+':'-', buf[17]&0x02?'+':'-', buf[17]&0x04?'+':'-',
		buf[17]&0x08?'+':'-');

	printf("LV8: %x, BLM: %x, ALDLCNTR: %x\n",
		buf[18], buf[19], buf[20]);
	printf("DISPFLOW: %d,%d\n", buf[21], (buf[22]*100)>>8);
	printf("Injector pulse %d.%d ms\n", buf[23], buf[24]);
}

static int handle_obd160_data(int fd, int event, void *uref) {
	char buf[32];
	int dlen;
	int rc;
	char obuf[512];

	if (obdd.mode!=MODE_160B) {
		printf("got event for wrong interface\n");
		return 0;
	}
	rc=io_read(fd,buf,sizeof(buf));
	printf("mode=obd_160, read returned %d bytes\n",rc);
	dlen=rc;
//	dump_data160(buf);


	if (menumode) {
		rc=io_control(obdd.fd_serial, F_SETFL, 0, 0);
		currentP->updatef(obdd.fd_serial,buf);
		rc=io_control(obdd.fd_serial, F_SETFL, (void*)O_NONBLOCK, 0);
	} else {
		int rc;
		char *p=obuf;
		int i;

		rc=sprintf(p,"%t:");
		p+=rc;
		for(i=0;i<(dlen-1);i++) {
			rc=sprintf(p,"0x%02x,", buf[i]);
			p+=rc;
		}
		rc=sprintf(p,"0x%02x\n",buf[dlen-1]);
		p+=rc;

		io_control(obdd.fd_serial, F_SETFL, (void*)0, 0);
		rc=io_write(obdd.fd_serial, obuf, p-obuf);
		io_control(obdd.fd_serial, F_SETFL, (void*)O_NONBLOCK, 0);
		if (rc<0) {
			printf("failed to write obd data to serial\n");
		}
	}
	return 0;
}

static void obd_gw_io(void *dum) {

	if (obdd.mode==MODE_160B) {
		int rc;
		int fd_obd=io_open("obd160_0");

		if (fd_obd<0) {
			printf("failed to open device\n");
			return;
		}

		obdd.fd_obd=fd_obd;

		currentP=&pdata[0];

		pdata[0].panel=RawData;
		pdata[0].initf=init_RawData;
		pdata[0].updatef=update_RawData;
		pdata[0].next=&pdata[1];

		pdata[1].panel=FlagData;
		pdata[1].initf=init_FlagData;
		pdata[1].updatef=update_FlagData;
		pdata[1].next=&pdata[2];

		pdata[2].panel=SensorData;
		pdata[2].initf=init_SensorData;
		pdata[2].updatef=update_SensorData;
		pdata[2].next=&pdata[3];

		pdata[3].panel=ErrorData;
		pdata[3].initf=init_ErrorData;
		pdata[3].updatef=update_ErrorData;
		pdata[3].next=&pdata[0];


		currentP->initf();
		draw_static_win(obdd.fd_serial, currentP->panel);
		set_cursor_position(obdd.fd_serial, last_line, 1);

		rc=io_control(obdd.fd_serial, F_SETFL, (void*)O_NONBLOCK, 0);
		printf("usb serial set to non block\n");

		// register event handler for data from car
		register_event(fd_obd, EV_READ, handle_obd160_data, &obdd);
	} else if (obdd.mode==MODE_8192B) {
		int fd_obd=io_open("obd8192_0");

		if (fd_obd<0) {
			printf("failed to open device\n");
			return;
		}

		obdd.fd_obd=fd_obd;

		register_event(fd_obd, EV_READ, handle_obd8192_data, &obdd);
	}

	register_timer(500, handle_timeout, 0);

	printf("obd_gw_io: leaving\n");
	endit=0;
}

static void obd_gw_io(void *dum);

#if 0
static int obd_gw() {
	thread_create(obd_gw_io,0,0,1,"obd_gw");
	return 0;
}
#endif

//========================================================================================

//static int current_mode=0;

static void obd1(void *dum) {
	int fd_serial=io_open("usb_serial0");
	int rc;

	if (fd_serial<0) {
		printf("failed to open device\n");
		return;
	}

	obdd.fd_serial=fd_serial;

	printf("fd for obd serial %d\n", fd_serial);

#if 0
	rc=io_control(fd_serial, F_SETFL, (void*)O_NONBLOCK, 0);
	if (rc<0) {
		printf("failed to set serial port to nonblock\n");
	}
#endif
	register_event(fd_serial, EV_READ, handle_serial_data, &obdd);

	obd_gw_io(0);

//	rc=io_control(fd_serial, F_SETFL, (void*)0, 0);
	rc=io_control(fd_serial, F_SETFL, (void*)O_NONBLOCK, 0);

	while(1) {
		do_event();
#if 0
		if (endit) {
			break;
		}
#endif
	}
	printf("obd1 thread: leaving\n");
	endit=0;
}


//int main(void) {
int init_pkg(void) {
	obdd.mode=MODE_160B;
	thread_create(obd1,0,0,1,"obd1");
	install_cmd_node(&cmn, root_cmd_node);
	/* create some jobs */
	printf("back from thread create\n");
	return 0;
}
