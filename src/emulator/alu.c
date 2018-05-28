#include"alu.h"
#include"emudef.h"
#define Z_BIT 30
#define N_BIT 31
#define C_BIT 29
#define V_BIT 28
#define UNSIGNED_INT_MAX ~0

//update the cpsr according to the spec
void change_cpsr(register int32_t a, int32_t *cpsr,register int32_t c) {
    //assert(cpsr);
    register uint32_t tmp = (uint32_t) *cpsr;
	register uint32_t ua = (uint32_t) a;
    register uint32_t nzcv = (ua >> N_BIT);
    nzcv <<= 1;
    nzcv |= (ua == 0);
    nzcv <<= 1;
    nzcv |= (c == -1? ((tmp & (1 << C_BIT)) != 0): c);
    nzcv <<= 1;
    nzcv |= ((tmp & (1U << V_BIT)) != 0); 
    tmp = (nzcv << V_BIT)|((tmp << 4) >> 4);
    *cpsr = (int32_t) tmp;
}

//execute alu according to the opcode and the spec, and update the cpsr
void alu_execute(Operation_Type opcode,register uint32_t a, register uint32_t b, int32_t *d, int32_t *cpsr,register uint32_t s) {
    //assert(destination);
    //assert(cpsr);
    register uint32_t dd,c;

    switch(opcode) {
        case AND: 
            dd = *d = a & b;
            c = -1;
            break;
              
        case EOR:
            dd = *d = a ^ b;
            c = -1;
            break;
        
        case SUB:
            dd = *d = a - b;
            c = (a>=b);
            break;
        
        case RSB:
            dd = *d = b - a;
            c = (b>=a);
            break;

        case ADD:
            dd = *d = a + b;
            c = (a > UNSIGNED_INT_MAX-b);
            break;
        case ORR:
            dd = *d = a|b;
            c = -1;
            break;
        case MOV:
            dd = *d = b;/*check*/
            c =  -1;
            s = 0;
            break;
        case TST:
            dd = a&b;
            c = -1;
            break;
        case TEQ:
            dd = a^b;
            c = -1;
            break;
        case CMP:
            dd = (int32_t) a - (int32_t) b;
            c = (a >= b);
            break;
            
        default: break;
    }

    if(s) {
        change_cpsr(dd,cpsr,c);
    }
}



