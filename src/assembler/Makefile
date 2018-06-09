CC      = gcc
CFLAGS  = -Wall -g -std=c99 -Werror -pedantic
BUILD   = imports assemble clean_imports clean

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

assemble: assemble.o strutils.o parsedata.o parseStr.o arm11struct.o assencode.o

assemble.o: arm11struct.h arm11def.h parseStr.h strutils.h

strutils.o: strutils.h

parsedata.o: parsedata.h strutils.h assencode.h

assencode.o: assencode.h

parseStr.o: parsedata.h parseStr.h arm11struct.h

imports:
	cd ../utilities; make; cp ./*.o ../assembler/; cp ./*.h ../assembler/

clean_imports:
	rm arm11def.h; rm arm11struct.h
