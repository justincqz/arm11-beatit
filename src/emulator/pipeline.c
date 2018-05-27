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
	assert(state);
	int32_t *pc_reg = state->storage->reg + PC_REG;
	uint32_t *instructions = (uint32_t *) state->storage->mem;
	uint32_t size = state->instructions_size;
	assert(pc_reg);
    assert(instructions);
	
	/* be sure PC < size + 2, fetched two steps a head execute*/
    uint32_t pos = *pc_reg/PC_GAP;/*4*/
	assert(pos < size + PC_AHEAD/PC_GAP);/*8*/


    /*fetch the code from instructions*/
    state->fetched_code = *(instructions + pos);
    
	/*marking*/
	state->isFetched = 1;
	return;
}

void decode(State_t *state) {
	if(!state->isFetched) {
		return;
	}
	
	Instruction_t *ins = state->decoded_ins;
	assert(ins);
	memset(ins,0,sizeof(*ins));

    Instruction_Type *ins_type = &(ins->instruction_type);
    assert(ins_type);

	/*try to figure out the ins type*/
	uint32_t code = state->fetched_code;
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
	
	/*mark ins as decoded*/
    state -> isDecoded = 1;
    return;
}

void execute(State_t *state) {
	if (!state->isDecoded) {
		return;
	}

    Instruction_t *ins = state->decoded_ins;
	assert(ins);
    
    int32_t *cpsr = state->storage->reg + CPSR_REG;
    assert(cpsr);
    uint32_t cpsr_value 
        = extract_code((uint32_t) *cpsr, CPSR_BIT_LOWER, CPSR_BIT_UPPER);
	
	/*if ins condition not equal to cpsr condition, return
	 otherwise, mark ins executable*/
    if (ins->cond == cpsr_value || ins->cond == AL) {
		ins->executable = 1;
    } else {
   	 	ins->executable = 0;
		return;
	}
	

	/*alloctate executaion to different types*/
    Storage_t *storage = state->storage;
	switch (ins->instruction_type) {
        case DATA_PROCESSING: 
            execute_data_processing(ins, storage);
            break;
        case MULTIPLY: 
            execute_multiply(ins, storage); 
            break;
        case SINGLE_DATA_TRANSFER: 
            execute_single_data_transfer(ins, storage); 
            break;
        case BRANCH: 
            execute_branch(ins, storage);
            break;
        case TERMINATION: 
            break;
    }

    return;
}

int32_t pipeline_circle(State_t *state) {
    assert(state);    
    /*Execute decoded*/
    execute(state);
	
  	
	/*BRANCH: clear pipeline stage
      TERMINATTION: mark terminate and get out*/
	Instruction_t *ins = state->decoded_ins;
	if (ins->instruction_type == BRANCH && ins->executable) {
           	memset(ins,0,sizeof(*ins));
           	state->fetched_code = 0;
			state->isFetched = 0;
			state->isDecoded = 0;
			return 0;
	} 
	
	if(ins->instruction_type == TERMINATION) {
    	state->isTerminated = 1;
        return 1;
   	}
    
    /*Decode fetched*/
    decode(state);   
    
    /*Fetch PC*/
    fetch(state);
    
    /*PC = PC + 1*/
	int32_t *pc_reg = state->storage->reg + PC_REG;
    *pc_reg += PC_GAP;
    return 0;
}

uint32_t get_ins_type(uint32_t code) {
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



