#ifndef EMUDECODE_H
#define EMUDECODE_H

#include <math.h>
#include <assert.h>
#include "emustruct.h"
#include "emuio.h"
#include "emudecode.h"

void decode_cond_instruction(uint32_t code, Instruction_t *ins);
uint32_t extract_code(register uint32_t code,register uint32_t lower_bit, register uint32_t upper_bit);
void decode_data_processing(State_t *state);
void decode_multiply(State_t *state);
void decode_single_data_transfer(State_t *t);
void decode_branch(State_t *t);

#endif
