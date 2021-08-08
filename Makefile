
#KREL=../../krel
#KREL=/home/anders/mips/qemu-jz/mycore/project/bl2/boards/MB997C
KREL=/home/anders/mips/qemu-jz/mycore/project/bl2/boards/MB1075B

OBJ:=$(KREL)/obj
LOBJ:=obj
#DRIVERS+=usr_drivers.o
include $(KREL)/Makefile



usr.bin.o: $(OBJ)/usr/sys_cmd/sys_cmd.o $(LOBJ)/usr/obd1_gw.o
	$(LD) -o $(LOBJ)/usr/usr.bin.o $(LDFLAGS_USR) $^
	$(OBJCOPY) --prefix-symbols=__usr_ $(LOBJ)/usr/usr.bin.o

usr_drivers: $(LOBJ) $(LOBJ)/usr $(LOBJ)/usr/obd1_gw_drivers.o
	$(LD) -r -o $(LOBJ)/usr/usr.drivers.o obd1_drv.o
	mkdir -p $(LOBJ)/drv_obj
	cp $(LOBJ)/usr/usr.drivers.o  $(LOBJ)/drv_obj/$@.o

$(LOBJ):
	mkdir $(LOBJ)

$(LOBJ)/usr:
	mkdir -p $(LOBJ)/usr


#all: obd1_gw.o obd1_gw_drivers.o

$(LOBJ)/usr/obd1_gw.o: main.o
	$(CC) -r -nostdlib -o $@ $^

asynchio.o: asynchio.c asynchio.h
	$(CC) $(CFLAGS_USR) -c -o $@ $<

$(LOBJ)/usr/obd1_gw_drivers.o: obd1_drv.o
	$(CC) -r -nostdlib -o $@ $^

%_drv.o: %_drv.c
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c
	$(CC) $(CFLAGS_USR) -c -o $@ $<


clean:
	rm -rf *.o obj myCore
