#include "arm11struct.h"

uint32_t set_bits(uint32_t num, uint32_t lowerBit, uint32_t upperBit, uint32_t val);

uint32_t encode(Instruction_t* ins);

uint32_t encode_cond_instruction(Instruction_t *ins);

uint32_t encode_data_processing(Instruction_t* ins);

uint32_t encode_multiply(Instruction_t *ins);

uint32_t encode_single_data_transfer(Instruction_t* ins);

uint32_t encode_branch(Instruction_t* ins);

uint32_t encode_termination(Instruction_t* ins);
