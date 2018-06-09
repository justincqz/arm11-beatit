#include "emuio.h"

#define maxn 10000

int main(void) {
	char *fileName = "add01.bin";

	State_t *state = new_state();
	emuread(fileName, state);

	size_t size = state->instructions_size;
	printf("size = %ld\n", size);
	
	uint32_t *instructions = (uint32_t *) (state->storage->mem);
	for (int i = 0; i < size; i++) {
		printf("%u\n", *(instructions + i)); 
	}
	free(state);
}

