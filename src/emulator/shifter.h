#ifndef SHIFTER_H
#define SHIFTER_H

#include<stdint.h>
#include"emustruct.h"

uint32_t shift(uint32_t a, uint32_t b, Shift_Type type, int32_t *cpsr, uint32_t s);
uint32_t shift_lsl(uint32_t a, uint32_t b,int32_t *c); 
uint32_t shift_lsr(uint32_t a, uint32_t b,int32_t *c);
uint32_t shift_asr(uint32_t a, uint32_t b,int32_t *c); 
uint32_t shift_ror(uint32_t a, uint32_t b,int32_t *c);

#endif
