//
// Created by nathi on 25/05/2018.
//

#ifndef ARM11_11_ALU_H
#define ARM11_11_ALU_H

#include "emustruct.h"

void andd(int a, int b, Condition_Field_t *cond, int *address);

void eor(int a, int b, Condition_Field_t *cond, int *address);

void sub(int a, int b, Condition_Field_t *cond, int *address);

void rsb(int a, int b, Condition_Field_t *cond, int *address);

void add(int a, int b, Condition_Field_t *cond, int *address);

void tst(int a, int b, Condition_Field_t *cond, int *address);

void teq(int a, int b, Condition_Field_t *cond, int *address);

void cmp(int a, int b, Condition_Field_t *cond, int *address);

void orr(int a, int b, Condition_Field_t *cond, int *address);

void move(int a, int b, Condition_Field_t *cond, int *address);

#endif //ARM11_11_ALU_H
