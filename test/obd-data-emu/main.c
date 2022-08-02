
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#define _XOPEN_SOURCE
#define __USE_XOPEN_EXTENDED
#include <stdlib.h>


static const struct timeval t_1400ms = { 1, 400000};
static const struct timeval t_1500ms = { 1, 500000};
static const struct timeval t_1600ms = { 1, 600000};
static pid_t pid=0;

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
		fprintf(stderr, "parse_time, bad hour\n");
		exit(1);
	}

	*p=0;
	errno=0;
	hours=strtoul(b, NULL, 10);
	if (errno) {
		fprintf(stderr, "parse_time, bad hour2\n");
		exit(1);
	}


	b=p+1;
	p=strchr(b, ':');  // find minutes
	if (p>(b+3)) {
		fprintf(stderr, "parse_time, bad minutes\n");
		exit(1);
	}

	*p=0;
	errno=0;
	minutes=strtoul(b, NULL, 10);
	if (errno) {
		fprintf(stderr, "parse_time, bad minutes2\n");
		exit(1);
	}

	b=p+1;
	p=strchr(b, '.');  // find seconds
	if (p>(b+3)) {
		fprintf(stderr, "parse_time, bad seconds\n");
		exit(1);
	}
	*p=0;
	errno=0;
	seconds=strtoul(b, NULL, 10);
	if (errno) {
		fprintf(stderr, "parse_time, bad seconds2\n");
		exit(1);
	}

	b=p+1;
	p=strchr(b, ':');  // find milliseconds
	if (p>(b+4)) {
		fprintf(stderr, "parse_time, bad ms\n");
		exit(1);
	}
	*p=0;
	errno=0;
	ms=strtoul(b, NULL, 10);
	if (errno) {
		fprintf(stderr, "parse_time, bad ms2\n");
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

struct timeval *subs_time(struct timeval *a, struct timeval *b) {
	int sec=a->tv_sec-b->tv_sec;
	int usec=a->tv_usec-b->tv_usec;

	while (usec>1000000) {
		usec=usec-1000000;
		sec++;
	}

	while (usec<0) {
		usec=usec+1000000;
		sec--;
	}

	b->tv_sec=sec;
	b->tv_usec=usec;

	return b;
}

static int cmp_time(const struct timeval *a, const struct timeval *b) {
	if (a->tv_sec>b->tv_sec) return 1;
	if (a->tv_sec<b->tv_sec) return -1;
	if (a->tv_usec>b->tv_usec) return 1;
	if (a->tv_usec<b->tv_usec) return -1;
	return 0;
}

static void time_sleep(const struct timeval *t) {
	struct timeval tcopy=*t;
	int rc=select(0,NULL, NULL, NULL, &tcopy);
}

static int open_ptmx() {
	int rc;
	int fd=open("/dev/ptmx", O_RDWR);
	char *devpath;

	if (fd<0) {
		perror("open ptmx");
		exit(1);
	}

	rc=grantpt(fd);
	if (rc<0) {
		perror("grantpt");
		exit(1);
	}

	rc=unlockpt(fd);
	if (rc<0) {
		perror("unlockpt");
		exit(1);
	}

	devpath=ptsname(fd);
	printf("open the device: %s\n", devpath);

	pid=fork();

	if (pid==0) { // the hombre nuevo
		close(fd);
		rc=execl("./obd1-menu/obd1", "bub", devpath,NULL);
		if (rc<0) {
			perror("execl");
		}
		exit(1);
	}


	return fd;
}

int main(int argc, char **argv) {
	FILE	*f=NULL;
	char	buf[2048];
	char	*p;
	int	opt;
	struct timeval ptime;
	int	fd;
	int	pstat=0;
	int	rc;

	memset(&ptime,0,sizeof(ptime));

	while ((opt=getopt(argc,argv,"d:"))!=-1) {
		switch (opt) {
			case 'd':
				f=fopen(optarg, "r");
				if (!f) {
					perror("open indata");
					exit(1);
				}
				break;
			default:
				fprintf(stderr, "need input file name with -d flag");
				exit(1);
		}
	}

	fd=open_ptmx();

	while(!feof(f)) {
		struct timeval time;
		const struct timeval *tdelta;
		char *tmpstr;
//		unsigned char d[255];
//		int i=0;

		fgets(buf,sizeof(buf),f);
		if (strlen(buf)>0) {
			buf[strlen(buf)-1]=0;
		}
		if (strlen(buf)==0) {
			continue;
		}

		p=tmpstr=strdup(buf);
//		printf("got line:%s:\n",buf);
		p=parse_time(p, &time);
//		printf("got p=%p for buf=%p, data=%s\n",p,buf,p);
//		i=parse_data(p,d,sizeof(d));
//		printf("got %d bytes in line\n", i);
		tdelta=subs_time(&time,&ptime);
		if (cmp_time(tdelta, &t_1400ms)<0) {
//			printf("tdelta smaller than 1400 ms\n");
			tdelta=&t_1500ms;
		}
		if (cmp_time(tdelta, &t_1600ms)>0) {
//			printf("tdelta bigger than 1600 ms\n");
			tdelta=&t_1500ms;
		}
//		printf("tdiff -> sec=%d, usec=%d\n", tdelta->tv_sec, tdelta->tv_usec);
		time_sleep(tdelta);
		sprintf(tmpstr,"%s\n\r", buf);
		rc=write(fd,tmpstr,strlen(tmpstr));
		if (rc<0) {
			perror("write");
		}
		free(tmpstr);
		ptime=time;
	}

//	fprintf(stderr, "leaving!, must stop pid %d\n", pid);
	rc=kill(pid, SIGTERM);
	if (rc<0) {
		perror("stopping obd");
	}

	waitpid(pid, &pstat, 0);

	return 0;
}
