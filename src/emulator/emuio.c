#include<stdio.h>
#include<stdint.h>
#include<assert.h>
#include<stdlib.h>
#include"emustruct.h"
#include"emuio.h"
#define INSTRUCTION_LENGTH 32
#define INSTRUCTION_BYTE 4
#define BYTES_FOR_INT 4
#define ZERO '0'
#define ONE '1'
uint32_t *convert(char *buffer, size_t *size);

void emuread(char *fileName, State_t *state) {

    FILE *file;
	size_t lengthFile;
    file = fopen(fileName, "rb");
    assert(file != NULL);

    if (!file) {
        fprintf(stderr, "Unable to open file %s", fileName);
        return;
    }

    fseek(file, 0, SEEK_END);
    lengthFile = (unsigned long) ftell(file);
    fseek(file, 0, SEEK_SET);
	
	//printf("length of file is %ld\n", lengthFile);
	size_t size = lengthFile/INSTRUCTION_BYTE;
	assert(size <= MEMORY_SIZE);
	state->instructions_size = size;

	for (int i = 0; i < size; i++) {
		fread(state->storage->mem+i, sizeof(uint32_t),1,file);	
	}

	fclose(file);
    return;
}

void emuwrite(Storage_t *storage) {

    assert(storage != NULL);

    printf("Registers:\n");

    for (int i = 0; i < 13; i++) {
        printf("$%-3d", i);
        printf(": %10d (0x%08x)\n", storage->reg[i], storage->reg[i]);
    }

    printf("PC  ");
    printf(": %10d (0x%08x)\n", storage->reg[PC_REG], storage->reg[PC_REG]);

    printf("CPSR");
    printf(": %10d (0x%08x)\n", storage->reg[CPSR_REG], storage->reg[CPSR_REG]);

    printf("Non-zero memory: \n");
    for (int i = 0; i < MEMORY_SIZE; i ++) {
        if ( *(storage->mem + i) != 0) {
			uint32_t value = *(storage->mem+i);
			uint32_t t = 4;

            printf("0x%08x: 0x", i*4);;
			while(t--) {
				printf("%02x", value & ((1<<8) - 1));
				value >>= 8;
			}
			printf("\n");
        }
    }
}

