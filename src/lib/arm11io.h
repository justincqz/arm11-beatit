#ifndef EMUIO_H
#define EMUIO_H

#include "arm11io.h"
#include <stdio.h>
#include"arm11struct.h"
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>
#include "arm11def.h"
#include <string.h>

void emuread(char *file_direction, State_t *state);

void emuwrite(Storage_t *storage);

FILE* assread(char *path);

int asswrite(char* outFile, uint32_t* binTable, uint32_t noInst);

#endif
