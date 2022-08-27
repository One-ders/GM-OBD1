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

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>

#include "asynchio.h"

#include "panellib/panel.h"

#include "menu.h"

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
	volatile int fd_serial_in;
	volatile int fd_serial_out;
	int		mode;
};


static struct obd_data obdd;

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


static int handle_serial_data(int fd, int event, void *uref) {
	char buf[128];
	int rc;

//	printf("got event callback for serial data\n");
	rc=read(fd,buf,sizeof(buf));
	if (rc<0) {
		perror("read");
		exit(1);
	}
	buf[rc]=0;
//	printf("hej, read returned %d\n",rc);

	if (strcmp(buf,"s")==0) {
		int fd_obd;
		printf("got an s command\n");
		return 0;
	} else if (strcmp(buf,"t")==0) {
		printf("toggle menu\n");
		if (menumode) {
			menumode=0;
		} else {
			menumode=1;
		}
	} else if (strcmp(buf,"m")==0) {
		printf("menu mode\n");
		menumode=1;
	} else if (strcmp(buf,"r")==0) {
		printf("raw mode\n");
		menumode=0;
	}

	if (menumode) {
		currentP=currentP->next;
		rc=ioctl(fd, F_SETFL, 0, 0);
		currentP->initf();
		draw_static_win(fd,currentP->panel);
		rc=ioctl(fd, F_SETFL, (void *)O_NONBLOCK, 0);
	}

	return 0;
}

//==============================================================================

static char *parse_time(char *buf, struct timeval *t) {
	char *b=buf;
	char *p;

	unsigned int tsecs=0;
	unsigned int hours;
	unsigned int minutes;
	unsigned int seconds;
	unsigned int ms;

	// parse h:mm:ss:xxx      xxx=milli sec
	p=strchr(b, ':');  // find hours
	if (p>(b+10)) {
//		fprintf(stderr, "parse_time, bad hour\n");
		exit(1);
	}

	*p=0;
	errno=0;
	hours=strtoul(b, NULL, 10);
	if (errno) {
//		fprintf(stderr, "parse_time, bad hour2\n");
		exit(1);
	}


	b=p+1;
	p=strchr(b, ':');  // find minutes
	if (p>(b+3)) {
//		fprintf(stderr, "parse_time, bad minutes\n");
		exit(1);
	}

	*p=0;
	errno=0;
	minutes=strtoul(b, NULL, 10);
	if (errno) {
//		fprintf(stderr, "parse_time, bad minutes2\n");
		exit(1);
	}

	b=p+1;
	p=strchr(b, '.');  // find seconds
	if (p>(b+3)) {
//		fprintf(stderr, "parse_time, bad seconds\n");
		exit(1);
	}
	*p=0;
	errno=0;
	seconds=strtoul(b, NULL, 10);
	if (errno) {
//		fprintf(stderr, "parse_time, bad seconds2\n");
		exit(1);
	}

	b=p+1;
	p=strchr(b, ':');  // find milliseconds
	if (p>(b+4)) {
//		fprintf(stderr, "parse_time, bad ms\n");
		exit(1);
	}
	*p=0;
	errno=0;
	ms=strtoul(b, NULL, 10);
	if (errno) {
//		fprintf(stderr, "parse_time, bad ms2\n");
		exit(1);
	}

#if 0
	printf("hours is %d\n", hours);
	printf("minutes is %d\n", minutes);
	printf("seconds is %d\n", seconds);
	printf("ms is %d\n", ms);
#endif

	if (t) {
		t->tv_sec=seconds+(minutes*60)+(hours*3600);
		t->tv_usec=ms*1000;
	}

#if 0
	printf("tv_sec=%d, tv_usec=%d\n", t->tv_sec, t->tv_usec);
#endif

	return p+1;
}

static int parse_data(char *csvbuf, unsigned char *bytebuf, int bbufsz) {
	int i;
	char *b=csvbuf;
	char *p;
	char *lastb=strlen(csvbuf)+csvbuf;
	unsigned int byte;

	for(i=0;i<bbufsz;i++) {
		p=strchr(b, ',');  // find byte separator
		if (!p) {
			break;
		}
		*p=0;
		errno=0;
		byte=strtoul(b,0,16);
		if (errno) {
			fprintf(stderr, "bad byte in stream\n");
			exit(1);
		}
		bytebuf[i]=byte;
		b=p+1;
	}

	if (b<lastb) {
		errno=0;
		byte=strtoul(b,0,16);
		if (errno) {
			fprintf(stderr, "bad byte in stream\n");
			exit(1);
		}
		bytebuf[i]=byte;
	}
	return i+1;
}

