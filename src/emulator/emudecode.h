#ifndef EMUDECODE_H
#define EMUDECODE_H

#include "emustruct.h"
#include "emuutils.h"


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int32_t extract_code(int32_t code, int32_t lower_bit, int32_t upper_bit);

void decode_data_processing(int32_t code, Instruction_t *ins);

void decode_multiply(int32_t code, Instruction_t *ins);

void decode_single_data_transfer(int32_t code, Instruction_t *ins);

void decode_branch(int32_t code, Instruction_t *ins);

void decode_cond_instruction(int32_t code, Instruction_t *ins);

#endif
