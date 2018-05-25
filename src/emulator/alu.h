//
// Created by nathi on 25/05/2018.
//

#ifndef ARM11_11_ALU_H
#define ARM11_11_ALU_H

#include <stdint.h>
#include "emustruct.h"

void andd(uint32_t a, uint32_t b, int *d, int *cpsr, int s);

void eor(uint32_t a, uint32_t b, int *d, int *cpsr, int s);

void sub(uint32_t a, uint32_t b, int *d, int *cpsr, int s);

void rsb(uint32_t a, uint32_t b, int *d, int *cpsr, int s);

void add(uint32_t a, uint32_t b, int *d, int *cpsr, int s);

void tst(uint32_t a, uint32_t b, int *cpsr, int s);

void teq(uint32_t a, uint32_t b, int *cpsr, int s);

void cmp(uint32_t a, uint32_t b, int *cpsr, int s);

void orr(uint32_t a, uint32_t b, int *d, int *cpsr, int s);

void move(int a, int *d, int *cpsr, int s);

#endif //ARM11_11_ALU_H
