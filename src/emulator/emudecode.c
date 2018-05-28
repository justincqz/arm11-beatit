#include<stdint.h>
#include<assert.h>
#include"emustruct.h"
#include"emudecode.h"
#include"emuutils.h"
#define INSTRUCTION_LENGTH 32

//extract bits of the Instruction and return decimal value
uint32_t extract_code(register uint32_t code, register uint32_t lower_bit, register uint32_t upper_bit) {
    code <<= (31-upper_bit);
    code >>= (31-upper_bit);
    code >>= lower_bit;
    return code;
}

//decode the first four bits, the condition part
void decode_cond_instruction(register uint32_t code, Instruction_t *ins) {
    ins->n = extract_code(code, COND_N_BIT, COND_N_BIT);
    ins->z = extract_code(code, COND_Z_BIT, COND_Z_BIT);
    ins->c = extract_code(code, COND_C_BIT, COND_C_BIT);
    ins->v = extract_code(code, COND_V_BIT, COND_V_BIT);
    ins->cond = ((((((ins->n << 1)|ins->z)) << 1)|ins->c) << 1) | ins->v;
}

//decode data processing according to the spec
void decode_data_processing(register uint32_t code, Instruction_t *ins) {
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
        ins->o = extract_code(code, BIT_FOUR, BIT_FOUR);
        if (ins->o) {
            ins->rs = extract_code(code, RS_LOWER_BIT, RS_UPPER_BIT);
        } else {
            ins->shift_constant = extract_code(code, SC_LOWER_BIT, SC_UPPER_BIT);
        }
        ins->shift_type = extract_code(code, ST_LOWER_BIT, ST_UPPER_BIT);
        ins->rm = extract_code(code, RM_LOWER_BIT, RM_UPPER_BIT);
    }
}

//decode multiply according to the spec
void decode_multiply(register uint32_t code, Instruction_t *ins) {
    //asssert(ins);
    //asssert(ins->instruction_type == MULTIPLY);

    decode_cond_instruction(code, ins);
    ins->a = extract_code(code, A_BIT, A_BIT);
    ins->s = extract_code(code, S_BIT, S_BIT);
    ins->rd = extract_code(code, RD_MULTIPLY_LOWER_BIT, RD_MULTIPLY_UPPER_BIT);
    ins->rn = extract_code(code, RN_MULTIPLY_LOWER_BIT, RN_MULTIPLY_UPPER_BIT);
    ins->rs = extract_code(code, RS_LOWER_BIT, RS_UPPER_BIT);
    ins->rm = extract_code(code, RM_LOWER_BIT, RM_UPPER_BIT);

}

//decode single data transfer according to the spec
void decode_single_data_transfer(register uint32_t code, Instruction_t *ins){
    //asssert(ins);
    //asssert(ins->instruction_type == SINGLE_DATA_TRANSFER);

    decode_cond_instruction(code, ins);
    ins->i = extract_code(code, I_BIT, I_BIT);
    ins->p = extract_code(code, P_BIT, P_BIT);
    ins->u = extract_code(code, U_BIT, U_BIT);
    ins->l = extract_code(code, S_BIT, S_BIT);
    ins->rn = extract_code(code, RN_LOWER_BIT, RN_UPPER_BIT);
    ins->rd = extract_code(code, RD_LOWER_BIT, RD_UPPER_BIT);

    if (ins->i) {
        ins->rs = extract_code(code, RS_LOWER_BIT, RS_UPPER_BIT);
		ins->shift_type = (Shift_Type) extract_code(code, ST_LOWER_BIT, ST_UPPER_BIT);

    } else {
        ins->imm = extract_code(code, BIT_ZERO, RS_UPPER_BIT);
    }



}

//decode branch according to the spec
void decode_branch(register uint32_t code, Instruction_t *ins){
    //asssert(ins);
    //asssert(ins->instruction_type == BRANCH);

    decode_cond_instruction(code, ins);
    ins->address = extract_code(code, BIT_ZERO, ADDRESS_UPPER_BIT);

}

