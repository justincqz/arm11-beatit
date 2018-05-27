#ifndef EMUIO_H
#define EMUIO_H

#include "emuio.h"
#include <stdio.h>
#include "emuutils.h"
#include"emustruct.h"
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>
#include "emudef.h"

uint32_t *emuread(char *file_direction, size_t *size);
void emuwrite(Storage_t *storage);

#endif
