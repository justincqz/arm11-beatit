CC      = gcc
CFLAGS  = -Wall -g -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: emulate

emulate: pipeline.o emuio.o emustruct.o emudecode.o emuexecute.o alu.o shifter.o emuutils.o

emulate.o: emuio.h emudef.h emustruct.h pipeline.h emudecode.h emuexecute.h

emuio.o: emuio.h emustruct.h

pipeline.o: pipeline.h emudef.h emustruct.h emudecode.h emuexecute.h

emudecode.o: emudecode.h emustruct.h emuutils.h

emuexecute.o: emuexecute.h emustruct.h

emustruct.o: emustruct.h emudef.h

alu.o: alu.h emustruct.h emudef.h

shifter.o: shifter.h emustruct.h

emuutils.o: emuutils.o emustruct.h emudef.h
