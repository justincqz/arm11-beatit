#ifndef PARSESTR_H
#define PARSESTR_H

#include "parsedata.h"
#include "arm11struct.h"
#include <stdint.h>

uint32_t parseStr(char* inp, Sym_t** symT, int currLine, int numInst, uint32_t* sdtAppend, size_t sizeOfSymT);

#endif
