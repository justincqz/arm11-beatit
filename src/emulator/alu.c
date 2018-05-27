#include"alu.h"
#include"emudef.h"
#define Z_BIT 30
#define N_BIT 31
#define C_BIT 29
#define UNSIGNED_INT_MAX ~0

//execute alu according to the opcode and the spec, and update the cpsr
void alu_execute(Operation_Type opcode, uint32_t arg1, uint32_t arg2, int32_t *destination, int32_t *cpsr, uint32_t s) {
    assert(destination);
    assert(cpsr);
    switch(opcode) {
//*destination = arg1 & arg2
        case AND: 
            and(arg1, arg2, destination, cpsr, s);
            break;
//*destination = arg1 ^ arg2
        case EOR:
            eor(arg1, arg2, destination, cpsr, s);
            break;
//*destination = arg1 - arg2
        case SUB:
            sub(arg1, arg2, destination, cpsr, s);
            break;
//*destination = arg2 - arg1
        case RSB:
            rsb(arg1, arg2, destination, cpsr, s);
            break;
//*destination = arg1 + arg2
        case ADD:
            add(arg1, arg2, destination, cpsr, s);
            break;
//change_cpsr(arg1 & arg2, cpsr, -1)
        case TST: 
            tst(arg1, arg2, cpsr, s);
            break;
//change_cpsr(arg1 ^ arg2, cpsr, -1)
        case TEQ:
            teq(arg1, arg2, cpsr, s);
            break;
//change_cpsr(arg1 - arg2, cpsr, -1)
        case CMP:
            cmp(arg1, arg2, cpsr, s);
            break;
//*destination = arg1 | arg2
        case ORR:
            orr(arg1, arg2, destination, cpsr, s);
            break;
//change_cpsr(arg2, cpsr, -1)
        case MOV:
            mov(arg2, destination, cpsr, s);
            break;
    }
}

//update the cpsr according to the spec
void change_cpsr(int32_t a, int32_t *cpsr, int32_t c) {
    assert(cpsr);

//check if there is overflow and update accordingly the C bit
    if (c != 1) {
        *cpsr = c ? (*cpsr | 1 << C_BIT) : ((*cpsr & ~(1 << C_BIT)));
    }
    *cpsr = a ? (*cpsr & ~(1 << Z_BIT)) : (*cpsr | (1 << Z_BIT));
    *cpsr |= a & (1 << N_BIT);
}

//exercute opcode: and
void and(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    *d = a & b;
    if (s) {
        change_cpsr(*d, cpsr, -1);
    }
}

//exercute opcode: eor
void eor(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    *d = a ^ b;
    if (s) {
        change_cpsr(*d, cpsr, -1);
    }
}

//exercute opcode: sub
void sub(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    *d = a - b;
    if (s) {
//update che cspr and check for overflow
        change_cpsr(*d, cpsr, a < b);
    }
}

//exercute opcode: rsb
void rsb(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    sub(b, a, d, cpsr, s);
}

//exercute opcode: add
void add(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    *d = a + b;
    if (s) {

//update the cspr and checking for overflow
        change_cpsr(*d, cpsr, b > UNSIGNED_INT_MAX - a);
    }
}

//exercute opcode: tst
void tst(uint32_t a, uint32_t b, int32_t *cpsr, uint32_t s) {

    if (s) {
        change_cpsr(a & s, cpsr, -1);
    }
}

//exercute opcode: teq
void teq(uint32_t a, uint32_t b, int32_t *cpsr, uint32_t s) {

    if (s) {
        change_cpsr(a ^ b, cpsr, -1);
    }
}

//exercute opcode: cmp
void cmp(uint32_t a, uint32_t b, int32_t *cpsr, uint32_t s) {

    if (s) {
        change_cpsr(a - b, cpsr, a < b);
    }
}


//exercute opcode: orr
void orr(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    *d = a | b;

    if (s) {
        change_cpsr(*d, cpsr, -1);
    }
}

//exercute opcode: mov
void mov(int32_t a, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);

    *d = a;

    if (s) {
        change_cpsr(a, cpsr, -1);
    }

}

