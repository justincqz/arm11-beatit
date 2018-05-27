/*cpsr checking*/
#include"emustruct.h"
#include<stdint.h>
#include"emudef.h"
#include"emuexecute.h"
#include"alu.h"
#include"shifter.h"

#define BRANCH_SIGNED_MASK 1<<23
#define BRANCH_EXTENSION ((1 << 9) - 1) << 23
#define N_BIT 31
#define Z_BIT 30

void set_bit(int32_t *cpsr, uint32_t bit, uint32_t b) {
    return; /*TODO*/
}

void execute_data_processing(Instruction_t *ins,Storage_t *storage) {
    assert(ins);
    assert(storage);
    assert(ins->instruction_type == DATA_PROCESSING);
    assert(ins->executable);
    
    int32_t *reg = storage->reg;
    assert(reg);
    uint32_t arg1 = reg[ins->rn];
    uint32_t arg2 = calculate_imm(ins, reg);
    alu_execute(ins->opcode, arg1, arg2, reg + ins->rd, reg + CPSR_REG, ins->s);
}

/*whether or not affect cpsr*/
void execute_multiply(Instruction_t *ins,Storage_t *storage) {
    assert(ins);
    assert(storage);
    assert(ins->instruction_type == MULTIPLY);

    int32_t *reg = storage-> reg;
    assert(reg);
    assert(ins->executable);
    reg[ins->rd] = reg[ins->rm]*reg[ins->rs];
    reg[ins->rd] += ins->a? reg[ins->rs]: 0;
    set_bit(&reg[CPSR_REG], N_BIT, (uint32_t) reg[ins->rd] >> 31);
    set_bit(&reg[CPSR_REG], Z_BIT, (uint32_t) reg[ins->rd] == 0);
}


/*whether or not affect cpsr*/
void execute_single_data_transfer(Instruction_t *ins,Storage_t *storage){
    assert(ins);
    assert(storage);
    assert(ins->instruction_type == SINGLE_DATA_TRANSFER);
    int32_t *reg = storage->reg;
    int32_t *mem = storage->mem;
    assert(reg);
    assert(mem);
    assert(ins->executable);

    uint32_t imm = calculate_imm(ins, reg);
    reg[ins->rn] += ins->p? ((ins->u<<1) - 1)*imm: 0;
    if (ins->l) {
        reg[ins->rd] = mem[reg[ins->rn]];
    } else {
        mem[reg[ins->rn]] = reg[ins->rd];
    }
    reg[ins->rn] += ins->p? 0: ((ins->u<<1)-1)*imm;
}

/*whether or not affect cpsr*/
void execute_branch(Instruction_t *ins,Storage_t *storage){
    assert(ins);
    assert(storage);
    assert(ins->instruction_type == BRANCH);
    int32_t *reg = storage->reg;
    int32_t *mem = storage->mem;
    assert(reg);
    assert(mem);
    assert(ins->executable);

    int32_t offset = ins->address;
    if ( offset & BRANCH_SIGNED_MASK) { /*1 << 23*/
        offset |= BRANCH_EXTENSION;/*((1 << 9) - 1) << 23*/
    }
    reg[PC_REG] += offset; 
}

uint32_t calculate_imm(Instruction_t *ins, int32_t *reg) {
    assert(ins);
    assert(reg);

    if(ins->i) {
        return (ins->imm) << (ins->rotate << 1); /*need to check*/
    }
    uint32_t gap = ins->o? (reg[ins->rs]&0xf): ins->shift_constant;
    return shift(reg[ins->rm], gap, ins->shift_type, reg + CPSR_REG);
}


