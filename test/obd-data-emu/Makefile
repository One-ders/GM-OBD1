



all: obd1-menu obd_emu

obd1-menu: obd1-menu/obd1

obd1-menu/obd1:
	(cd obd1-menu; $(MAKE))

obd_emu: main.o
	gcc -o $@ $^

test-run: obd_emu
	./obd_emu -d data2.cap

clean:
	(cd obd1-menu; $(MAKE) clean)
	rm -rf obd_emu *.o
