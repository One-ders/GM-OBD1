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

extern unsigned int last_line, last_col;

static int set_run_mode(int argc, char **argv, struct Env *env);
static int send_request(int argc, char **argv, struct Env *env);

static struct cmd cmds[] = {
	{"help", generic_help_fnc},
	{"mode", set_run_mode},
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

struct obd_data {
	volatile int fd_obd;
	volatile int fd_serial;
};

static struct obd_data obdd;
static int obd8192_running=0;

static int handle_timeout(int dumfd, int dumevent, void *uref) {
	return 0;
}

static int obd8192_gw(struct Env *env);

static void dump_bytes(void *buf, int size) {
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

static int handle_serial_data(int fd, int event, void *uref) {
	char buf[128];
	int rc;

	printf("got event callback for serial data\n");
	rc=io_read(fd,buf,sizeof(buf));
	printf("hej, read returned %d\n",rc);

	currentP=currentP->next;
	rc=io_control(obdd.fd_serial, F_SETFL, 0, 0);
	currentP->initf();
	draw_static_win(fd,currentP->panel);
	rc=io_control(obdd.fd_serial, F_SETFL, (void *)O_NONBLOCK, 0);
#if 0
	dump_bytes(buf,rc);
	if (!obd8192_running) {
		obd8192_gw(0);
		while(obdd.fd_obd!=0) sleep(1);
	}
	io_write(obdd.fd_obd, buf, rc);
#endif
	return 0;
}

static int endit=0;
static int send_req=0;
//==============================================================================

static void dump_data8192(const char *buf) {
}

static int handle_obd8192_data(int fd, int event, void *uref) {
	struct obd_data *obdd=(struct obd_data *)uref;
	char buf[128];
	int dlen;
	int rc;

	printf("got event callback for obd data\n");
	rc=io_read(fd,buf,sizeof(buf));
	printf("hej, read returned %d\n",rc);
	dlen=rc;
//	dump_data8192(buf);
	if (obdd->fd_serial) {
		rc=io_write(obdd->fd_serial, buf, dlen);
		if (rc<0) {
			printf("failed to write obd data to serial\n");
		}
	}
	return 0;
}

unsigned char mode10_packet[]={ 0x80, 0x56, 0x01, 0x29 };

static void obd8192_gw_io(void *dum) {
	int fd_obd=io_open("obd8192_0");
	int rc;

	if (fd_obd<0) {
		printf("failed to open device\n");
		return;
	}

	obdd.fd_obd=fd_obd;

	printf("fd for obd driver %d\n", fd_obd);

	register_event(fd_obd, EV_READ, handle_obd8192_data, &obdd);
	register_timer(500, handle_timeout, 0);

	while(1) {
		do_event();
		if (send_req) {
			send_req=0;
			rc=io_write(fd_obd, mode10_packet, sizeof(mode10_packet));
		}
		if (endit) {
			break;
		}
	}
	printf("leaving\n");
	obd8192_running=0;
	endit=0;
}

static void obd8192_gw_io(void *dum);

static int obd8192_gw(struct Env *env) {
	if (!obd8192_running) {
		obd8192_running=1;
		thread_create(obd8192_gw_io,0,0,1,"obd8192_gw");
	}
	return 0;
}


static int obd8192_gw_off(struct Env *env) {
	int fd_obd=obdd.fd_obd;
//	int fd_serial=obdd.fd_serial;

	register_event(fd_obd, EV_READ, 0, 0);
//	register_event(fd_serial, EV_READ, 0, 0);

	io_close(fd_obd);
//	io_close(fd_serial);
	endit=1;
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
	struct obd_data *obdd=(struct obd_data *)uref;
	char buf[32];
	int dlen;
	int rc;

	printf("got event callback for obd data\n");
	rc=io_read(fd,buf,sizeof(buf));
	printf("hej, read returned %d\n",rc);
	dlen=rc;
	dump_data160(buf);

#if 1
	rc=io_control(obdd->fd_serial, F_SETFL, 0, 0);
	currentP->updatef(obdd->fd_serial,buf);
	rc=io_control(obdd->fd_serial, F_SETFL, (void*)O_NONBLOCK, 0);
#endif
#if 0
	rc=io_control(obdd->fd_serial, F_SETFL, (void*)O_NONBLOCK, 0);
	if (rc<0) {
		printf("failed to set serial port to nonblock\n");
	} else {
		rc=io_write(obdd->fd_serial, buf, dlen);
		if (rc!=dlen) {
			printf("failed to write %d bytes obd data to serial\n",rc);
		}
	}
#endif
	return 0;
}

static void obd160_gw_io(void *dum) {
	int fd_obd=io_open("obd160_0");
	int rc;

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
	rc=io_control(obdd.fd_serial, F_SETFL, (void*)O_NONBLOCK, 0);
//	rc=io_control(obdd.fd_serial, F_SETFL, 0, 0);
	draw_static_win(obdd.fd_serial, currentP->panel);
	set_cursor_position(obdd.fd_serial, last_line, 1);
//	rc=io_control(obdd.fd_serial, F_SETFL, (void*)O_NONBLOCK, 0);

	printf("usb serial set to non block\n");

	register_event(fd_obd, EV_READ, handle_obd160_data, &obdd);
	register_timer(500, handle_timeout, 0);


	while(1) {
		do_event();
		if (endit) {
			break;
		}
	}
	printf("leaving\n");
	endit=0;
}

static void obd160_gw_io(void *dum);

static int obd160_gw(struct Env *env) {
	thread_create(obd160_gw_io,0,0,1,"obd160_gw");
	return 0;
}

static int obd160_gw_off(struct Env *env) {
	int fd_obd=obdd.fd_obd;

	register_event(fd_obd, EV_READ, 0, 0);

	io_close(fd_obd);
	endit=1;
	return 0;
}

//========================================================================================

static int current_mode=0;
static int send_request(int argc, char **argv, struct Env *env) {
	send_req=1;
	return 0;
}



static int set_run_mode(int argc, char **argv, struct Env *env) {
	if (argc!=2) {
		fprintf(env->io_fd, "need arg 160, 8192, or off\n");
		return -1;
	}

	if (strcmp(argv[1],"160")==0) {
		if (current_mode!=0) {
			fprintf(env->io_fd,"stop current obd1 run first\n");
			return -1;
		}
		if (obd160_gw(env)<0) {
			current_mode=0;
			fprintf(env->io_fd,"failed to start obd 160 stream\n");
			return -1;
		}
		current_mode=160;
	} else if (strcmp(argv[1],"8192")==0) {
		if (current_mode!=0) {
			fprintf(env->io_fd,"stop current obd1 run first\n");
			return -1;
		}
		if (obd8192_gw(env)<0) {
			current_mode=0;
			fprintf(env->io_fd,"failed to start obd 160 stream\n");
			return -1;
		}
		current_mode=8192;
	} else if (strcmp(argv[1],"off")==0) {
		if (current_mode==0) {
			fprintf(env->io_fd,"obd1 is not running\n");
			return -1;
		}
		if (current_mode==160) {
			obd160_gw_off(env);
		} else if (current_mode==8192) {
			obd8192_gw_off(env);
		}
		current_mode=0;

	} else {
		fprintf(env->io_fd,"Bad argument: %s\n", argv[1]);
		return -1;
	}
	return 0;
}

static void obd1_serial_mon(void *dum) {
	int fd_serial=io_open("usb_serial0");
	int rc;

	if (fd_serial<0) {
		printf("failed to open device\n");
		return;
	}

	obdd.fd_serial=fd_serial;

	printf("fd for obd serial %d\n", fd_serial);

	rc=io_control(fd_serial, F_SETFL, (void*)O_NONBLOCK, 0);
	if (rc<0) {
		printf("failed to set serial port to nonblock\n");
	}

	io_write(fd_serial, "hej hej\n", 8);
	register_event(fd_serial, EV_READ, handle_serial_data, &obdd);

	while(1) {
		do_event();
#if 0
		if (endit) {
			break;
		}
#endif
	}
	printf("leaving\n");
	endit=0;
}


//int main(void) {
int init_pkg(void) {
	char *bub[]={"mode", "160"};
	struct Env fenv;
	thread_create(obd1_serial_mon,0,0,1,"obd1_serial_mon");
	fenv.io_fd=0;
	set_run_mode(2, bub, &fenv);
	install_cmd_node(&cmn, root_cmd_node);
	/* create some jobs */
	printf("back from thread create\n");
////	while (1);
	return 0;
}
