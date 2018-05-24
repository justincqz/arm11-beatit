//
// Created by Yoram Boccia on 24/05/2018.
//

#include <stdint.h>

int32_t const INSTRUCTION_LENGTH = 32;
char const ONE = '1';
char const ZERO = '0';
char const EOF = '\0';

//instruction from file to address
int32_t read(char *fileName, int32_t *address);

//helper function to convert binary to decimal and save in the address
int32_t convert(char *buffer, int32_t *address);

//output values of each register and
// the contents of any non-zero memory location
void write(Storage_t *storage);

