#include<stdint.h>
#include"emustruct.h"
#include<assert.h>
#include"emudecode.h"
#include"emuexecute.h"
#include"pipeline.h"
#include<string.h>
#include"emudef.h"


void (*decode_functions[5]) (State_t *state) = {
	decode_data_processing,
	decode_single_data_transfer,
	decode_branch,
	decode_multiply,
	decode_termination
};

Error (*execute_functions[5]) (State_t *state) = {
	execute_data_processing,
	execute_single_data_transfer,
	execute_branch,
	execute_multiply,
	execute_termination
};



void fetch(State_t *state) {
	int32_t *pc_reg = state->storage->reg + PC_REG;
    state->fetched_code = *((uint32_t *)state->storage->mem + *pc_reg/PC_GAP);
	state->isFetched = 1;
	return;
}

void decode(State_t *state) {
	if(!state->isFetched) {
		return;
	}
	
	Instruction_t *ins = state->decoded_ins;
    Instruction_Type *ins_type = &(ins->instruction_type);

	/*try to figure out the ins type*/
	*ins_type = get_ins_type(state->fetched_code);
    
	/*allocate the different instruction type to different implementationi*/
	decode_functions[*ins_type](state);

    state -> isDecoded = 1;
    return;
}

uint32_t check_condition(uint32_t cond,  uint32_t nzcv) {    
    uint32_t v = nzcv & 1;
    nzcv >>= 1;
    nzcv >>= 1;
    uint32_t z = nzcv & 1;
    nzcv >>= 1;
    uint32_t n = nzcv & 1;

    switch(cond) {
        case EQ:
            return z == 1;
        case NE:
            return z == 0;
        case GE:
            return n == v;
        case LT:
            return n != v;
        case GT:
            return !z && (n == v);
        case LE:
            return z || (n != v);
        default:
            return 1;
    }

}

Error execute(State_t *state) {
	if (!state->isDecoded) {
		return SUCCESS;
	}

    Instruction_t *ins = state->decoded_ins; 
    int32_t *cpsr = state->storage->reg + CPSR_REG;
    uint32_t nzcv
        = extract_code((uint32_t) *cpsr, CPSR_BIT_LOWER, CPSR_BIT_UPPER);
    
	if(ins->instruction_type == TERMINATION) {
		state->isTerminated=1;
		return SUCCESS;
	}

	if (check_condition(ins->cond, nzcv) ) {
		ins->executable = 1;
    } else {
   	 	ins->executable = 0;
		return SUCCESS;
	}
	

	/*alloctate executaion to different types*/
	return execute_functions[ins->instruction_type](state);
}

Error pipeline_circle(State_t *state) {    
  	execute(state);
	if(state->isTerminated) {
        return SUCCESS;
   	}
    decode(state);   
    fetch(state);
	*(state->storage->reg + PC_REG) += PC_GAP;
    return SUCCESS;
}

uint32_t get_ins_type( uint32_t code) {
    uint32_t res = extract_code(code,TYPE_BIT_LOWER,TYPE_BIT_UPPER);
    if (res == FURTHER_TYPE_CHECKING) {
       
		uint32_t first_value 
            = extract_code(code, MULTIPLY_FIRST_BIT_LOWER, MULTIPLY_FIRST_BIT_UPPER);
        uint32_t second_value
            = extract_code(code, MULTIPLY_SECOND_BIT_LOWER, MULTIPLY_SECOND_BIT_UPPER);
        
		if(first_value == MULTIPLY_FIRST_VALUE
            && second_value == MULTIPLY_SECOND_VALUE) {
            res = MULTIPLY;
        }
    }
	if (code == TERMINATION_CODE) {
		res = TERMINATION;
	}
	return res;
}



