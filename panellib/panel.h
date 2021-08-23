
#define TYPE_UNKNOWN     0
#define TYPE_STATIC_TEXT 1

struct Field {
        char *label;
        int type;
        int line;
        int col;
        int len;
};

typedef int (*Opfunc)(char *form, int val);

struct Op {
	struct Field *fref;
	Opfunc func;
	char   	*form;
	int	val;
};

struct Panel {
        char *id;
        struct Field *fields;
	struct Op    *ops;
};

int outf(struct Field *, char *f, ...);
int outb(struct Field *, char *out, int printIfval);
int draw_static_win(int fd, struct Panel *p);
