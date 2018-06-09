
#ifndef ARM11_11_OPERATIONS_H
#define ARM11_11_OPERATIONS_H

#include<stdint.h>
#include"arm11struct.h"
#include"arm11def.h"



void alu_execute(Operation_Type opcode, uint32_t arg1, uint32_t arg2, int32_t *destination, int32_t *cpsr, uint32_t s);
uint32_t and(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);
uint32_t eor(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);
uint32_t sub(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);
uint32_t rsb(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);
uint32_t add(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);
uint32_t tst(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);
uint32_t teq(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);
uint32_t cmp(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);
uint32_t orr(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);
uint32_t mov(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);
uint32_t lsl(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);
uint32_t lsr(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);
uint32_t asr(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);
uint32_t ror(uint32_t a, uint32_t b, int32_t *d, uint32_t *c);



uint32_t shift(uint32_t a, uint32_t b, Shift_Type type, int32_t *cpsr, uint32_t s);
uint32_t shift_lsl(uint32_t a, uint32_t b,int32_t *c);
uint32_t shift_lsr(uint32_t a, uint32_t b,int32_t *c);
uint32_t shift_asr(uint32_t a, uint32_t b,int32_t *c);
uint32_t shift_ror(uint32_t a, uint32_t b,int32_t *c);


#endif //ARM11_11_OPERATIONS_H
