/* $OBD1_GW: main.c, v1.1 2014/04/07 21:44:00 anders Exp $ */

/*
 * Copyright (c) 2014, Anders Franzen.
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

#include "obd1_drv.h"

static int set_run_mode(int argc, char **argv, struct Env *env);

static struct cmd cmds[] = {
	{"help", generic_help_fnc},
	{"mode", set_run_mode},
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
	int fd_obd;
	int fd_serial;
};

static struct obd_data obdd;

static void dump_data(const char *buf) {

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

static int handle_timeout(int dumfd, int dumevent, void *uref) {
	return 0;
}

static int handle_serial_data(int fd, int event, void *uref) {
	char buf[32];
	int rc;

	printf("got event callback for serial data\n");
	rc=io_read(fd,buf,sizeof(buf));
	printf("hej, read returned %d\n",rc);
	return 0;
}


static int handle_obd_data(int fd, int event, void *uref) {
	struct obd_data *obdd=(struct obd_data *)uref;
	char buf[32];
	int dlen;
	int rc;

	printf("got event callback for obd data\n");
	rc=io_read(fd,buf,sizeof(buf));
	printf("hej, read returned %d\n",rc);
	dlen=rc;
	dump_data(buf);
	rc=io_control(obdd->fd_serial, F_SETFL, (void*)O_NONBLOCK, 0);
	if (rc<0) {
		printf("failed to set serial port to nonblock\n");
	} else {
		rc=io_write(obdd->fd_serial, buf, dlen);
		if (rc<0) {
			printf("failed to write obd data to serial\n");
		}
	}
	return 0;
}

static void obd1_gw_io(void *dum);

static int obd160_gw(struct Env *env) {
	thread_create(obd1_gw_io,0,0,1,"obd1_gw");
	return 0;
}

static int endit=0;

static int obd160_gw_off(struct Env *env) {
	int fd_obd=obdd.fd_obd;
	int fd_serial=obdd.fd_serial;

	register_event(fd_obd, EV_READ, 0, 0);
	register_event(fd_serial, EV_READ, 0, 0);

	io_close(fd_obd);
	io_close(fd_serial);
	endit=1;
	return 0;
}

static int current_mode=0;
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
		current_mode=160;
		if (obd160_gw(env)<0) {
			current_mode=0;
			fprintf(env->io_fd,"failed to start obd 160 stream\n");
			return -1;
		}
	} else if (strcmp(argv[1],"8192")==0) {
		if (current_mode!=0) {
			fprintf(env->io_fd,"stop current obd1 run first\n");
			return -1;
		}
	} else if (strcmp(argv[1],"off")==0) {
		if (current_mode==0) {
			fprintf(env->io_fd,"obd1 is not running\n");
			return -1;
		}
		if (current_mode==160) {
			obd160_gw_off(env);
		}
		current_mode=0;

	} else {
		fprintf(env->io_fd,"Bad argument: %s\n", argv[1]);
		return -1;
	}
	return 0;
}

static void obd1_gw_io(void *dum) {
	int fd_serial=io_open("usb_serial0");
	int fd_obd=io_open("obd160_0");
	int rc;

	if (fd_serial<0) {
		printf("failed to open device\n");
		return;
	}

	if (fd_obd<0) {
		printf("failed to open device\n");
		return;
	}

	obdd.fd_obd=fd_obd;
	obdd.fd_serial=fd_serial;

	printf("fd for obd driver %d, and for obd serial %d\n", fd_obd, fd_serial);


	rc=io_control(fd_serial, F_SETFL, (void*)O_NONBLOCK, 0);
	if (rc<0) {
		printf("failed to set serial port to nonblock\n");
	}

	register_event(fd_serial, EV_READ, handle_serial_data, &obdd);
	register_event(fd_obd, EV_READ, handle_obd_data, &obdd);
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


//int main(void) {
int init_pkg(void) {
//	thread_create(obd1_gw_io,0,0,1,"obd1_gw");
	install_cmd_node(&cmn, root_cmd_node);
	/* create some jobs */
	printf("back from thread create\n");
////	while (1);
	return 0;
}
