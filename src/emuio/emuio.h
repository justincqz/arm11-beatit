//
// Created by Yoram Boccia on 24/05/2018.
//

#ifndef ARM11_11_EMUIO_H
#define ARM11_11_EMUIO_H

#include <stdint.h>

#define INSTRUCTION_LENGTH 32
#define ONE '1'
#define ZERO '0'
#define EOF '\0'
#define BYTES_IN_WORD 4

//instruction from file to address
int32_t read(char *fileName, int32_t *address);

//helper function to convert binary to decimal and save in the address
int32_t convert(char *buffer, int32_t *address);

//output values of each register and
// the contents of any non-zero memory location
void write(Storage_t *storage);

#endif //ARM11_11_EMUIO_H


