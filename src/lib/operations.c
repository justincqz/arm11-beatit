#include "operations.h"
#include<assert.h>
#include "arm11def.h"

uint32_t (*operations[14]) (uint32_t, uint32_t, int32_t *, uint32_t *)= {
  and,
  eor,
  sub,
  rsb,
  add,
  lsl,
  lsr,
  asr,
  tst,
  teq,
  cmp,
  ror,
  orr,
  mov
};

uint32_t directshift[4] = { 5, 6, 7, 11}; 

//update the cpsr according to the spec
void change_cpsr(int32_t a, int32_t *cpsr,int32_t c) {
  //assert(cpsr);
  uint32_t tmp = (uint32_t) *cpsr;
  uint32_t ua = (uint32_t) a;
  uint32_t nzcv = (ua >> N_BIT);
  nzcv <<= 1;
  nzcv |= (ua == 0);
  nzcv <<= 1;
  nzcv |= (c == -1? ((tmp & (1 << C_BIT)) != 0): c);
  nzcv <<= 1;
  nzcv |= ((tmp & (1U << V_BIT)) != 0);
  tmp = (nzcv << V_BIT)|((tmp << 4) >> 4);
  *cpsr = (int32_t) tmp;
}

uint32_t and(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  *c = -1;
  return (*d = a & b);
}

uint32_t eor(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  *c = -1;
  return (*d = a ^ b);
}

uint32_t sub(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  *c = (a >= b);
  return (*d = a - b);
}

uint32_t rsb(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  *c = (b >= a);
  return (*d = b - a);
}

uint32_t add(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  *c = (a > UNSIGNED_INT_MAX-b);
  return (*d = a + b);
}

uint32_t orr(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  *c = -1;
  return (*d = a | b);
}

uint32_t mov(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  *c = -1;/*s = 0*/
  return (*d = b);
}

uint32_t tst(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  *c = -1;
  return a & b;
}


uint32_t teq(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  *c = -1;
  return a ^ b;
}


uint32_t cmp(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  *c = (a >= b);
  return (int32_t) a - (int32_t) b;
}

uint32_t lsl(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  *c = b ? (a >> (WORD_BITS -b)) & 1 : 0;
  return a << b;
}


uint32_t lsr(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  *c = b ? (a >> (b-1)) & 1: 0;
  return a >> b;
}


uint32_t asr(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  *c = b ? (a >> (b-1)) & 1: 0;
  return ((int32_t) a) >> b;
}

uint32_t ror(uint32_t a, uint32_t b, int32_t *d, uint32_t *c) {
  uint32_t tmp = b ? a << (WORD_BITS-b):a;
  *c = b ? (a>>(b-1)) & 1: 0;
  return (a >> b) | tmp;
}



//execute alu according to the opcode and the spec, and update the cpsr
void alu_execute(Operation_Type opcode,uint32_t a, uint32_t b, int32_t *d, int32_t *cpsr,uint32_t s) {
  uint32_t dd,c;
  dd = operations[opcode](a,b,d,&c); 
  if(s){
    change_cpsr(dd,cpsr,c);
  }
}


uint32_t shift( uint32_t a,  uint32_t b, Shift_Type type, int32_t *cpsr,uint32_t s) {
  assert(cpsr);
  uint32_t c = 0;/*overlow flag*/
  uint32_t res = operations[directshift[type]](a,b,NULL,&c);

  if(s) {
        *cpsr |= c*(1 << C_BIT);
  }
  return res;
}




