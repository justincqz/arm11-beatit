#include<stdint.h>
#include"emustruct.h"
#include<assert.h>
#include"emudecode.h"
#include"emuexecute.h"
#include"pipeline.h"
#define PC_GAP 4
#define PC_AHEAD 8

Instruction_t *fetch(int32_t *pc_reg, uint32_t *instructions,uint32_t size) {
    assert(pc_reg);
    assert(instructions);
	
	/* be sure PC < size + 2, fetched two steps a head execute*/
    uint32_t pos = *pc_reg/PC_GAP;/*4*/
    assert(pos < size + PC_AHEAD/PC_GAP);/*8*/
    
    /*Open an new instruction, all elements set to 0*/
    Instruction_t *ins = new_instruction();
    assert(ins);

    /*fetch the code from instructions*/
    ins -> binary_code = *(instructions + pos);

	/*marked as fetched*/
    ins -> isFetched = 1;
    return ins;
}

Instruction_t *decode(Instruction_t *ins) {
    if(ins == NULL) {
        return NULL;
    }
    assert(ins->isFetched);
	
    uint32_t code = ins -> binary_code;
    Instruction_Type *ins_type = &(ins->instruction_type);
    assert(ins_type);

  
	/*try to figure out the ins type*/
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
    ins -> isDecoded = 1;
    return ins;
}

Instruction_t *execute(Instruction_t *ins, Storage_t *storage) {
    if(ins == NULL) {
        return NULL;
    }
    
    int32_t *cpsr = storage->reg + CPSR_REG;
    assert(cpsr);
    uint32_t cpsr_value 
        = extract_code((uint32_t) *cpsr, CPSR_BIT_LOWER, CPSR_BIT_UPPER);
	
	/*if ins condition not equal to cpsr condition, return
	 otherwise, mark ins executable*/
    if (ins->cond != cpsr_value) {
        return 0;
    }
    ins->executable = 1;
	
	/*alloctate executaion to different types*/
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
    ins -> isExecuted = 1;
    return ins;
}

int32_t pipeline_circle(State_t *state) {
    assert(state);
    Storage_t *storage = state->storage;
    assert(storage);
    int32_t *pc_reg = (storage->reg) + PC_REG;
    assert(pc_reg);

    /*Execute decoded*/
    Instruction_t *ins = execute(state->decoded_ins, state->storage);
    assert(ins);
    
    /*BRANCH: clear pipeline stage
      TERMINATTION: mark terminate and get out*/
    switch(ins->instruction_type) {
        case BRANCH:
            state->decoded_ins = NULL;
            state->fetched_ins = NULL;
            break;
        case TERMINATION:
            state->isTerminated = 1;
            return 1;
        default:
            break;
    }
    
    /*Decode fetched*/
    state->decoded_ins = decode(state->fetched_ins);   
    
    /*Fetch PC*/
    state->fetched_ins = fetch(pc_reg, state->instructions, state->instructions_size);
    
    /*PC = PC + 1*/
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



