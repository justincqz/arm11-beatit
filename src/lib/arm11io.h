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

void emu_read(char *file_direction, State_t *state);
void emu_write(Storage_t *storage);

FILE* ass_read(char *path);
int ass_write(char* outFile, uint32_t* binTable, uint32_t noInst);

#endif
