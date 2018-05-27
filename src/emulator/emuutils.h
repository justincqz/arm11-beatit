//
// Created by nathi on 25/05/2018.
//

#ifndef ARM11_11_EMUUTILS_H
#define ARM11_11_EMUUTILS_H

#include <stdint.h>

uint32_t *convertToBinary(uint32_t code);

int32_t convertToDecimal(int32_t *code, int32_t size);

int32_t powerOfTwo(int power);

#endif //ARM11_11_EMUUTILS_H
