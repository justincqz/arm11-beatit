CC		= gcc
CFLAGS	= -Wall -g std=c99 -Werror -pendantic -Iemulator
LDLIBS 	= -Lemulator -lemulate 
BUILD	= emulators assemblers emulate assemble


all: $(BUILD)

clean:
	rm -f  $(BUILD) *.o core

spotless:	clean
	cd emulator; make clean; cd ..; cd assembler; make clean

emulators:
	cd emulator; make;mv emulate .. 

assemblers:
	cd assembler;make; mv assemble ..
	
