//
// Created by Yoram Boccia on 24/05/2018.
//

#include "emuio.h"
#include "emudecode.h"

int32_t extract_code(int32_t code, int32_t lower_bit, int32_t upper_bit) {
    int32_t *binary = convertToBinary(code);

    int32_t size = upper_bit - lower_bit + 1;
    int32_t result[size];

    for (int i = 0; i < size; ++i) {
        result[i] = binary[INSTRUCTION_LENGTH - upper_bit - 1 + i];
    }

    return convertToDecimal(result);
}

int32_t convertToDecimal(int32_t *code) {

    int32_t decimal = 0;
    int power = 0;

    for (int i = 0; i < INSTRUCTION_LENGTH; ++i) {
        assert(code[i] == 0 || code[i] == 1);
        if (code[i] == 1) {
            decimal += pow(2, power);
        }
        power++;
    }

    return decimal;
}

int32_t *convertToBinary(int32_t code) {

    static int32_t binary[INSTRUCTION_LENGTH];

    int i = INSTRUCTION_LENGTH - 1;

    while (code > 0) {
        binary[i] = code % 2;
        code /= 2;
        i--;
    }

    return binary;
}

void decode_data_processing(int32_t code, Instruction_t *ins);

void decode_multiply(int32_t code, Instruction_t *ins);

void decode_signal_data_transfer(int32_t code, Instruction_t *ins);

void decode_branch(int32_t code, Instruction_t *ins);

