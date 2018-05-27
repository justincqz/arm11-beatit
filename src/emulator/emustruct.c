#include<stdint.h>
#include"emustruct.h"
#include<string.h>

Storage_t *new_storage() {
    int32_t *reg = calloc(NUMBERS_OF_REG, sizeof(int32_t)); /*free*/
    int32_t *mem = calloc(MEMORY_SIZE, sizeof(int32_t)); /*free*/
    Storage_t *storage = malloc(sizeof(Storage_t)); /*free*/
    storage->reg = reg;
    storage->mem = mem;
    return storage;
}


State_t *new_state(Storage_t *storage, uint32_t *instructions, size_t size) {
    assert(storage);
    assert(instructions);

    State_t *state = malloc(sizeof(state)); /*free*/
    memset(state, 0, sizeof(*state));  /*is it right?*/
    /*State_t state = {0} ?? */

    state->storage = storage;
    state->instructions = instructions;
    state->instructions_size = size;
    return state;
}


Instruction_t *new_instruction() {
    Instruction_t *ins = malloc(sizeof(Instruction_t));
    return ins;
}

