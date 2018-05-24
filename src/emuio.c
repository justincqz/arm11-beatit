//
// Created by Yoram Boccia on 24/05/2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include <ctype.h>
#include "emuio.h"
#include "emuutils.h"
#include "emudef.h"

int32_t read(char *fileName, int32_t *address) {

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
    lengthFile = ftell(file);

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

    return sizeof(address) / sizeof(address[0]);

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
                decimal += pow(2, power);
            }
            y++;
            power--;
        }
        address[x] = decimal;
        x++;
    }

    return 0;
}

void write(Storage_t *storage) {

    assert(storage != NULL);

    for (int i = 0; i < NUMBERS_OF_REG; ++i) {
        printf("Reg%d = %d\n", i, storage.reg[i]);
    }

    for (int j = 0; j < MEMORY_SIZE; ++j) {
        if (storage.mem[i] != 0) {
            printf("The content at Memory location %d is %d\n",
                   storage.mem[i], *storage.mem[i]);
        }
    }
}

