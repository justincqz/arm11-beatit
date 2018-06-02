#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "strutils.h"
#include "assdef.h"

FILE* openFile(char *path);
struct sym{
    char* name;
    uint8_t lineNum;
};

uint32_t* firstPass(struct sym* symTable, FILE* fp);
uint32_t* secondPass(struct sym* symTable, FILE* fp, uint32_t* noInst);
int writeFile(char* outFile, uint32_t* binTable, uint32_t noInst);

int main(int argc, char **argv) {
    FILE* inpF;
    uint32_t* binT;
    uint32_t numInst;

    if(argc != 3) {
        printf("Usage: %s <inputFile> <outputFile>\n", argv[0]);
        return -1;
    }

    // Assuming that all files will have less than 32 labels, if more then will reallocate
    struct sym* symT = malloc(32 * sizeof(struct sym));

    inpF = openFile(argv[1]);

    // First pass, generates the symbol table and counts the number of instructions and size of symTable
    uint32_t* out;
    out = firstPass(symT, inpF);
    numInst = out[0];
    //uint32_t symSize = out[1];

    // Second pass, generates the array of bitcodes to write to file
    binT = secondPass(symT, inpF, &numInst);
    fclose(inpF);

    // Write binary file
    writeFile(argv[2], binT, numInst);
    return EXIT_SUCCESS;
}

