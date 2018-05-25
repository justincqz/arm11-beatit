#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include"emudef.h"
#include"emudecode.h"
#include"emuexecute.h"

/*TODO*/
State_t *newState(Storage_t *storage, int *instructions, int lines) {
    instructions = malloc(sizeof(int));
    return malloc(sizeof(State_t));
}

void fetch(State_t *state) {
	int *pc_register = (state->storage->reg) + PC_REG;
	assert( (*pc_register) < PC_INCREMENT_GAP* (state->lines) );
	
	/*refresh the instruction*/
	Instruction_t *ins = state-> curr_instruction;
	memset(ins, 0, sizeof(int));
	
	/*fetch the code from instructions*/
	ins -> binary_code 
		= *(state -> instructions + *pc_register/PC_INCREMENT_GAP);

	/*pc = pc + 1*/
	*pc_register += PC_INCREMENT_GAP;
	
	ins -> isFetched = 1;
	return;
}

void decode(State_t *state) {
	assert(state->curr_instruction->isFetched);
	Instruction_t *ins = state->curr_instruction;
	int code = ins -> binary_code;
	Instruction_Type *ins_type = &(ins->instruction_type);
	if (code == TERMINATION_CODE) {
		*ins_type = TERMINATION;
		return;
	}

	*ins_type = extract_code(code,TYPE_BIT_LOWER,TYPE_BIT_UPPER);
	if (*ins_type == FURTHER_TYPE_CHECKING) {
		int first_value 
			= extract_code(code, MULTIPLY_FIRST_BIT_LOWER, MULTIPLY_FIRST_BIT_UPPER);
		int second_value
			= extract_code(code, MULTIPLY_SECOND_BIT_LOWER, MULTIPLY_SECOND_BIT_UPPER);
		if(first_value == MULTIPLY_FIRST_VALUE
		   && second_value == MULTIPLY_SECOND_VALUE) {
			*ins_type = MULTIPLY;
		}
	}
	
	switch (*ins_type) {
		case DATA_PROCESSING: 
            decode_data_processing(code,ins);
            break;
		case MULTIPLY: 
            decode_multiply(code, ins); 
            break;
		case SINGLE_DATA_TRANSFER: 
            decode_single_data_transfer(code, ins);
            break;
		case BRANCH: 
            decode_branch(code, ins);
            break;
        default: break;
	}

	ins -> isDecoded = 1;
	return;
}

void execute(State_t *state) {
	assert(state->curr_instruction->isDecoded);
	switch (state->curr_instruction->instruction_type) {
		case DATA_PROCESSING: 
            execute_data_processing(state);
            break;
		case MULTIPLY: 
            execute_multiply(state); 
            break;
		case SINGLE_DATA_TRANSFER: 
            execute_single_data_transfer(state); 
            break;
		case BRANCH: 
            execute_branch(state);
            break;
	    default: break;
    }
	return;
}
