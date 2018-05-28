#ifndef EMUDECODE_H
#define EMUDECODE_H

#include <math.h>
#include <assert.h>
#include "emustruct.h"
#include "emuio.h"
#include "emudecode.h"

#define COND_N_BIT 31
#define COND_Z_BIT 30
#define COND_C_BIT 29
#define COND_V_BIT 28
#define OPCODE_LOWER_BIT 21
#define OPCODE_UPPER_BIT 24
#define I_BIT 25
#define P_BIT 24
#define U_BIT 23
#define A_BIT 21
#define S_BIT 20
#define RN_LOWER_BIT 16
#define RN_UPPER_BIT 19
#define RN_MULTIPLY_LOWER_BIT 12
#define RN_MULTIPLY_UPPER_BIT 15
#define RD_MULTIPLY_LOWER_BIT 16
#define RD_MULTIPLY_UPPER_BIT 19
#define RD_LOWER_BIT 12
#define RD_UPPER_BIT 15
#define RS_LOWER_BIT 8
#define RS_UPPER_BIT 11
#define SC_LOWER_BIT 7
#define SC_UPPER_BIT 11
#define ST_LOWER_BIT 5
#define ST_UPPER_BIT 6
#define ROTATE_LOWER_BIT 8
#define ROTATE_UPPER_BIT 11
#define RM_LOWER_BIT 0
#define RM_UPPER_BIT 3
#define BIT_ZERO 0
#define BIT_FOUR 4
#define ADDRESS_UPPER_BIT 23
#define IMMEDIATE_UPPER_BIT 7

void decode_cond_instruction(uint32_t code, Instruction_t *ins);
uint32_t extract_code(register uint32_t code,register uint32_t lower_bit, register uint32_t upper_bit);
void decode_data_processing(State_t *state);
void decode_multiply(State_t *state);
void decode_single_data_transfer(State_t *t);
void decode_branch(State_t *t);

#endif
