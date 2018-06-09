#include "arm11def.h"
#include "arm11struct.h"
#include "strutils.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "assencode.h"

// Parses the register int
int parseRegInt(char* reg) {
  reg = cleanDelim(reg, " r,\n");
  if(reg == NULL){
    return 0;
  }
  int res = strtol(reg, NULL, 10);
  return res;
}

// Shift immediate is 5 bits, immediate addressing is 8 bits
// To declare shift mode, set type to 1
// To declare signed mode, set type to 2
Instruction_t* parseImmediate(char* inp, Instruction_t* ins) {
  uint32_t res = 0;

  // Strip '#'
  char* temp = inp;
  if (*temp == '#') {
    temp++;
  }

  // If negative immediate value
  if (temp[0] == '-') {
    if (ins->instruction_type != SINGLE_DATA_TRANSFER) {
      perror("Negative immediate value set for non data transfer\n");
      exit(EXIT_FAILURE);
    }
    ins->u = 0;
    temp++;
  }

  // Check for hex
  if(strlen(inp) > 2 && temp[1] == 'x') {
    temp += 2;
    res = strtol(temp, NULL, 16);
  } else {
    res = strtol(temp, NULL, 10);
  }

  // Check for overflows depending on whether it is a shift (5 bits) or
  // Deal with rotations if it is address mode (8 bits)
  if ((ins->i && ins->instruction_type == SINGLE_DATA_TRANSFER) ||
    (!ins->i && ins->instruction_type == DATA_PROCESSING)) {
    
    ins->o = 0;
    if (res > 31) {
      perror("Cannot represent register shift within 5 bits\n");
      exit(EXIT_FAILURE);
    }
    ins->shift_constant = res;
  }

  // Calculate and deal with rotations
  else if (ins->instruction_type == DATA_PROCESSING && res > UNSIGNED_8BIT_MAX) {
    int rotate = countRRot(res);

    // If rotate doesn't exist, throw error
    if (rotate == -1) {
      perror("Cannot represent mov command in 8 bits\n");
      exit(EXIT_FAILURE);
    }

     // If rotate is odd,
     if (rotate % 2 == 1) {
       rotate--;
     }

     // Rotate res by the new rotate value
     res = (res >> rotate % INSTRUCTION_LENGTH) |
     (res << (INSTRUCTION_LENGTH - rotate) % INSTRUCTION_LENGTH);

     // Since we right shift by rotate, we must store the left shift value
     ins->rotate = (32 - rotate) / 2;
     ins->imm = res;

  } else {
    ins->imm = res;
  }

  return ins;
}

Shift_Type parseShiftType(char* op) {
  switch(op[0]) {
    case 'l':
      if(op[2] == 'l') {
        return LSL;
      } else {
        return LSR;
      }

    case 'a':
      return ASR;

    case 'r':
      return ROR;

    default:
      perror("Failed to parse shift type.");
      exit(EXIT_FAILURE);
  }
}

// Handles the second operand of data processing instructions
// Flag is immediate value if sdt is 0, else it is a negative sign flag
Instruction_t* parseOperand2(Instruction_t* ins) {
  char* op1 = ins->op1;
  char* op2 = ins->op2;
  char* op3 = ins->op3;

  // If operand2 is a register
  if (*op1 != '#') {

    ins->i = (ins->instruction_type == SINGLE_DATA_TRANSFER);

    // If operand2 is a shifter register
    if (op2 != NULL) {

      // If shift by immediate value
      if (op3[0] == '#') {
        ins->o = 0;
        ins = parseImmediate(op3, ins);

      } else {
        // Shift by register value
        ins->o = 1;
        ins->rs = parseRegInt(op3);
      }

      // Add shift type
      ins->shift_type = parseShiftType(op2);
    }

    printf("U: %u\n", ins->u);
    ins->rm = parseRegInt(op1);
    return ins;
  }

  // If operand2 is an immediate address
  if (ins->instruction_type == DATA_PROCESSING) {
    ins->i = 1;
  } else {
    ins->i = 0;
  }
  printf("U: %u\n", ins->u);
  ins = parseImmediate(op1, ins);
  return ins;
}

uint32_t parseDataCmd(char* inp, Operation_Type opType) {
  Instruction_t *ins = new_instruction();
  strtok(inp, " ,\n");
  ins->opcode = opType;
  ins->instruction_type = DATA_PROCESSING;
  ins->cond = AL;

  if (opType == MOV) {
    ins->rd = parseRegInt(strtok(NULL, " r,\n"));
    ins->rn = 0;
    ins->s = UNSET;

  } else if ((opType == TST) || (opType == TEQ) || (opType == CMP)) {
    ins->rd = 0;
    ins->rn = parseRegInt(strtok(NULL, " r,\n"));
    ins->s = SET;
  } else {
    ins->rd = parseRegInt(strtok(NULL, " r,\n"));
    ins->rn = parseRegInt(strtok(NULL, " r,\n"));
    ins->s = UNSET;
  }

  // Read operands
  ins->op1 = strtok(NULL, " r,\n");
  ins->op2 = strtok(NULL, " r,\n");
  ins->op3 = strtok(NULL, " r,\n");

  ins = parseOperand2(ins);
  uint32_t res = encode(ins);
  delete_instruction(ins);
  return res;
}

