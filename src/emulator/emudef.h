#ifndef EMUDEF_H
#define EMUDEF_H

#define NUMBERS_OF_REG 20
#define MEMORY_SIZE  65540
#define PC_REG 15
#define CPSR_REG 16
#define TERMINATION_CODE 0
#define PC_INCREMENT_GAP 4


#define FURTHER_TYPE_CHECKING 0
#define TYPE_BIT_LOWER 26
#define TYPE_BIT_UPPER 27
#define MULTIPLY_FIRST_VALUE 9
#define MULTIPLY_SECOND_VALUE 0
#define MULTIPLY_FIRST_BIT_LOWER 4	
#define MULTIPLY_FIRST_BIT_UPPER 7
#define MULTIPLY_SECOND_BIT_LOWER 22
#define MULTIPLY_SECOND_BIT_UPPER 25

typedef enum {
	DATA_PROCESSING = 0,
	MULTIPLY = 3,
	SINGLE_DATA_TRANSFER = 1,
	BRANCH = 2,
	TERMINATION = 4
} Instruction_Type;

typedef enum {
	LSL, /*logic left*/
	LSR, /*logic right*/
	ASR, /*arithmatic right*/
	ROR /*rotate right*/
} Shift_Type;

typedef enum  {
	EQ = 0,
	NE = 1,
	GE = 10,
	LT = 11,
	GT = 12,
	LE = 13,
	AL = 14
} Condition_Type;

typedef enum {
    AND = 0,
    EOR = 1,
    SUB = 2,
    RSB = 3,
    ADD = 4,
    TST = 8,
    TEQ = 9,
    CMP = 10,
    ORR = 12,
    MOV = 13
} Operation_Type;

#endif
