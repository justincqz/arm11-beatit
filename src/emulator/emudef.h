#ifndef EMUDEF_H
#define EMUDEF_H

#define NUMBERS_OF_REG 20
#define MEMORY_SIZE  65540
#define PC_REG 15
#define CPSR_REG 16
#define TERMINATION_CODE 0
#define PC_INCREMENT_GAP 4

#define CPSR_BIT_LOWER 28
#define CPSR_BIT_UPPER 31

#define INSTRUCTION_LENGTH 32

#define INSTRUCTION_BYTE 4
#define BYTES_FOR_INT 4

#define FURTHER_TYPE_CHECKING 0
#define TYPE_BIT_LOWER 26
#define TYPE_BIT_UPPER 27
#define MULTIPLY_FIRST_VALUE 9
#define MULTIPLY_SECOND_VALUE 0
#define MULTIPLY_FIRST_BIT_LOWER 4	
#define MULTIPLY_FIRST_BIT_UPPER 7
#define MULTIPLY_SECOND_BIT_LOWER 22
#define MULTIPLY_SECOND_BIT_UPPER 25

#define COND_N_BIT 31
#define COND_Z_BIT 30
#define COND_C_BIT 29
#define COND_V_BIT 28
#define OPCODE_LOWER_BIT 21
#define OPCODE_UPPER_BIT 24
#define I_BIT 25
#define P_BIT 24
#define U_BIT 23
#define A_BIT 21
#define S_BIT 20
#define RN_LOWER_BIT 16
#define RN_UPPER_BIT 19
#define RN_MULTIPLY_LOWER_BIT 12
#define RN_MULTIPLY_UPPER_BIT 15
#define RD_MULTIPLY_LOWER_BIT 16
#define RD_MULTIPLY_UPPER_BIT 19
#define RD_LOWER_BIT 12
#define RD_UPPER_BIT 15
#define RS_LOWER_BIT 8
#define RS_UPPER_BIT 11
#define SC_LOWER_BIT 7
#define SC_UPPER_BIT 11
#define ST_LOWER_BIT 5
#define ST_UPPER_BIT 6
#define ROTATE_LOWER_BIT 8
#define ROTATE_UPPER_BIT 11
#define RM_LOWER_BIT 0
#define RM_UPPER_BIT 3
#define BIT_ZERO 0
#define BIT_FOUR 4
#define ADDRESS_UPPER_BIT 23
#define IMMEDIATE_UPPER_BIT 7

#define C_BIT 29
#define WORD_BITS 32
#define PC_GAP 4
#define PC_AHEAD 8

#define SHIFT_GAP_NZCV_CPSR 28
#define BRANCH_SIGNED_MASK 1<<23
#define BRANCH_EXTENSION ((1 << 9) - 1) << 23

#define Z_BIT 30
#define N_BIT 31
#define V_BIT 28
#define UNSIGNED_INT_MAX ~0

typedef enum {
    FAILURE = -1,
    SUCCESS = 0
} Error;

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
