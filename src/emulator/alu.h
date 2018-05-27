#ifndef ALU_H
#define ALU_H

#include <stdint.h>
#include "emustruct.h"
#include"emudef.h"

void alu_execute(Operation_Type opcode, uint32_t arg1, uint32_t arg2, int32_t *destination, int32_t *cpsr, uint32_t s);
void and(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s);
void eor(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s);
void sub(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s);
void rsb(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s);
void add(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s);
void tst(uint32_t a, uint32_t b, int32_t *cpsr, uint32_t s);
void teq(uint32_t a, uint32_t b, int32_t *cpsr, uint32_t s);
void cmp(uint32_t a, uint32_t b, int32_t *cpsr, uint32_t s);
void orr(uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr, uint32_t s);
void mov(int32_t a, int32_t *d, int32_t *cpsr, uint32_t s);

#endif

