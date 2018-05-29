CC      = gcc
CFLAGS  = -Wall -g -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: emulate testio testalu testshifter testdecode

testdecode: pipeline.o emustruct.o emuexecute.o emudecode.o operations.o

testdecode.o: pipeline.h emustruct.h emudef.h

testshifter: operations.o

testshifter.o: emudef.h operations.h

testalu: operations.o

testalu.o: emudef.h operations.h

testio: emuio.o emustruct.o

testio.o: emuio.h

emulate: pipeline.o emuio.o emustruct.o emudecode.o emuexecute.o operations.o

emulate.o: emuio.h emudef.h emustruct.h pipeline.h emudecode.h emuexecute.h

emuio.o: emuio.h emustruct.h

pipeline.o: pipeline.h emudef.h emustruct.h emudecode.h emuexecute.h

emudecode.o: emudecode.h emustruct.h

emuexecute.o: emuexecute.h emustruct.h emudef.h

emustruct.o: emustruct.h emudef.h

operations.o: emustruct.h emudef.h

