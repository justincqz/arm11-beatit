#ifndef EMUSTRUCT_H
#define EMUSTRUCT_H


#include "emudef.h"

typedef struct Storage {
	int reg[NUMBERS_OF_REG];
	int mem[MEMORY_SIZE];
	/*
    Storage() {
		memset(reg,0,sizeof(reg));
		memset(mem,0,sizeof(mem))
	}
    */
} Storage_t;

typedef struct State {
	Storage_t *storage;
    int isTerminated;
	int *instructions;
	int lines;
	struct Instruction *curr_instruction;
} State_t;

typedef struct Condition_Field {
	int n,z,c,v;
} Condition_Field_t;

typedef struct Instruction {
	int isFetched;
    int	isDecoded;
	Instruction_Type instruction_type;
	int binary_code;

	Condition_Field_t cond;
	int i; /*Immediate Operand*/
	int s; /*Set Condition codes*/
	int shift_flg;
	int opcode;
	int rn; /*First operand register*/
	int rd; /*Destination operand register*/
	int rotate;
	int imm;
	Shift_Type shift_type;
	int shift_constant; /*interger in spec*/
	int rm;
	int rs; /*shift register*/
	int a; /*accumulate*/
	int p; /*Pre/Post indexing bit*/
	int u; /*Up bit*/
	int l; /*load/store bit*/
	int address; /*used in branch*/

} Instruction_t;

void someFunctions();

#endif
