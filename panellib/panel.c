#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <termios.h>
#include "panel.h"
#endif

#include <string.h>
#include <stdarg.h>
#include <sys.h>
#include <io.h>
#include "panel.h"

#define write io_write
#define read io_read


static int out_fd;
#if 0
static struct termios	orig_termios;
static int		orig_fd;
#endif

unsigned int last_col, last_line;

#if 0
static void restore_termios() {
	tcsetattr(orig_fd, TCSAFLUSH, &orig_termios);
}
#endif

void enable_raw_mode(int fd) {
#if 0
	struct termios raw;
	orig_fd=fd;
	tcgetattr(fd, &orig_termios);
	atexit(restore_termios);
	raw=orig_termios;
	raw.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(fd, TCSAFLUSH, &raw);
#endif
}

int set_cursor_position(int fd, unsigned int y, unsigned int x) {
	int rc;
	char buf[32];

	sprintf(buf,"%c[%d;%dH",0x1b,y,x);
	rc=write(fd,buf,strlen(buf));
#if 0
	if (rc!=strlen(buf)) {
		printf("back from set cp: wrote %d, want %d\n", rc, strlen(buf));
	}
#endif
	return 0;
}

static int get_cursor_position(int fd, unsigned int *y, unsigned int *x) {
	char buf[32];
	int row=0;
	int col=0;
	int rc;
	int cnt=0;
	int pcnt=0;

	sprintf(buf,"%c[6n",0x1b);
	rc=write(fd,buf,strlen(buf));
#if 0
	if (rc!=strlen(buf)) {
		printf("back from get cp: wrote %d, want %d\n", rc, strlen(buf));
	}
#endif
	cnt=0;
	read(fd,buf,2);
	if (buf[0]!=0x1b) return -1;
	if (buf[1]!='[') return -1;
	while(1) {
		int rc=read(fd,buf,1);
		if (rc!=1) {
			cnt++;
			if (cnt>=1000) {
				return -1;
			}
		} else {
			cnt=0;
		}
		if (buf[0]>='0' && buf[0]<='9') {
			row=(row*10)+buf[0]-'0';
		} else if (buf[0]==';') {
			break;
		} else return -1;
	}

	cnt=0;
	while(1) {
		int rc=read(fd,buf,1);
		if (rc!=1) {
			cnt++;
			if (cnt>=1000) {
				return -1;
			}
		} else {
			cnt=0;
		}
		if (buf[0]>='0' && buf[0]<='9') {
			col=(col*10)+buf[0]-'0';
		} else if (buf[0]=='R') {
			break;
		} else return -1;
	}

	*y=row;
	*x=col;
	return 0;
}

static int get_scrn_size(int fd, unsigned int *y, unsigned int *x) {
	int rc;
	set_cursor_position(fd,999,999);
	rc=get_cursor_position(fd, y, x);
	if (rc<0) {
		*y=40;
		*x=132;
	}
	return 0;
}

static int clr_scrn(int fd) {
	int rc;
	char buf[32];
	sprintf(buf,"%c[2J",0x1b);
	rc=write(fd,buf,strlen(buf));
#if 0
	if (rc!=strlen(buf)) {
		printf("clr_scrn: write, wrote %d, wanted %d\n", rc, strlen(buf));
	}
#endif
	return 0;
}

int outf(struct Field *field, char *form, ...) {
	char buf[8];
	char *dotpos;
	va_list ap;
	int rc;
	if ((field->line+2>last_line)||
		(field->col+1-3>last_col)) {
		return 0;
	}
//	set_cursor_position(out_fd, field->line+1, field->col+1-3);
	va_start(ap,form);
	rc=vsnprintf(buf,sizeof(buf),form,ap);
	va_end(ap);
	dotpos=strchr(buf,'.');
	if (dotpos) {
		set_cursor_position(out_fd, field->line+1, field->col+1-rc+1);
	} else {
		set_cursor_position(out_fd, field->line+1, field->col+1-rc+1);
	}
	write(out_fd,buf,strlen(buf));
	return 0;
}

int outb(struct Field *field, char *output, int val) {
	char *space=" ";
	if ((field->line+2>last_line)||
		(field->col+1-3>last_col)) {
		return 0;
	}
	set_cursor_position(out_fd, field->line+1, field->col+1-3);
	if (val) {
		write(out_fd,output,1);
	} else {
		write(out_fd,space,1);
	}
	return 0;
}

int draw_static_win(int fd, struct Panel *p) {
	struct Field *f;
	int i;

	out_fd=fd;
	enable_raw_mode(fd);
	get_scrn_size(fd,&last_line,&last_col);
	clr_scrn(fd);

	for(i=0,f=p->fields;f->label;i++,f=&p->fields[i]) {
		if (f->type==TYPE_STATIC_TEXT) {
			if ((f->line+1)<last_line) {
				int rc;
				set_cursor_position(fd,f->line+1,f->col+1);
				rc=write(fd,f->label,strlen(f->label));
				if (rc!=strlen(f->label)) {
					printf("error from print: rc=%d, wanted %d\n", rc, strlen(f->label));
				}
			}
		}
	}
	return 0;
}
