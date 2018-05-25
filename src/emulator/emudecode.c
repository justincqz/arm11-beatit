#include <math.h>
#include <assert.h>
#include "emustruct.h"
#include "emuio.h"
#include "emudecode.h"
#include "emuutils.h"

#define COND_N_BIT 31
#define COND_Z_BIT 30
#define COND_C_BIT 29
#define COND_V_BIT 28
#define OPCODE_LOWER_BIT 21
#define OPCODE_UPPER_BIT 21
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


int32_t extract_code(int32_t code, int32_t lower_bit, int32_t upper_bit) {
    int32_t *binary = convertToBinary(code);

    int32_t size = upper_bit - lower_bit + 1;
    int32_t result[size];

    for (int i = 0; i < size; ++i) {
        result[i] = binary[INSTRUCTION_LENGTH - upper_bit - 1 + i];
    }

    return convertToDecimal(result, size);
}

void decode_cond_instruction(int32_t code, Instruction_t *ins) {
    assert(ins);

    ins->cond.n = extract_code(code, COND_N_BIT, COND_N_BIT);
    ins->cond.z = extract_code(code, COND_Z_BIT, COND_Z_BIT);
    ins->cond.c = extract_code(code, COND_C_BIT, COND_C_BIT);
    ins->cond.v = extract_code(code, COND_V_BIT, COND_V_BIT);
}

void decode_data_processing(int32_t code, Instruction_t *ins) {
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

void decode_multiply(int32_t code, Instruction_t *ins) {
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

void decode_single_data_transfer(int32_t code, Instruction_t *ins){
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

void decode_branch(int32_t code, Instruction_t *ins){
    assert(ins);
    assert(ins->instruction_type == BRANCH);

    decode_cond_instruction(code, ins);
    ins->address = extract_code(code, BIT_ZERO, ADDRESS_UPPER_BIT);

}
