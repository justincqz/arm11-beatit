#ifndef EMUEXECUTE_H
#define EMUEXECUTE_H

#include"emustruct.h"

void execute_data_processing(State_t *state);
void execute_multiply(State_t *state);
void execute_single_data_transfer(State_t *state);
void execute_branch(State_t *state);

#endif
