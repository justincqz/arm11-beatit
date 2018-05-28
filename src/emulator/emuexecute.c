/*cpsr checking*/
#include"emustruct.h"
#include<stdint.h>
#include"emudef.h"
#include"emuexecute.h"
#include"alu.h"
#include"shifter.h"
#include<stdio.h>
#define BRANCH_SIGNED_MASK 1<<23
#define BRANCH_EXTENSION ((1 << 9) - 1) << 23
#define N_BIT 31
#define Z_BIT 30



Error execute_data_processing(State_t *state) {
    int32_t *reg = state->storage->reg;
    Instruction_t *ins = state->decoded_ins;

    alu_execute(ins->opcode, reg[ins->rn], calculate_imm(ins,reg)
			, reg + ins->rd, reg + CPSR_REG, ins->s);
    return SUCCESS;
}



Error execute_multiply(State_t *state) {
    int32_t *reg = state->storage->reg;
    Instruction_t *ins = state->decoded_ins;
    /*start code*/
    reg[ins->rd] = reg[ins->rm]*reg[ins->rs];
    reg[ins->rd] += ins->a? reg[ins->rs]: 0;
    return SUCCESS;
}


/*need to debug*/
Error execute_single_data_transfer(State_t *state){
    int32_t *reg = state->storage->reg;
    int32_t *mem = state->storage->mem;
    Instruction_t *ins = state->decoded_ins;

    if(ins->rm == PC_REG || ins->rd == PC_REG) {
        printf("Invalid Instruction\n");
        return FAILURE;
    }
    
    if (ins->p == 0 && ins->rm == ins->rd) {
        printf("Invalid Instruction\n");
        return FAILURE;
    }

    /*start code*/
    uint32_t offset = ins->imm;
    if (ins->i) {
        uint32_t gap = ins->o? (reg[ins->rs]&0xf): ins->shift_constant;
        offset = shift(reg[ins->rm], gap, ins->shift_type, reg + CPSR_REG, ins->s);
    
    }
    


    uint32_t mem_add = (uint32_t) reg[ins->rn];
    if(ins->p) {
        if(ins->u) {
            mem_add += offset;
        } else {
            mem_add -= offset;
        }
    }
    

     int32_t *location = (int32_t *) ((char *) mem + mem_add);
    /*need to recheck the bound*/ 
    if (location >= mem + MEMORY_SIZE) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", mem_add);
        return FAILURE;
    }

    if(ins -> l) {
        reg[ins->rd] = *(location);
    } else {
        *location = reg[ins->rd];
    }
    
    if(!ins->p) {
        if(ins->u) {
            reg[ins->rn] += offset;
        } else {
            reg[ins->rn] -= offset;
        }
    }
    return SUCCESS;
}
    
   
/*whether or not affect cpsr*/
Error execute_branch(State_t *state){    
    int32_t *reg = state->storage->reg;
    Instruction_t *ins = state->decoded_ins;

    /*start code*/
    int32_t offset = ins->address;
    if ( offset & BRANCH_SIGNED_MASK) { /*1 << 23*/
        offset |= BRANCH_EXTENSION;/*((1 << 9) - 1) << 23*/
    }
    offset <<= 2;
    reg[PC_REG] += offset;
    state->isFetched=0;
    state->isDecoded=0;
    ins->executable=0;
    return SUCCESS;
}

uint32_t calculate_imm(Instruction_t *ins, int32_t *reg) {
    if(ins->i) {
        return shift ( (ins->imm), ins->rotate << 1, ROR, reg + CPSR_REG,ins->s); /*need to check*/
    }
     uint32_t gap = ins->o? (reg[ins->rs]&0xf): ins->shift_constant;
    return shift(reg[ins->rm], gap, ins->shift_type, reg + CPSR_REG,ins->s);
}


