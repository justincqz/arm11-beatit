//
// Created by nathi on 25/05/2018.
//

#include <assert.h>
#include "emuutils.h"
#include "emuio.h"


int32_t powerOfTwo(int power) {

    int32_t result = 1;
    for (int i = 0; i < power; ++i) {
        result *= 2;
    }

    return result;

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

int32_t convertToDecimal(int32_t *code, int32_t size) {
    assert(code);

    int32_t decimal = 0;
    int power = size - 1;

    for (int i = 0; i < size; ++i) {
        assert(code[i] == 0 || code[i] == 1);
        if (code[i] == 1) {
            decimal += powerOfTwo(power);
        }
        power--;
    }

    return decimal;
}

