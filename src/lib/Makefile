CC      = gcc
CFLAGS  = -Wall -g -std=c99 -Werror -pedantic
LIB     = libarm11.a
EMUOBJS = pipeline.o arm11io.o emudecode.o emuexecute.o operations.o
ASSOBJS = parsedata.o parsestr.o assencode.o strutils.o
COMOBJS = arm11struct.o
LIBOBJS = $(COMOBJS) $(EMUOBJS) $(ASSOBJS)
BUILD   = $(LIB)

all: $(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o core

$(LIB): $(LIBOBJS)
	ar rc $(LIB) $(LIBOBJS)
	ranlib $(LIB)

arm11io.o: arm11io.h arm11struct.h

pipeline.o: pipeline.h emudecode.h emuexecute.h

emudecode.o: emudecode.h

emuexecute.o: emuexecute.h

arm11struct.o: arm11struct.h

operations.o: operations.h

strutils.o: strutils.h

parsedata.o: parsedata.h strutils.h assencode.h

assencode.o: assencode.h

parsestr.o: parsedata.h parsestr.h arm11struct.h
