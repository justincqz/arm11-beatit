#include"alu.h"
#include"emudef.h"
#define Z_BIT 30
#define N_BIT 31
#define C_BIT 29
#define UNSIGNED_INT_MAX ~0

void alu_execute(Operation_Type opcode, uint32_t arg1, uint32_t arg2, int32_t *destination, int32_t *cpsr, uint32_t s) {
    assert(destination);
    assert(cpsr);
    switch(opcode) {
        case AND: 
            and(arg1, arg2, destination, cpsr, s);
            break;
        case EOR:
            eor(arg1, arg2, destination, cpsr, s);
            break;
        case SUB:
            sub(arg1, arg2, destination, cpsr, s);
            break;
        case RSB:
            rsb(arg1, arg2, destination, cpsr, s);
            break;
        case ADD:
            add(arg1, arg2, destination, cpsr, s);
            break;
        case TST: 
            tst(arg1, arg2, cpsr, s);
            break;
        case TEQ:
            teq(arg1, arg2, cpsr, s);
            break;
        case CMP:
            cmp(arg1, arg2, cpsr, s);
            break;
        case ORR:
            orr(arg1, arg2, destination, cpsr, s);
            break;
        case MOV:
            mov(arg2, destination, cpsr, s);
            break;
    }
}

void change_cspr(int32_t a, int32_t *cpsr, int32_t c) {
    assert(cpsr);

    if (c != 1) {
        *cpsr = c ? (*cpsr | 1 << C_BIT) : ((*cpsr & ~(1 << C_BIT)));
    }
    *cpsr = a ? (*cpsr & ~(1 << Z_BIT)) : (*cpsr | (1 << Z_BIT));
    *cpsr |= a & (1 << N_BIT);
}

void and(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    *d = a & b;
    if (s) {
        change_cspr(*d, cpsr, -1);
    }
}

void eor(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    *d = a ^ b;
    if (s) {
        change_cspr(*d, cpsr, -1);
    }
}

void sub(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    *d = a - b;
    if (s) {
        change_cspr(*d, cpsr, a < b);
    }
}

void rsb(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    sub(b, a, d, cpsr, s);
}

void add(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    *d = a + b;
    if (s) {
        change_cspr(*d, cpsr, b > UNSIGNED_INT_MAX - a);
    }
}

void tst(uint32_t a, uint32_t b, int32_t *cpsr, uint32_t s) {

    if (s) {
        change_cspr(a & s, cpsr, -1);
    }
}

void teq(uint32_t a, uint32_t b, int32_t *cpsr, uint32_t s) {

    if (s) {
        change_cspr(a ^ b, cpsr, -1);
    }
}

void cmp(uint32_t a, uint32_t b, int32_t *cpsr, uint32_t s) {

    if (s) {
        change_cspr(a - b, cpsr, a < b);
    }
}

void orr(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    *d = a | b;

    if (s) {
        change_cspr(*d, cpsr, -1);
    }
}

void mov(int32_t a, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    *d = a;

    if (s) {
        change_cspr(a, cpsr, -1);
    }

}

