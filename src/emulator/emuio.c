//

// Created by Yoram Boccia on 24/05/2018.
//
#include "emuio.h"

#include <stdio.h>
#include "emuutils.h"

#include"emustruct.h"
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>
#include "emudef.h"

int32_t emuread(char *fileName, int32_t *address) {

    FILE *file;

    char *buffer;
    unsigned long lengthFile;

    file = fopen(fileName, "rb");

    assert(file != NULL);

    if (!file) {
        fprintf(stderr, "Unable to open file %s", fileName);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    lengthFile = (unsigned long) ftell(file);

    assert(lengthFile % INSTRUCTION_LENGTH == 0);

    fseek(file, 0, SEEK_SET);

    buffer = (char *) malloc(lengthFile + 1);

    assert(buffer != NULL);

    if (!buffer) {
        fprintf(stderr, "Memory error!");
        fclose(file);
        return 0;
    }

    fread(buffer, lengthFile, 1, file);
    fclose(file);

    convert(buffer, address);

    free(buffer);

    return (int32_t) (lengthFile / INSTRUCTION_LENGTH);

}

int32_t convert(char *buffer, int32_t *address) {
    int x = 0;
    int y = 0;


    while (buffer[y] != EOF) {
        int decimal = 0;
        int power = INSTRUCTION_LENGTH - 1;
        for (int i = 0; i < INSTRUCTION_LENGTH; ++i) {
            assert(buffer[y] == ZERO || buffer[y] == ONE);
            if (buffer[y] == ONE) {
                decimal += powerOfTwo(power);
            }
            y++;
            power--;
        }
        address[x] = decimal;
        x++;
    }

    return 0;
}

void emuwrite(Storage_t *storage) {

    assert(storage != NULL);

    printf("Registers:\n");

    for (int i = 0; i < 13; i++) {
        printf("$%-3d", i);
        printf(": %10d (0x%08x)\n", storage->reg[i], storage->reg[i]);
    }

    printf("PC");
    printf(": %10d (0x%08x)\n", storage->reg[PC_REG], storage->reg[PC_REG]);

//  PART I: initialize the CPSR to 0
    printf("CPSR");
    int cpsr = 0;
    printf(": %10d (0x%08x)\n", cpsr, cpsr);

    printf("Non-zero memory location: \n");
    for (int i = 0; i < MEMORY_SIZE; i += BYTES_FOR_INT) {
        if (((int *) storage->mem)[i / BYTES_FOR_INT] != 0) {
            printf("0x%08x: 0x", i);
            for (int j = 0; j < BYTES_FOR_INT; j++) {
                printf("%02x", storage->mem[i + j]);
            }
            printf("\n");
        }
    }
}

