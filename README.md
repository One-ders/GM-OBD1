# GM-OBD1

What is it, and what does it do.
================================
1. An implemetation to communicate with old GM ECM's specifically 1227165.
2. it runs on a BLACKPILL 10$ board, but any STM 32 would do.
3. The file config.h specifies the pin to be used for ECM communication (PB10 rx and PB3 tx),
Earlier version (last commit: 82b610d) used one wire with a serial 10K resistor between ALDL line and MCU. But a small
2 transistor level adaptor was created to better control voltage levels. (ECM is 5 volts. STM is 3.3).
![OBD1 connector with adaptor and schematic](./pics/20220803_140636.jpg?raw=true "OBD1 connector with adaptor")

4. on the other side it will publish an usb-serial interface. Here a raw byte stream will presented to a laptop running a scanner software.
5. the Interface will start by reading the 160 baud 25 bytes, frames.
This can be used to get most of the information needed.
6. It  will try to initiate 8192 Baud mode, with 64 bytes frames.


How to build
============

this software is implemented as an application on top of the RTScheduler OS (also available on my github account).

1. Checkout the RTScheduler and build it. An arm compiler will be needed. but that should be documented in the RTScheduler OS repo.
2. Modify the Makefile macro KREL to point into to the build directory of the
RTScheduler.
3. make

the result will be a loadfile myCore. use the st-link program and arm-(xxx)-gdb to burn it to the micro controller.

-------------------------------------------------------
