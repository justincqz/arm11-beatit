#ifndef EMUEXECUTE_H
#define EMUEXECUTE_H
#include"emudef.h"
#include"emustruct.h"
#include<assert.h>
#include"operations.h"



/*helper functions*/
uint32_t calculate_imm(Instruction_t *ins, int32_t *reg);

Error execute_data_processing(State_t *state);
Error execute_multiply(State_t *state);
Error execute_single_data_transfer(State_t *state);
Error execute_branch(State_t *state);

#endif