static char *filter_crlf(char *buf) {
	char *p=strchr(buf,'\n');
	if (p) *p=0;
	p=strchr(buf,'\r');
	if (p) *p=0;
	return buf;
}


static int handle_obd160_data(int fd, int event, void *uref) {
	char buf[512];
	char bbuf[32];
	char *p;
	int dlen;
	int rc;
	int i;
	char *tbuf;

	rc=read(fd,buf,sizeof(buf));
	if (rc<=0) {
//		fprintf(stderr, "obd1:read returned %d\n", rc);
		exit(1);
	}

	buf[rc]=0;
	tbuf=strdup(buf);
	p=filter_crlf(tbuf);
	if (strlen(tbuf)==0) {
//		fprintf(stderr, "obd1:read empty buf after filter\n");
		return 0;
	}
	p=parse_time(p, NULL);
	i=parse_data(p,bbuf,sizeof(bbuf));
//	printf("mode=obd_160, read returned %d bytes\n",rc);
	dlen=i;
//	dump_data160(buf);


	if (menumode) {
		rc=ioctl(obdd.fd_serial_out, F_SETFL, 0, 0);
		currentP->updatef(obdd.fd_serial_out,bbuf);
		rc=ioctl(obdd.fd_serial_out, F_SETFL, (void*)O_NONBLOCK, 0);
	} else {
		int rc;
		rc=write(obdd.fd_serial_out, buf, strlen(buf));
	}

	free(tbuf);
	return 0;
}

static void obd_gw_io(char *dev) {
	int rc;
	int fd_obd=open(dev, O_RDONLY);

	if (fd_obd<0) {
		printf("failed to open device\n");
		return;
	}

	obdd.fd_obd=fd_obd;

	currentP=&pdata[0];

	pdata[0].panel=SensorData;
	pdata[0].initf=init_SensorData;
	pdata[0].updatef=update_SensorData;
	pdata[0].next=&pdata[1];

	pdata[1].panel=RawData;
	pdata[1].initf=init_RawData;
	pdata[1].updatef=update_RawData;
	pdata[1].next=&pdata[2];

	pdata[2].panel=FlagData;
	pdata[2].initf=init_FlagData;
	pdata[2].updatef=update_FlagData;
	pdata[2].next=&pdata[3];

	pdata[3].panel=ErrorData;
	pdata[3].initf=init_ErrorData;
	pdata[3].updatef=update_ErrorData;
	pdata[3].next=&pdata[0];


	currentP->initf();
	draw_static_win(obdd.fd_serial_out, currentP->panel);
	set_cursor_position(obdd.fd_serial_out, last_line, 1);

	rc=ioctl(obdd.fd_serial_in, F_SETFL, (void*)O_NONBLOCK, 0);

	// register event handler for data from car
	register_event(fd_obd, EV_READ, handle_obd160_data, &obdd);
}


//========================================================================================


static void obd1(char *dev) {
	int fd_serial_in=0;
	int fd_serial_out=1;
	int rc;

	obdd.fd_serial_in=fd_serial_in;
	obdd.fd_serial_out=fd_serial_out;

	register_event(fd_serial_in, EV_READ, handle_serial_data, &obdd);

	obd_gw_io(dev);

	rc=ioctl(fd_serial_in, F_SETFL, (void*)O_NONBLOCK, 0);

	while(1) {
		if (do_event()<0) break;
	}
	printf("obd1 thread: leaving\n");
}

static void pterm_handler(int signum) {
	printf("\n");
	exit(1);
}

int main(int argc, char **argv) {
	struct sigaction sa;
	obdd.mode=MODE_160B;
	if (argc!=2) {
		fprintf(stderr, "need device path for obd 160 stream input\n");
		exit(1);
	}
	memset(&sa,0,sizeof(sa));
	sa.sa_handler=pterm_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags=0;

	sigaction(SIGTERM, &sa, NULL);
	obd1(argv[1]);
	return 0;
}
