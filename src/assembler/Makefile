CC      = gcc
CFLAGS  = -Wall -g -std=c99 -Werror -pedantic
BUILD   = assemble

all: $(BUILD)

clean:
	rm *.o

#testio testalu testshifter testdecode

#testdecode: pipeline.o emustruct.o emuexecute.o emudecode.o operations.o

#testdecode.o: pipeline.h emustruct.h emudef.h

#testshifter: operations.o

#testshifter.o: emudef.h operations.h

#testalu: operations.o

#testalu.o: emudef.h operations.h

#testio: emuio.o emustruct.o

#testio.o: emuio.h

assemble: assemble.c
