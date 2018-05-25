#ifndef EMUDECODE_H
#define EMUDECODE_H

#include "emustruct.h"

/*return the decimal number of the binary code of "code" from 
 the position lower_bit to upper_bit
eg: extact_code(12,2,3) should return 3
12 => 1100   get the 2~3 positions.
3  => 11
 */
int extract_code(int code,int lower_bit, int upper_bit);

/*
   1.check if the ins is in data_processing type.
   2.if yes decode the "code" into "ins" according to spec;
 */
void decode_data_processing(int code, Instruction_t *ins);
/*same for followings*/
void decode_multiply(int code, Instruction_t *ins);
void decode_single_data_transfer(int code, Instruction_t *ins);
void decode_branch(int code, Instruction_t *ins);

#endif
