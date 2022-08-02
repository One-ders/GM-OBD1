

all: obd1

obd1: menu.o main.o asynchio.o ./panellib/panel.o
	$(CC) -o $@ main.o asynchio.o menu.o ./panellib/panel.o

main.o: main.c asynchio.h
	$(CC) $(CFLAGS_USR) -c -o $@ $<

asynchio.o: asynchio.c asynchio.h
	$(CC) $(CFLAGS_USR) -c -o $@ $<

menu.o: menu.c panellib
	$(CC) $(CFLAGS_USR) -I./panellib -c -o $@ $<

menu.c: menues.tpl
	pcomp -o $(basename $@) $<

menues.tpl: ../../../menues.tpl
	ln -s $^ $@

panellib:
	mkdir panellib
	lndir ../../../../panellib ./panellib
	rm -f ./panellib/*.o

panellib/panellib.a: panellib/panel.o

panellib/panel.o: panellib/panel.c
	$(CC) $(CFLAGS_USR) -I./panellib -c -o $@ $<


clean:
	rm -rf  obd1 *.tpl menu.c menu.h *.o obj panellib
