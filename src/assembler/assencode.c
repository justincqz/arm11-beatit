#include "arm11def.h"
#include "arm11struct.h"
#include <stdio.h>

// Extract8Bits offset from right
uint32_t extractBits(int input, int offsetFromRight, int numBits) {
  return (((1 << numBits) - 1) & (input >> offsetFromRight));
}

uint32_t setBits(uint32_t num, uint32_t lowerBit, uint32_t upperBit, uint32_t val) {

  uint32_t leftBits = extractBits(num, upperBit + 1, 32 - (upperBit + 1)) << (upperBit + 1);
  uint32_t rightBits = extractBits(num, 0, lowerBit);

  return leftBits + (val << lowerBit) + rightBits;
}

uint32_t encode_data_processing(Instruction_t* ins) {

  uint32_t res = 0;

  res = setBits(res, COND_LOWER_BIT, COND_UPPER_BIT, ins->cond);
  res = setBits(res, OPCODE_LOWER_BIT, OPCODE_UPPER_BIT, ins->opcode);
  res = setBits(res, I_BIT, I_BIT, ins->i);
  res = setBits(res, S_BIT, S_BIT, ins->s);
  res = setBits(res, RN_LOWER_BIT, RN_UPPER_BIT, ins->rn);
  res = setBits(res, RD_LOWER_BIT, RD_UPPER_BIT, ins->rd);

  if (ins->i) {
    res = setBits(res, ROTATE_LOWER_BIT, ROTATE_UPPER_BIT, ins->rotate);
    res = setBits(res, BIT_ZERO, IMMEDIATE_UPPER_BIT, ins->imm);
  } else {
    res = setBits(res, BIT_FOUR, BIT_FOUR, ins->o);
    if (ins->o) {
      res = setBits(res, RS_LOWER_BIT, RS_UPPER_BIT, ins->rs);
    } else {
      res = setBits(res, SC_LOWER_BIT, SC_UPPER_BIT, ins->shift_constant);
    }

    res = setBits(res, ST_LOWER_BIT, ST_UPPER_BIT, ins->shift_type);
    res = setBits(res, RM_LOWER_BIT, RM_UPPER_BIT, ins->rm);
  }

  return res;
}

//decode multiply according to the spec
uint32_t encode_multiply(Instruction_t *ins) {
  uint32_t res = 0;
  res = setBits(res, COND_LOWER_BIT, COND_UPPER_BIT, ins->cond);
  res = setBits(res, RM_UPPER_BIT + 1, RS_LOWER_BIT - 1, 0x9);
  res = setBits(res, A_BIT, A_BIT, ins->a);
  res = setBits(res, S_BIT, S_BIT, ins->s);
  res = setBits(res, RD_MULTIPLY_LOWER_BIT, RD_MULTIPLY_UPPER_BIT, ins->rd);
  res = setBits(res, RN_MULTIPLY_LOWER_BIT, RN_MULTIPLY_UPPER_BIT, ins->rn);
  res = setBits(res, RS_LOWER_BIT, RS_UPPER_BIT, ins->rs);
  res = setBits(res, RM_LOWER_BIT, RM_UPPER_BIT, ins->rm);

  return res;
}

//decode single data transfer according to the spec
uint32_t encode_single_data_transfer(Instruction_t* ins) {
  uint32_t res = 0;
  res = setBits(res, COND_LOWER_BIT, COND_UPPER_BIT, ins->cond);
  res = setBits(res, I_BIT + 1, COND_LOWER_BIT - 1, 0x1);
  res = setBits(res, I_BIT, I_BIT, ins->i);
  res = setBits(res, P_BIT, P_BIT, ins->p);
  res = setBits(res, U_BIT, U_BIT, ins->u);
  res = setBits(res, S_BIT, S_BIT, ins->l);
  res = setBits(res, RN_LOWER_BIT, RN_UPPER_BIT, ins->rn);
  res = setBits(res, RD_LOWER_BIT, RD_UPPER_BIT, ins->rd);

  if (ins->i) {
    if (ins->o) {
      res = setBits(res, RS_LOWER_BIT, RS_UPPER_BIT, ins->rs);
    } else {
      res = setBits(res, SC_LOWER_BIT, SC_UPPER_BIT, ins->shift_constant);
    }

    res = setBits(res, ST_LOWER_BIT, ST_UPPER_BIT, ins->shift_type);
    res = setBits(res, RM_LOWER_BIT, RM_UPPER_BIT, ins->rm);
  } else {
    res = setBits(res, BIT_ZERO, RS_UPPER_BIT, ins->imm);
  }

  return res;
}

//decode branch according to the spec
uint32_t encode_branch(Instruction_t* ins) {
  uint32_t res = 0;
  res = setBits(res, COND_LOWER_BIT, COND_UPPER_BIT, ins->cond);
  res = setBits(res, ADDRESS_UPPER_BIT + 1, COND_LOWER_BIT - 1, 0xA);
  res = setBits(res, BIT_ZERO, ADDRESS_UPPER_BIT, ins->address);
  return res;
}

uint32_t encode_termination(Instruction_t* ins) {
  return 0;
}

uint32_t encode(Instruction_t* ins) {
  switch (ins->instruction_type) {
    case (DATA_PROCESSING):
      return encode_data_processing(ins);
    case (SINGLE_DATA_TRANSFER):
      return encode_single_data_transfer(ins);
    case (BRANCH):
      return encode_branch(ins);
    case (MULTIPLY):
      return encode_multiply(ins);
    case (TERMINATION):
      return encode_termination(ins);
    default:
      return 0;
  }
}
