#ifndef ARM11STRUCT_H
#define ARM11STRUCT_H
#include "arm11def.h"
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef struct Storage {
    int32_t *reg;
    int32_t *mem;
} Storage_t;


typedef struct Instruction {
    uint32_t executable;
    Instruction_Type instruction_type;

    uint32_t cond; /*nzcv*/
    uint32_t n;
    uint32_t z;
    uint32_t c;
    uint32_t v;
    uint32_t i; /*Immediate Operand*/
    FLAG s; /*Set Condition codes*/
    Operation_Type opcode;
    uint32_t rn; /*First operand register*/
    uint32_t rd; /*Destination operand register*/
    uint32_t rotate;
    uint32_t imm;
    Shift_Type shift_type;
    uint32_t shift_constant; /*interger in spec*/
    uint32_t rm;
    uint32_t rs; /*shift register*/
    uint32_t a; /*accumulate*/
    uint32_t p; /*Pre/Post indexing bit*/
    uint32_t u; /*Up bit*/
    uint32_t l; /*load/store bit*/
    uint32_t address; /*used in branch*/
    uint32_t o; /*TODO: decode it*/
    char* op1;
    char* op2;
    char* op3;
} Instruction_t;



typedef struct State {
  Storage_t *storage;
	size_t instructions_size;

	uint32_t fetched_code;
	Instruction_t *decoded_ins;

	uint32_t isFetched;
	uint32_t isDecoded;
  uint32_t isTerminated;
} State_t;



typedef struct Symbol {
    char* name;
    uint8_t lineNum;
} Sym_t;


State_t *new_state();
void delete_state(State_t *state);

Instruction_t *new_instruction();
void delete_instruction(Instruction_t *ins);

Sym_t *new_symbol();
void delete_symbol(Sym_t *sym);


#endif
