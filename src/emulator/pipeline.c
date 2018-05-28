#include<stdint.h>
#include"emustruct.h"
#include<assert.h>
#include"emudecode.h"
#include"emuexecute.h"
#include"pipeline.h"
#include<string.h>

#define PC_GAP 4
#define PC_AHEAD 8

void fetch(State_t *state) {
	register int32_t *pc_reg = state->storage->reg + PC_REG;
	register uint32_t *mem = (uint32_t *) state->storage->mem;

	
    register uint32_t pos = *pc_reg/PC_GAP;/*4*/
    state->fetched_code = *(mem + pos);
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
	register uint32_t code = state->fetched_code;
	*ins_type = get_ins_type(code);
    
	/*allocate the different instruction type to different implementation*/
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
        case TERMINATION: 
			break;
    }
	
    state -> isDecoded = 1;
    return;
}

uint32_t check_condition(register uint32_t cond, register uint32_t nzcv) {    
    register uint32_t v = nzcv & 1;
    nzcv >>= 1;
    nzcv >>= 1;
    register uint32_t z = nzcv & 1;
    nzcv >>= 1;
    register uint32_t n = nzcv & 1;

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
    register int32_t *cpsr = state->storage->reg + CPSR_REG;
    register uint32_t nzcv
        = extract_code((uint32_t) *cpsr, CPSR_BIT_LOWER, CPSR_BIT_UPPER);
    if (check_condition(ins->cond, nzcv) ) {
		ins->executable = 1;
    } else {
   	 	ins->executable = 0;
		return SUCCESS;
	}
	

	/*alloctate executaion to different types*/
 	switch (ins->instruction_type) {
        case DATA_PROCESSING: 
            return execute_data_processing(state);
        
        case MULTIPLY: 
            return execute_multiply(state); 
        
        case SINGLE_DATA_TRANSFER: 
            return execute_single_data_transfer(state); 
        
        case BRANCH: 
            return execute_branch(state);
        
        case TERMINATION:
            state->isTerminated = 1;
            return SUCCESS;
    }
    return SUCCESS;
}

Error pipeline_circle(State_t *state) { 
    execute(state);
    
  	if(state->isTerminated) {
        return SUCCESS;
   	}
    
    decode(state);   
    fetch(state);
	register int32_t *pc_reg = state->storage->reg + PC_REG;
    *pc_reg += PC_GAP;
    return SUCCESS;
}

uint32_t get_ins_type(register uint32_t code) {
    register uint32_t res = extract_code(code,TYPE_BIT_LOWER,TYPE_BIT_UPPER);
    if (res == FURTHER_TYPE_CHECKING) {
        
		register uint32_t first_value 
            = extract_code(code, MULTIPLY_FIRST_BIT_LOWER, MULTIPLY_FIRST_BIT_UPPER);
        register uint32_t second_value
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



