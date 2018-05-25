#ifndef PIPELINE_H
#define PIPELINE_H

#include "emustruct.h"
State_t *newState(Storage_t *storage, int* instructions, int lines);
void fetch(State_t *state);
void decode(State_t *state);
void execute(State_t *state);

#endif

