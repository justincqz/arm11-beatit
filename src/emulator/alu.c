//
// Created by nathi on 25/05/2018.
//

#include <assert.h>
#include "emuio.h"
#include "alu.h"

#define UNSIGNED_INT_MAX ~0
#define N_BIT 31
#define Z_BIT 30
#define C_BIT 29

void change_cspr(int a, int *cpsr, int c) {

    assert(cpsr);

    if (c != 1) {
        *cpsr = c ? (*cpsr | 1 << C_BIT) : ((*cpsr & ~(1 << C_BIT)));
    }

    *cpsr = a ? (*cpsr & ~(1 << Z_BIT)) : (*cpsr | (1 << Z_BIT));
    *cpsr |= a & (1 << N_BIT);
}

void andd(uint32_t a, uint32_t b, int *d, int *cpsr, int s) {
    assert(d);

    *d = a & b;

    if (s) {
        change_cspr(*d, cpsr, 0);
    }
}

void eor(uint32_t a, uint32_t b, int *d, int *cpsr, int s) {
    assert(d);

    *d = a ^ b;

    if (s) {
        change_cspr(*d, cpsr, 0);
    }
}

void sub(uint32_t a, uint32_t b, int *d, int *cpsr, int s) {
    assert(d);

    *d = a - b;
    if (s) {
        change_cspr(*d, cpsr, a < b);
    }
}

void rsb(uint32_t a, uint32_t b, int *d, int *cpsr, int s) {
    assert(d);

    sub(b, a, d, cpsr, s);
}

void add(uint32_t a, uint32_t b, int *d, int *cpsr, int s) {
    assert(d);

    *d = a + b;

    if (s) {
        change_cspr(*d, cpsr, b > UNSIGNED_INT_MAX - a);
    }
}

void tst(uint32_t a, uint32_t b, int *cpsr, int s) {

    if (s) {
        change_cspr(a & s, cpsr, -1);
    }
}

void teq(uint32_t a, uint32_t b, int *cpsr, int s) {

    if (s) {
        change_cspr(a ^ b, cpsr, -1);
    }
}

void cmp(uint32_t a, uint32_t b, int *cpsr, int s) {

    if (s) {
        change_cspr(a - b, cpsr, -1);
    }
}

void orr(uint32_t a, uint32_t b, int *d, int *cpsr, int s) {
    assert(d);

    *d = a | b;

    if (s) {
        change_cspr(*d, cpsr, -1);
    }
}

void move(int a, int *d, int *cpsr, int s) {
    assert(d);

    *d = a;

    if (s) {
        change_cspr(a, cpsr, -1);
    }

}
