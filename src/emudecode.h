//
// Created by nathi on 24/05/2018.
//

#ifndef ARM11_11_EMUDECODE_H
#define ARM11_11_EMUDECODE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int32_t extract_code(int32_t code, int32_t lower_bit, int32_t upper_bit);

int32_t *convertToBinary(int32_t code);

int32_t convertToDecimal(int32_t *code);

void decode_data_processing(int32_t code, Instruction_t *ins);

void decode_multiply(int32_t code, Instruction_t *ins);

void decode_signal_data_transfer(int32_t code, Instruction_t *ins);

void decode_branch(int32_t code, Instruction_t *ins);

#endif //ARM11_11_EMUDECODE_H
