#ifndef EMUIO_H
#define EMUIO_H

#include "emuio.h"
#include <stdio.h>
#include"arm11struct.h"
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>
#include "arm11def.h"

void emuread(char *file_direction, State_t *state);

void emuwrite(Storage_t *storage);

#endif
