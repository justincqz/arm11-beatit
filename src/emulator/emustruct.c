#include<stdint.h>
#include"emustruct.h"
#include<string.h>

State_t *new_state() {

    State_t *state = malloc(sizeof(State_t)); /*free*/
    if(state==NULL) {
		return NULL;
	}
	
	state->decoded_ins = malloc(sizeof(Instruction_t));
	if (state->decoded_ins == NULL) {
		free(state);
		return NULL;
	}

	state->storage = malloc(sizeof(Storage_t));
	if (state->storage == NULL) {
		free(state->decoded_ins);
		free(state);
		return NULL;
	}

	state->storage->reg = calloc(NUMBERS_OF_REG, sizeof(int32_t));
	if(state->storage->reg == NULL) {
		free(state->decoded_ins);
		free(state->storage);
		free(state);
	}
	memset(state->storage->reg, 0, NUMBERS_OF_REG*sizeof(int32_t));

	state->storage->mem = calloc(MEMORY_SIZE, sizeof(int32_t));
	if(state->storage->reg == NULL) {
		free(state->decoded_ins);
		free(state->storage->reg);
		free(state->storage);
		free(state);
	}
	memset(state->storage->mem, 0, MEMORY_SIZE*sizeof(int32_t));		

	state->instructions_size = 0;
	state->fetched_code = 0;
	state->isTerminated = 0;
	state->isDecoded = 0;
	state->isFetched = 0;
    
	return state;
}

void delete_state(State_t *state) {
	if(state != NULL) {
		if(state->decoded_ins != NULL) {
			free(state->decoded_ins);
		}

		if(state->storage != NULL) {
			if(state->storage->reg != NULL) {
				free(state->storage->reg);
			}
			if(state->storage->mem != NULL) {
				free(state->storage->mem);
			}
			free(state->storage);
		}
		free(state);
	}
}


