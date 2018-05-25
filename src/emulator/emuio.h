#ifndef EMUIO_H
#define EMUIO_H

#include "emustruct.h"
#include "emuutils.h"
#include <stdint.h>

#define INSTRUCTION_LENGTH 32
#define ONE '1'
#define ZERO '0'
#define BYTES_FOR_INT 4

//instruction from file to address
int32_t emuread(char *fileName, int32_t *address);

//helper function to convert binary to decimal and save in the address
int32_t convert(char *buffer, int32_t *address);

//output values of each register and
// the contents of any non-zero memory location
void emuwrite(Storage_t *storage);

#endif
