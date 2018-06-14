#ifndef PARSEDATA_H
#define PARSEDATA_H

#include "arm11def.h"
#include "arm11struct.h"
#include <stdint.h>

int parseRegInt(char* reg);
Instruction_t* parseImmediate(char* inp, Instruction_t* ins);
Shift_Type parseShiftType(char* op);
Instruction_t* parseOperand2(Instruction_t* ins);
uint32_t parseDataCmd(char* inp, Operation_Type opType);
uint32_t parseSDTCmd(char* inp, uint32_t* append, int off);
uint32_t multCmd(char* inp);
uint32_t lslCmd(char* inp);
uint8_t lookUpLineNum(Sym_t* symTable, char* name, size_t sizeOfSymT);
uint32_t branchCmd(char* inp, Sym_t* symT, Condition_Type cond, int currLine, size_t sizeOfSymT);

#endif
