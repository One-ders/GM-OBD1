
#KREL=../../krel
KREL=/home/anders/mips/qemu-jz/mycore/project/bl2/boards/MB997C
#KREL=/home/anders/mips/qemu-jz/mycore/project/bl2/boards/MB1075B

## application brings own driver, specify the make target in the
## macro below. The make file is expected to create a object file
## with the name of the target+".o" and put it under 'obj/drv_obj'
APPLICATION_DRIVERS=my_drivers

OBJ:=$(KREL)/obj
LOBJ:=obj

include $(KREL)/Makefile



usr.bin.o: $(OBJ)/usr/sys_cmd/sys_cmd.o $(LOBJ)/usr/obd1_gw.o
	$(LD) -o $(LOBJ)/usr/usr.bin.o $(LDFLAGS_USR) $^
	$(OBJCOPY) --prefix-symbols=__usr_ $(LOBJ)/usr/usr.bin.o

my_drivers: $(LOBJ)/usr $(LOBJ)/usr/obd1_gw_drivers.o
	$(LD) -r -o $(LOBJ)/usr/usr.drivers.o obd1_drv.o
	mkdir -p $(LOBJ)/drv_obj
	cp $(LOBJ)/usr/usr.drivers.o  $(LOBJ)/drv_obj/$@.o

$(LOBJ)/usr:
	mkdir -p $(LOBJ)/usr

$(LOBJ)/usr/obd1_gw.o: $(LOBJ)/usr main.o
	$(CC) -r -nostdlib -o $@ main.o

$(LOBJ)/usr/obd1_gw_drivers.o: obd1_drv.o
	$(CC) -r -nostdlib -o $@ $^

%_drv.o: %_drv.c
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c
	$(CC) $(CFLAGS_USR) -c -o $@ $<

asynchio.o: asynchio.c asynchio.h
	$(CC) $(CFLAGS_USR) -c -o $@ $<

clean:
	rm -rf *.o obj myCore
