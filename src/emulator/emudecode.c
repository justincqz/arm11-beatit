#include<stdint.h>
#include<assert.h>
#include"emustruct.h"
#include"emudecode.h"
#define INSTRUCTION_LENGTH 32
#include"emuutils.h"
uint32_t extract_code(uint32_t code, uint32_t lower_bit, uint32_t upper_bit) {
    uint32_t *binary = convertToBinary(code);

    uint32_t size = upper_bit - lower_bit + 1;
    int32_t result[size];

    for (int i = 0; i < size; ++i) {
        result[i] = binary[INSTRUCTION_LENGTH - upper_bit - 1 + i];
    }

    return convertToDecimal(result, size);
}

void decode_cond_instruction(uint32_t code, Instruction_t *ins) {
    assert(ins);

    ins->n = extract_code(code, COND_N_BIT, COND_N_BIT);
    ins->z = extract_code(code, COND_Z_BIT, COND_Z_BIT);
    ins->c = extract_code(code, COND_C_BIT, COND_C_BIT);
    ins->v = extract_code(code, COND_V_BIT, COND_V_BIT);
    ins->cond = ((((((ins->n << 1)|ins->z)) << 1)|ins->c) << 1) | ins->v;
}

void decode_data_processing(uint32_t code, Instruction_t *ins) {
    assert(ins);
    assert(ins->instruction_type == DATA_PROCESSING);

    decode_cond_instruction(code, ins);
    ins->opcode = extract_code(code, OPCODE_LOWER_BIT, OPCODE_UPPER_BIT);
    ins->i = extract_code(code, I_BIT, I_BIT);
    ins->s = extract_code(code, S_BIT, S_BIT);
    ins->rn = extract_code(code, RN_LOWER_BIT, RN_UPPER_BIT);
    ins->rd = extract_code(code, RD_LOWER_BIT, RD_UPPER_BIT);

    if (ins->i) {
        ins->rotate = extract_code(code, ROTATE_LOWER_BIT, ROTATE_UPPER_BIT);
        ins->imm = extract_code(code, BIT_ZERO, IMMEDIATE_UPPER_BIT);
    } else {
        if (extract_code(code, BIT_FOUR, BIT_FOUR)) {
            ins->rs = extract_code(code, RS_LOWER_BIT, RS_UPPER_BIT);
        } else {
            ins->shift_constant = extract_code(code, SC_LOWER_BIT, SC_UPPER_BIT);
        }
        ins->shift_type = (Shift_Type) extract_code(code, ST_LOWER_BIT, ST_UPPER_BIT);
        ins->rm = extract_code(code, RM_LOWER_BIT, RM_UPPER_BIT);
    }
}

void decode_multiply(uint32_t code, Instruction_t *ins) {
    assert(ins);
    assert(ins->instruction_type == MULTIPLY);

    decode_cond_instruction(code, ins);
    ins->a = extract_code(code, A_BIT, A_BIT);
    ins->s = extract_code(code, S_BIT, S_BIT);
    ins->rd = extract_code(code, RD_MULTIPLY_LOWER_BIT, RD_MULTIPLY_UPPER_BIT);
    ins->rn = extract_code(code, RN_MULTIPLY_LOWER_BIT, RN_MULTIPLY_UPPER_BIT);
    ins->rs = extract_code(code, RS_LOWER_BIT, RS_UPPER_BIT);
    ins->rm = extract_code(code, RM_LOWER_BIT, RM_UPPER_BIT);

}

void decode_single_data_transfer(uint32_t code, Instruction_t *ins){
    assert(ins);
    assert(ins->instruction_type == SINGLE_DATA_TRANSFER);

    decode_cond_instruction(code, ins);
    ins->i = extract_code(code, I_BIT, I_BIT);
    ins->p = extract_code(code, P_BIT, P_BIT);
    ins->u = extract_code(code, U_BIT, U_BIT);
    ins->l = extract_code(code, S_BIT, S_BIT);
    ins->rn = extract_code(code, RN_LOWER_BIT, RN_UPPER_BIT);
    ins->rd = extract_code(code, RD_LOWER_BIT, RD_UPPER_BIT);

    if (ins->i) {
        ins->rs = extract_code(code, BIT_ZERO, RS_UPPER_BIT);
    } else {
        ins->imm = extract_code(code, BIT_ZERO, RS_UPPER_BIT);
    }

}

void decode_branch(uint32_t code, Instruction_t *ins){
    assert(ins);
    assert(ins->instruction_type == BRANCH);

    decode_cond_instruction(code, ins);
    ins->address = extract_code(code, BIT_ZERO, ADDRESS_UPPER_BIT);

}

