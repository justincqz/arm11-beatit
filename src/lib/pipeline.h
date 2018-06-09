#ifndef PIPELINE_H
#define PIPELINE_H

#include "arm11struct.h"
#include "emudecode.h"
#include "emuexecute.h"
#include<stdint.h>
#include<stdlib.h>
#include<assert.h>

uint32_t check_condition(uint32_t cond, uint32_t nzcv);
uint32_t get_ins_type(uint32_t code);
Error pipeline_circle(State_t *state);
void fetch(State_t *state);
void decode(State_t *state);
Error execute(State_t *state);

#endif

