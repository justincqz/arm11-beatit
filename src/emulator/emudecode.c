#include <math.h>
#include <assert.h>
#include "emustruct.h"
#include "emuio.h"
#include "emudecode.h"
#include "emuutils.h"

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
    ins->cond.n = extract_code(code, 31, 31);
    ins->cond.z = extract_code(code, 30, 30);
    ins->cond.c = extract_code(code, 29, 29);
    ins->cond.v = extract_code(code, 28, 28);
}

void decode_data_processing(int32_t code, Instruction_t *ins) {
    assert(ins->instruction_type == DATA_PROCESSING);

    decode_cond_instruction(code, ins);
    ins->opcode = extract_code(code, 21, 24);
    ins->s = extract_code(code, 20, 20);
    ins->rn = extract_code(code, 16, 19);
    ins->rd = extract_code(code, 12, 15);


}

void decode_multiply(int32_t code, Instruction_t *ins){
    assert(ins->instruction_type == MULTIPLY);

    decode_cond_instruction(code, ins);
    ins->a = extract_code(code, 21, 21);
    ins->s = extract_code(code, 20, 20);
    ins->rd = extract_code(code, 16, 19);
    ins->rn = extract_code(code, 12, 15);
    ins->rs = extract_code(code, 8, 11);
    ins->rm = extract_code(code, 0, 3);

}

void decode_single_data_transfer(int32_t code, Instruction_t *ins){
    assert(ins->instruction_type == SINGLE_DATA_TRANSFER);

    decode_cond_instruction(code, ins);
    ins->i = extract_code(code, 25, 25);
    ins->p = extract_code(code, 24, 24);
    ins->u = extract_code(code, 23, 23);
    ins->l = extract_code(code, 20, 20);
    ins->rn = extract_code(code, 16, 19);
    ins->rd = extract_code(code, 12, 15);

}

void decode_branch(int32_t code, Instruction_t *ins){
    assert(ins->instruction_type == BRANCH);

    decode_cond_instruction(code, ins);

}
