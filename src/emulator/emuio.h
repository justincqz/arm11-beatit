#ifndef EMUIO_H
#define EMUIO_H

#include "emuio.h"
#include <stdio.h>
#include"emustruct.h"
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>
#include "emudef.h"

void emuread(char *file_direction, State_t *state);

void emuwrite(Storage_t *storage);

#endif
