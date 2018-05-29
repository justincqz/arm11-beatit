CC		= gcc
CFLAGS	= -Wall -g std=c99 -Werror -pendantic -Iemulator
LDLIBS 	= -Lemulator -lemulate 
BUILD	= emulators emulate 


all: $(BUILD)

clean:
	rm -f  $(BUILD) *.o core

spotless:	clean
	cd emulator; make clean

emulators:
	cd emulator; make;mv emulate .. 
	
