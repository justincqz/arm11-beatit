#ifndef PIPELINE_H
#define PIPELINE_H

#include "emustruct.h"
#include "emudef.h"
#include "emudecode.h"
#include "emuexecute.h"
#include<stdint.h>
#include<stdlib.h>
#include<assert.h>

uint32_t get_ins_type(uint32_t code);
int32_t pipeline_circle(State_t *state);
Instruction_t *fetch(int32_t *pc_reg, uint32_t *instructions, uint32_t size);
Instruction_t *decode(Instruction_t *ins);
Instruction_t *execute(Instruction_t *ins, Storage_t *storage);

#endif

