#ifndef EMUIO_H
#define EMUIO_H

#include "emustruct.h"

int emuread(char *file_direction, int *instructions);
void emuwrite(Storage_t *storage);

#endif
