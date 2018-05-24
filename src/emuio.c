//
// Created by Yoram Boccia on 24/05/2018.
//

#include <stdio.h>
#include <math.h>
#include "emuio.h"

int read(char *fileName, int *address) {

    FILE *file;

    char *buffer;
    unsigned long lengthFile;

    char *buffer;
    unsigned long fileLen;

    file = fopen(fileName, "r");

    if (!file) {
        fprintf(stderr, "Unable to open file %s", fileName);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    fileLen = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer = (char *) malloc(fileLen + 1);

    if (!buffer) {
        fprintf(stderr, "Memory error!");
        fclose(file);
        return 0;
    }

    fread(buffer, fileLen, 1, file);
    fclose(file);

    convert(buffer, address);

    free(buffer);

}

int convert(char *buffer, int *address) {
    int x = 0;
    int y = 0;


    while (buffer[y] != EOF) {
        int decimal = 0;
        int power = INSTRUCTION_LENGTH - 1;
        for (int i = 0; i < INSTRUCTION_LENGTH; ++i) {
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

