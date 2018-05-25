CC      = gcc
CFLAGS  = -Wall -g -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: emulate

emulate: pipeline.o emuio.o emustruct.o emudecode.o emuexecute.o emuutils.o

emulate.o: emuio.h emudef.h emustruct.h pipeline.h emudecode.h emuexecute.h

emuio.o: emustruct.h

pipeline.o: emudef.h emustruct.h emudecode.h emuexecute.h

emudecode.o: emustruct.h

emuexecute.o: emustruct.h

emustruct.o: emustruct.h emudef.h

emuutils.o: emuutils.h emuio.h
