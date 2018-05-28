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
void fetch(State_t *state);
void decode(State_t *state);
void execute(State_t *state);

#endif

