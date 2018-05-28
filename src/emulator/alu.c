#include"alu.h"
#include"emudef.h"
#define Z_BIT 30
#define N_BIT 31
#define C_BIT 29
#define V_BIT 28
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
    uint32_t tmp = (uint32_t) *cpsr;
	uint32_t ua = (uint32_t) a;

    uint32_t n = (ua >> N_BIT);
    uint32_t z = (ua == 0);
    c = c == -1? ((tmp & (1 << C_BIT)) != 0): c;
    uint32_t v = ((tmp & (1U << V_BIT)) != 0); 
    uint32_t nzcv = (((((n << 1)|z) << 1)|c) << 1 |v);
//check if there is overflow and update accordingly the C bit

    tmp = (nzcv << V_BIT)|((tmp << 4) >> 4);
    *cpsr = (int32_t) tmp;
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

    *d = (int32_t)a - (int32_t)b;
    if (s) {
//update che cspr and check for overflow
		uint32_t c = (a >= b); 
        change_cpsr(*d, cpsr, c);
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
		uint32_t c = a < UNSIGNED_INT_MAX - b; 
//update the cspr and checking for overflow
        change_cpsr(*d, cpsr, c);
    }
}

//exercute opcode: tst
void tst(uint32_t a, uint32_t b, int32_t *cpsr, uint32_t s) {

    if (s) {
        change_cpsr(a & b, cpsr, -1);
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
//update che cspr and check for overflow
		uint32_t c = (a >= b); 
        change_cpsr((int32_t)a-(int32_t)b, cpsr, c);
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

//exercute opcode: mov TODO: s is not used
void mov(int32_t a, int32_t *d, int32_t *cpsr, uint32_t s) {
    assert(d);
    *d = a;
}

