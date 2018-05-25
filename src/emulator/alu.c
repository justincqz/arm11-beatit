//
// Created by nathi on 25/05/2018.
//

#include <assert.h>
#include "alu.h"

void andd(int a, int b, Condition_Field_t *cond, int *address) {
    assert(address);

    *address = a & b;
}

void eor(int a, int b, Condition_Field_t *cond, int *address) {
    assert(address);

    *address = a ^ b;
}

void sub(int a, int b, Condition_Field_t *cond, int *address) {
    assert(address);

    *address = a - b;
}

void rsb(int a, int b, Condition_Field_t *cond, int *address) {
    assert(address);

    *address = b - a;
}

void add(int a, int b, Condition_Field_t *cond, int *address) {
    assert(address);

    *address = a + b;
}

void tst(int a, int b, Condition_Field_t *cond, int *address) {
    assert(address);

    a & b;
}

void teq(int a, int b, Condition_Field_t *cond, int *address) {
    assert(address);

    a ^ b;
}

void cmp(int a, int b, Condition_Field_t *cond, int *address) {
    assert(address);

    a - b;
}

void orr(int a, int b, Condition_Field_t *cond, int *address) {
    assert(address);

    *address = a | b;
}

void move(int a, int b, Condition_Field_t *cond, int *address) {
    assert(address);
    
}
