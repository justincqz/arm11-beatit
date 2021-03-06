/*cpsr checking*/
#include"arm11struct.h"
#include<stdint.h>
#include"arm11def.h"
#include"emuexecute.h"
#include<stdio.h>
#include"operations.h"

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
  reg[ins->rd] = reg[ins->rm]*reg[ins->rs];
  reg[ins->rd] += ins->a? reg[ins->rs]: 0;
  return SUCCESS;
}

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
  if (location >= mem + MEMORY_SIZE) {
    switch (mem_add) {
      case GPIO0_9:
        reg[ins->rd] = mem_add;
        printf("One GPIO pin from 0 to 9 has been accessed\n");
        break;
      case GPIO10_19:
        reg[ins->rd] = mem_add;
        printf("One GPIO pin from 10 to 19 has been accessed\n");
        break;
      case GPIO20_29:
        reg[ins->rd] = mem_add;
        printf("One GPIO pin from 20 to 29 has been accessed\n");
        break;
      case PIN_ON:
        printf("PIN ON\n");
        break;
      case PIN_OFF:
        printf("PIN OFF\n");
        break;
      default:
        printf("Error: Out of bounds memory access at address 0x%08x\n", mem_add);
        return FAILURE;
    }
    return SUCCESS;
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

Error execute_branch(State_t *state){
  int32_t *reg = state->storage->reg;
  Instruction_t *ins = state->decoded_ins;

  uint32_t offset = ins->address;
  if ( offset & BRANCH_SIGNED_MASK) { /*1 << 23*/
      offset |= BRANCH_EXTENSION;/*((1 << 9) - 1) << 23*/
  }
  offset <<= 2;
  reg[PC_REG] += (int32_t) offset;
  state->isFetched=0;
  state->isDecoded=0;
  ins->executable=0;
  return SUCCESS;
}

Error execute_termination(State_t *state) {
  state->isTerminated = 1;
  return SUCCESS;
}

uint32_t calculate_imm(Instruction_t *ins, int32_t *reg) {
  if(ins->i) {
    return shift ( (ins->imm), ins->rotate << 1, ROR, reg + CPSR_REG,ins->s);
  }
  uint32_t gap = ins->o? (reg[ins->rs]&0xf): ins->shift_constant;
  return shift(reg[ins->rm], gap, ins->shift_type, reg + CPSR_REG,ins->s);
}
