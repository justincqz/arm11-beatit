CC = gcc
CFLAGS = -Wall -Werror -pedantic -std=c99 -Ilib
LDLIBS = -Llib -larm11
BUILD  = 	libs emulate assemble

all: $(BUILD) 

clean: 
	/bin/rm -f $(BUILD) *.o core

spotless: clean
	cd lib; make clean

libs: 
	cd lib; make 

emulate: emulate.o

emulate.o:

assemble: assemble.o

assemble.o: 