// Will be moved to a new file
uint32_t parseSDTCmd(char* inp, uint32_t* append, int off) {
  Instruction_t* ins = new_instruction();
  ins->instruction_type = SINGLE_DATA_TRANSFER;
  ins->cond = AL;
  ins->l = (*inp == 'l');
  ins->u = 1;

  strtok(inp, " r,\n");
  ins->rd = parseRegInt(strtok(NULL, " r,\n"));

  char* address = cleanDelim(strtok(NULL, ""), " ,\n");

  // If unsigned offset mode
  if (*address == '=') {
    ins->i = 0;
    *address = '#';
    ins = parseImmediate(address, ins);

      // If offset is less than 255 and also an ldr command, return the mov cmd
    if (ins->imm <= UNSIGNED_8BIT_MAX && ins->l == 1) {
      ins->instruction_type = DATA_PROCESSING;
      ins->opcode = MOV;
      ins->rn = 0;
      ins->s = 0;
      ins->i = 1;

      // Or else just redefine to pre-index PC instruction
    } else {
      *append = ins->imm;
      ins->p = 1;
      ins->rn = PC_REG;
      int offset = (off - 2) * 4;

      if (offset < 0) {
        ins->u = 0;
        offset *= -1;
      } else {
        ins->u = 1;
      }

      if (offset > UNSIGNED_12BIT_MAX) {
        delete_instruction(ins);
        perror("Cannot store data, offset too large\n");
        exit(EXIT_FAILURE);
      }
      ins->imm = offset;
    }
  } else {
    // Shifted Register Offset
    // If pre-indexing mode
    if (strlen(strpbrk(address, "]")) == 1) {
      ins->p = 1;
    }
    address = cleanDelim(address, " r[]");
    ins->rn = parseRegInt(strtok(address, " ]r,\n"));
    ins->op1 = strtok(NULL, " ]r,\n");
    ins->op2 = strtok(NULL, " ]r,\n");
    ins->op3 = strtok(NULL, " ]r,\n");

    // If there is no operands, offset = 0;
    if (ins->op1 != NULL) {
      if (*(ins->op1) == '-') {
        ins->op1++;
        ins->u = 0;
      }
      ins = parseOperand2(ins);
    } else {
      ins->i = 0;
      ins->imm = 0;
      ins->rotate = 0;
    }
  }

  uint32_t res = encode(ins);
  delete_instruction(ins);
  return res;
}

//Will be moved to a new file
uint32_t multCmd(char* inp) {
  Instruction_t* ins = new_instruction();
  strtok(inp, " r,\n");

  ins->instruction_type = MULTIPLY;
  ins->cond = AL;
  ins->a = 0;
  ins->s = 0;
  ins->rd = parseRegInt(strtok(NULL, " r,\n"));
  ins->rm = parseRegInt(strtok(NULL, " r,\n"));
  ins->rs = parseRegInt(strtok(NULL, " r,\n"));

  char* rn = strtok(NULL, " r,\n");
  if (rn != NULL) {
    ins->a = 1;
  }
  ins->rn = parseRegInt(rn);

  uint32_t res = encode(ins);
  delete_instruction(ins);
  return res;
}

uint32_t lslCmd(char* inp) {
  Instruction_t* ins = new_instruction();
  strtok(inp, " ,\n");
  char* rn = strtok(NULL, " r,\n");

  ins->instruction_type = DATA_PROCESSING;
  ins->opcode = MOV;
  ins->rd = parseRegInt(rn);
  ins->s = 0;
  ins->cond = AL;
  ins->op1 = rn;
  ins->op2 = "lsl";
  ins->op3 = strtok(NULL, " r,\n");
  ins = parseOperand2(ins);

  uint32_t res = encode(ins);
  delete_instruction(ins);
  return res;
}

// TODO: Assumes a symTable size of 32 for now.
uint8_t lookUpLineNum(Sym_t** symTable, char* name) {
  for (int i = 0; i < 32; i++) {
    if (strcmp(symTable[i]->name, name) == 0) {
      return symTable[i]->lineNum;
    }
  }
  return -1;
}

uint32_t branchCmd(char* inp, Sym_t** symT, Condition_Type cond, int currLine) {
  Instruction_t* ins = new_instruction();
  strtok(inp, " ");
  char* label = stripStr(strtok(NULL, " "), '\n');
  int32_t offset = lookUpLineNum(symT, label) - (currLine + 2);
  uint32_t addr = offset & (0x00FFFFFF);

  ins->instruction_type = BRANCH;
  ins->cond = cond;
  ins->address = addr;

  uint32_t res = encode(ins);
  delete_instruction(ins);
  free(label);
  return res;
}