FILE* openFile(char *path){
    FILE* fp = fopen(path, "r");
    if(fp == NULL) {
        perror("Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    return fp;
}

int writeFile(char* outFile, uint32_t* binTable, uint32_t noInst){
  FILE* fp = fopen(outFile, "wb");
  fwrite(binTable, sizeof(uint32_t), noInst, fp);
  fclose(fp);
  return 0;
}

uint32_t* firstPass(struct sym* symTable, FILE* fp){
    uint32_t lineNum = 0;
    int currSym = 0;
    char* currStr = calloc(512, sizeof(char));

    while(fgets(currStr, 512, fp) != NULL){
        //printf("line no: %d\n", lineNum);
        //printf("Current Str: %s\n", currStr);
        currStr = cleanDelim(currStr, " \n");
        if(chrExists(currStr, ':') == 0 && *currStr != '\0'){
            lineNum++;
        } else {
            if(currSym == (sizeof(symTable)/sizeof(struct sym)) - 1) {
                symTable = realloc(symTable, (sizeof(symTable)/sizeof(struct sym)) * 2);
            }

            struct sym s;
            s.name = stripStr(currStr, ':');
            s.lineNum = lineNum;
            symTable[currSym] = s;
            currSym++;
        }
    }

    //if (*currStr != NULL) {
      //free(currStr);
    //}

    uint32_t* out = calloc(2, sizeof(uint32_t));
    out[0] = lineNum;
    out[1] = currSym;
    return out;
}

// Cannot take an r character
int parseRegInt(char* reg) {
  reg = cleanDelim(reg, " r,\n");
  if(reg == NULL){
    return 0;
  }
  int res = strtol(reg, NULL, 10);
  return res;
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

// Shift immediate is 5 bits, immediate addressing is 8 bits
// To declare shift mode, set type to 1
// To declare signed mode, set type to 2
uint32_t* parseImmediate(char* inp, uint8_t type) {

  // Strip '#'
  int sizeInp = strlen(inp);
  char* temp = malloc(sizeInp + 1);
  temp[sizeInp - 1] = '\0';
  temp = strcpy(temp, inp);
  temp++;

  uint32_t rotate = 0;
  uint32_t res = 0;

  // If negative immediate value
  if (temp[0] == '-') {
      if (type == 2) {
        rotate = 1;
      }
      temp++;
      printf("No longer negative\n");
  }


  // Check for hex
  if(sizeInp > 2 && temp[1] == 'x') {
    temp += 2;
    res = strtol(temp, NULL, 16);
  } else {
    res = strtol(temp, NULL, 10);
  }

  // Check for overflows depending on whether it is a shift (5 bits) or
  // Deal with rotations if it is address mode (8 bits)
  if (type == 1 && res > 31) {
    perror("Invalid shift size");
    exit(EXIT_FAILURE);

  } else if (res > 255 && type == 0) {
      rotate = countRRot(res);

      if (rotate == -1) {
        perror("Cannot represent mov command in 8 bits\n");
        exit(EXIT_FAILURE);
      }
      // If rotate is odd,
      if (rotate % 2 == 1) {
        rotate--;
      }
      res = (res >> rotate % 32) | (res << (32 - rotate) % 32);

      // Since we right shift by rotate, we must store the left shift value
      rotate = (32 - rotate) / 2;
  }
  uint32_t* out = calloc(2, sizeof(uint32_t));

  out[0] = res;
  out[1] = rotate;
  return out;
}

// Handles the second operand of data processing instructions
// Flag is immediate value if sdt is 0, else it is a negative sign flag
int parseOperand2(char* op1, char* op2, char* op3, char* flag, int isSdt){
  uint16_t res = 0;

  // If operand2 is a register
  if (op1[0] != '#'){
    uint8_t shift = 0;
    *flag = 0;
    flag[1] = 1;

    // If operand2 is a shifter register
    if (op2 != NULL) {

      // If shift by immediate value
      if (op3[0] == '#'){
        uint32_t* imm = parseImmediate(op3, isSdt * 2);
        shift += *imm << 3;

        // If it is a single data transfer command, pass on the sign bit
        if (isSdt == 1) {
          *flag = imm[1];
        }
      } else {
      // Shift by register value
        shift += 1;
        shift += parseRegInt(op3) << 4;
      }

      // Add shift type
      shift += parseShiftType(op2) << 1;
    }
    if (op1[0] != '-' && isSdt) {
      flag[0] = 1;
    }
    res += shift << 4;
    res += parseRegInt(op1);

    return res;
  }

  // If operand2 is an immediate address
  flag[0] = 1;
  flag[1] = 0;
  if (isSdt == 1 && op1[1] == '-') {
    flag[0] = 0;
  }

  uint32_t* out = parseImmediate(op1, 0);
  res += out[0];
  res += out[1] << 8;

  //if (*out != NULL) {
    //free(out);
  //}

  return res;
}

// Will be moved to a new file
uint32_t dataCmd(char* opcode, char* rd, char* rn, char* op1, char* op2, char* op3, int s){
  uint32_t res = 0xE0000000;

  // Parse and add operand2 to res
  char* isImmediate = calloc(2, sizeof(uint8_t));
  uint16_t operand2 = parseOperand2(op1, op2, op3, isImmediate, 0);
  res += operand2;

  // Add rd to res
  res += parseRegInt(rd) << 12;


  // Add the value of the opcode, flag set bit, and immediate value
  res += isImmediate[0] << 25;
  //free(isImmediate);

  for (int x = 0; x < 4; x++) {
    if (opcode[x] == '1') {
      res += 1 << (24 - x);
    }
  }
  res += s << 20;

  // Add the value of rn to res
  res += parseRegInt(rn) << 16;
  return res;
}



// Will be moved to a new file
uint32_t sdtCmd(char* rd, char* address, int isLDR, uint32_t* append, int off, int imm) {
  int u = 0;
  int i = imm;
  int p = 0;
  uint32_t offset = 0;
  uint32_t res = 0xE4000000;

  // If unsigned offset mode
  if (address[0] == '=') {
      address[0] = '#';
      offset = parseImmediate(address, 3)[0];

      // If offset is less than 255 and also an ldr command, return the mov cmd
      if (offset <= 255 && isLDR == 1) {
          return dataCmd("1101", rd, NULL, address, NULL, NULL, 0);

      // Or else just redefine to pre-index PC instruction
      } else {
        append[0] = offset;
        char* newAddr = calloc(18, sizeof(char));
        sprintf(newAddr, "[r15,#%u]", (off - 2) * 4);

        return sdtCmd(rd, newAddr, 1, append, off, 0);
      }


  }
    // Shifted Register Offset
    // If pre-indexing mode
    if (strlen(strpbrk(address, "]")) == 1) {
      p = 1;
    }

    address = cleanDelim(address, " r[]");

    char* isNeg = calloc(2, sizeof(uint8_t));
    char* rn = strtok(address, " ]r,\n");
    char* op1 = strtok(NULL, " ]r,\n");
    char* op2 = strtok(NULL, " ]r,\n");
    char* op3 = strtok(NULL, " ]r,\n");

    // If there is no operands, offset = 0;
    if (op1 != NULL) {
      offset = parseOperand2(op1, op2, op3, isNeg, 1);
    }

    u = *isNeg;
    if (offset == 0) {
      u = 1;
    }
    // If it is immediate addressing
    i = isNeg[1];


  // Add offset
  res += offset;

  // Add rd, rn
  res += parseRegInt(rn) << 16;
  res += parseRegInt(rd) << 12;

  // Add l, u, i, p
  res += isLDR << 20;
  res += u << 23;
  res += p << 24;
  res += i << 25;

  //free(isNeg);

  return res;
}

uint32_t processData(char* inp, char* opcode, int type){
  char* rd;
  char* rn;
  char* op1;
  char* op2;
  char* op3;
  int s;
  strtok(inp, " ,\n");

  // type 0: compute results; type 1: mov; type 2: set CPSR flags;
  if (type == 0) {
    rd = strtok(NULL, " r,\n");
    rn = strtok(NULL, " r,\n");
    s = 0;

  } else if (type == 1) {
    rd = strtok(NULL, " r,\n");
    rn = "r0";
    s = 0;

  } else if (type == 2) {
    rd = "r0";
    rn = strtok(NULL, " r,\n");
    s = 1;
  }

  // Read operands
  op1 = strtok(NULL, " r,\n");
  op2 = strtok(NULL, " r,\n");
  op3 = strtok(NULL, " r,\n");

  return dataCmd(opcode, rd, rn, op1, op2, op3, s);
}

//Will be moved to a new file
uint32_t multCmd(char* inp){
  strtok(inp, " r,\n");

  // initialise the mul bitcode with known bits
  uint32_t res = 0xE0000090;

  // Add rd to res
  res += parseRegInt(strtok(NULL, " r,\n")) << 16;

  // Add rm to res
  res += parseRegInt(strtok(NULL, " r,\n"));

  // Add rs to res
  res += parseRegInt(strtok(NULL, " r,\n")) << 8;

  // Check rn
  // If rn is not empty, then apply accumulate bit and update rn to res
  char* rn = strtok(NULL, " r,\n");
  if (rn != NULL) {
    res += 1 << 21;
  }
  res += parseRegInt(rn) << 12;

  return res;
}

uint32_t lslCmd(char* inp) {
  strtok(inp, " ,\n");
  char* rn = strtok(NULL, " r,\n");
  char* op1 = rn;
  char* op2 = "lsl";
  char* op3 = strtok(NULL, " r,\n");
  return dataCmd("1101", rn, "0", op1, op2, op3, 0);
}

// TODO: Assumes a symTable size of 32 for now.
uint8_t lookUpLineNum(struct sym* symTable, char* name) {
  for (int i = 0; i < 32; i++) {
    if (strcmp(symTable[i].name, name) == 0) {
      return symTable[i].lineNum;
    }
  }
  return -1;
}

uint32_t branchCmd(char* inp, struct sym* symT, int lineNo) {
  char* command = strtok(inp, " ");

  char* label = stripStr(strtok(NULL, " "), '\n');
  int labelLineNo = lookUpLineNum(symT, label);
  int32_t offset = labelLineNo - (lineNo + 2);
  uint32_t offset24Bits = offset & (0x00FFFFFF);

  uint32_t res;
  switch (command[1]) {
    case 'e':
      //beq
      res = 0x0A000000;
      break;
    case 'n':
      //bne
      res = 0x1A000000;
      break;
    case 'g':
      //bgt and bge
      res = command[2] == 'e' ? 0xAA000000 : 0xCA000000;
      break;
    case 'l':
      //blt and ble
      res = command[2] == 'e' ? 0xDA000000 : 0xBA000000;
      break;
    default:
      res = 0xEA000000;
  }

  res += offset24Bits;
  return res;
}

uint32_t parseStr(char* inp, struct sym* symT, int lineNo, int noInst, uint32_t* sdtAppend){
  switch(inp[0]) {
    case 'b':
    return branchCmd(inp, symT, lineNo);
    break;
      switch(inp[1]) {
        case 'l':
          if(inp[2] == 't') {
            // blt
          } else {
            //ble
          }
          break;

        case 'g':
          if(inp[2] == 't') {
            // bgt
          } else {
            // bge
          }
          break;

        case 'n':
          //bne
          break;

        case 'e':
          // beq
          break;

        case ' ':
          // b
          break;

        default:
          exit(EXIT_FAILURE);
      }
      break;

    case 'a':
      switch(inp[1]) {
        case 'n':
          if(inp[3] == 'e') {
            return 0;
          } else {
            return processData(inp, "0000", 0);
          }
          break;

        case 'd':
          return processData(inp, "0100", 0);
          break;

        default:
          exit(EXIT_FAILURE);
      }
      break;

    case 'm':
      switch(inp[1]) {
        case 'o':
          return processData(inp, "1101", 1);
          break;

        case 'u':
          return multCmd(inp);
          break;

        case 'l':
          return multCmd(inp);
          break;

        default:
          exit(EXIT_FAILURE);
      }
      break;

    case 's':
      if(inp[1] == 'u') {
        return processData(inp, "0010", 0);
      } else {
        strtok(inp, " r,\n");
        char* rd = strtok(NULL, " r,\n");
        char* address = strtok(NULL, "");
        address = cleanDelim(address, " ,\n");
        return sdtCmd(rd, address, 0, sdtAppend, noInst - lineNo, 1);
      }
      break;

    case 't':
      if(inp[1] == 's') {
        return processData(inp, "1000", 2);
      } else {
        return processData(inp, "1001", 2);
      }
      break;

    case 'l':
      if(inp[1] == 'd') {
        strtok(inp, " r,\n");
        char* rd = strtok(NULL, " r,\n");
        char* address = strtok(NULL, "");
        address = cleanDelim(address, " ");
        return sdtCmd(rd, address, 1, sdtAppend, noInst - lineNo, 1);

      } else {
        return lslCmd(inp);
      }
      break;

    case 'r':
      return processData(inp, "0011", 0);
      break;

    case 'e':
      return processData(inp, "0001", 0);
      break;

    case 'o':
      return processData(inp, "1100", 0);
      break;

    case 'c':
      return processData(inp, "1010", 2);
      break;

    default:
      exit(EXIT_FAILURE);
  }
return -1;
}


// Returns an array of 32 byte
uint32_t* secondPass(struct sym* symTable, FILE* fp, uint32_t* noInst){
  uint32_t totalSize = *noInst;
  if(fseek(fp, 0, SEEK_SET) == -1) {
    perror("File IO has failed.");
    exit(EXIT_FAILURE);
  }

  char* currStr = calloc(512, sizeof(char));
  uint32_t* byteTable = calloc(totalSize, sizeof(uint32_t));
  int lineNum = 0;

  while (fgets(currStr, 512, fp) != NULL) {
      currStr = cleanDelim(currStr, " \n");

      if (*currStr == '\0') {
        continue;
      }
      if (*currStr == '/') {
        continue;
      }

      printf("Current Inp: %s\n", currStr);
      // Empty line, go to next
        if(chrExists(currStr, ':') == 0) {
          uint32_t *sdtAppend = calloc(1, sizeof(uint32_t));
          byteTable[lineNum] = parseStr(currStr, symTable, lineNum, totalSize, sdtAppend);

          // If a sdt instruction was called
          if (sdtAppend[0] != 0) {
            byteTable = realloc(byteTable, sizeof(uint32_t) * (totalSize + 1));
            byteTable[totalSize] = sdtAppend[0];
            *noInst = totalSize + 1;
            totalSize = *noInst;
          }
          lineNum++;
        }

  }
  //if (*currStr != NULL) {
    //free(currStr);
  //}
  return byteTable;
}
