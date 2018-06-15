#include<stdint.h>
#include<assert.h>
#include"arm11struct.h"
#include"emudecode.h"
#include"arm11def.h"
#include"arm11utils.h"

void decode_cond_instruction(register uint32_t code, Instruction_t *ins) {
  ins->n = extract_bits(code, N_BIT, N_BIT);
  ins->z = extract_bits(code, Z_BIT, Z_BIT);
  ins->c = extract_bits(code, C_BIT, C_BIT);
  ins->v = extract_bits(code, V_BIT, V_BIT);
  ins->cond = ((((((ins->n << 1)|ins->z)) << 1)|ins->c) << 1) | ins->v;
}

void decode_data_processing(State_t *state) {
  Instruction_t *ins = state->decoded_ins;
  uint32_t code = state->fetched_code;


  decode_cond_instruction(code, ins);
  ins->opcode = extract_bits(code, OPCODE_LOWER_BIT, OPCODE_UPPER_BIT);
  ins->i = extract_bits(code, I_BIT, I_BIT);
  ins->s = extract_bits(code, S_BIT, S_BIT);
  ins->rn = extract_bits(code, RN_LOWER_BIT, RN_UPPER_BIT);
  ins->rd = extract_bits(code, RD_LOWER_BIT, RD_UPPER_BIT);

  if (ins->i) {
    ins->rotate = extract_bits(code, ROTATE_LOWER_BIT, ROTATE_UPPER_BIT);
    ins->imm = extract_bits(code, BIT_ZERO, IMMEDIATE_UPPER_BIT);
  } else {
    ins->o = extract_bits(code, BIT_FOUR, BIT_FOUR);
    if (ins->o) {
      ins->rs = extract_bits(code, RS_LOWER_BIT, RS_UPPER_BIT);
    } else {
      ins->shift_constant = extract_bits(code, SC_LOWER_BIT, SC_UPPER_BIT);
    }
    ins->shift_type = extract_bits(code, ST_LOWER_BIT, ST_UPPER_BIT);
    ins->rm = extract_bits(code, RM_LOWER_BIT, RM_UPPER_BIT);
  }
}

void decode_multiply(State_t *state) {
  Instruction_t *ins = state->decoded_ins;
  uint32_t code = state->fetched_code;
  decode_cond_instruction(code, ins);
  ins->a = extract_bits(code, A_BIT, A_BIT);
  ins->s = extract_bits(code, S_BIT, S_BIT);
  ins->rd = extract_bits(code, RD_MULTIPLY_LOWER_BIT, RD_MULTIPLY_UPPER_BIT);
  ins->rn = extract_bits(code, RN_MULTIPLY_LOWER_BIT, RN_MULTIPLY_UPPER_BIT);
  ins->rs = extract_bits(code, RS_LOWER_BIT, RS_UPPER_BIT);
  ins->rm = extract_bits(code, RM_LOWER_BIT, RM_UPPER_BIT);
}

void decode_single_data_transfer(State_t *state){
  Instruction_t *ins = state->decoded_ins;
  uint32_t code = state->fetched_code;
  decode_cond_instruction(code, ins);
  ins->i = extract_bits(code, I_BIT, I_BIT);
  ins->p = extract_bits(code, P_BIT, P_BIT);
  ins->u = extract_bits(code, U_BIT, U_BIT);
  ins->l = extract_bits(code, S_BIT, S_BIT);
  ins->rn = extract_bits(code, RN_LOWER_BIT, RN_UPPER_BIT);
  ins->rd = extract_bits(code, RD_LOWER_BIT, RD_UPPER_BIT);

  if (ins->i) {
    if (ins->o) {
        ins->rs = extract_bits(code, RS_LOWER_BIT, RS_UPPER_BIT);
    } else {
        ins->shift_constant = extract_bits(code, SC_LOWER_BIT, SC_UPPER_BIT);
    }
    ins->shift_type = extract_bits(code, ST_LOWER_BIT, ST_UPPER_BIT);
    ins->rm = extract_bits(code, RM_LOWER_BIT, RM_UPPER_BIT);
  } else {
    ins->imm = extract_bits(code, BIT_ZERO, RS_UPPER_BIT);
  }
}

void decode_branch(State_t *state) {
  decode_cond_instruction(state->fetched_code, state->decoded_ins);
  state->decoded_ins->address = extract_bits(state->fetched_code, BIT_ZERO, ADDRESS_UPPER_BIT);
}

void decode_termination(State_t *state) {
  return;
}
