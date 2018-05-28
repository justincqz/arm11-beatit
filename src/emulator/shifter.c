#include<stdint.h>
#include"shifter.h"
#include<assert.h>
#define C_BIT 29
#define LSL_MASK 1<<31
#define LSR_MASK 1
#define ASR_MASK 1
#define ROR_MASK 1
#define B_CONSTANT 32

uint32_t shift(uint32_t a, uint32_t b, Shift_Type type, int32_t *cpsr) {
    assert(cpsr);
    uint32_t res,tmp;
    int32_t c = 0;/*overlow flag*/

    switch(type) {
        case LSL:
            res = a << b;
            c = b ? (a >> (32-b) != 0): 0;
            break;
        case LSR:
            res = a >> b;
            c = b ? (a << (32-b) != 0): 0;
            break;
        case ASR:
            res = ((int32_t) a) >> b;
            c = b ? (a << (32-b) != 0): 0;
            break;
        case ROR:
            tmp = b ? (a << (32-b)): 0;
            c = (tmp != 0);
            res = (a >> b) | tmp;
            break;
    }
    
	/*if overflow, set the c bit to one*/
    *cpsr |= c*(1 << C_BIT); 

    return res;
}


