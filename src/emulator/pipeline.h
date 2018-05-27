#ifndef PIPELINE_H
#define PIPELINE_H

#include "emustruct.h"
#include "emudef.h"
#include "emudecode.h"
#include "emuexecute.h"
#include<stdint.h>
#include<stdlib.h>
#include<assert.h>


int32_t pipeline_circle(State_t *state);
Instruction_t *fetch(int32_t *pc_reg, uint32_t instructions, uint32_t size);
Instruction_t *decode(Instruction_t *ins);
uint32_t execute(Instruction_t *ins);

#endif

