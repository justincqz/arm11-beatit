#include<stdint.h>
#include"shifter.h"
#include<assert.h>
#define C_BIT 29
#define WORD_BITS 32


uint32_t shift( uint32_t a,  uint32_t b, Shift_Type type, int32_t *cpsr,uint32_t s) {
    assert(cpsr);
     uint32_t res,tmp;
     int32_t c = 0;/*overlow flag*/

    switch(type) {
        case LSL:
            res = a << b;
            c = b ? (a >> (WORD_BITS -b)) & 1 : 0;
            break;
        case LSR:
            res = a >> b;
            c = b ? (a >> (b-1)) & 1: 0;
            break;
        case ASR:
            res = ((int32_t) a) >> b;
            c = b ? (a >> (b-1)) & 1: 0;
            break;
        case ROR:
            tmp = b ? a << (WORD_BITS-b):a;
			c = b ? (a>>(b-1)) & 1: 0;
            res = (a >> b) | tmp;
            break;
    }
    
	/*if overflow, set the c bit to one*/
	if(s) {
    	*cpsr |= c*(1 << C_BIT);
	}

    return res;
}


