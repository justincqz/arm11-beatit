#include "arm11def.h"
#include "arm11struct.h"
#include "parsedata.h"
#include <stdlib.h>
#include <string.h>

uint32_t parseStr(char* inp, Sym_t* symT, int currLine, int numInst, uint32_t* sdtAppend) {
  char secChar = inp[1];
  char thirdChar = inp[2];
  switch(*inp) {
    case 'b':
      switch(secChar) {
        case 'l':
          if(thirdChar == 't') {
            return branchCmd(inp, symT, LT, currLine);
          } else {
            return branchCmd(inp, symT, LE, currLine);
          }

        case 'g':
          if(thirdChar == 't') {
            return branchCmd(inp, symT, GT, currLine);
          } else {
            return branchCmd(inp, symT, GE, currLine);
          }
        case 'n':
          return branchCmd(inp, symT, NE, currLine);

        case 'e':
          return branchCmd(inp, symT, EQ, currLine);

        case ' ':
          return branchCmd(inp, symT, AL, currLine);

        default:
          exit(EXIT_FAILURE);
      }

    case 'a':
      switch(secChar) {
        case 'n':
          if(inp[3] == 'e') {
            return 0;
          } else {
            return parseDataCmd(inp, AND);
          }

        case 'd':
          return parseDataCmd(inp, ADD);

        default:
          exit(EXIT_FAILURE);
      }

    case 'm':
      switch(secChar) {
        case 'o':
          return parseDataCmd(inp, MOV);

        case 'u':
          return multCmd(inp);

        case 'l':
          return multCmd(inp);

        default:
          exit(EXIT_FAILURE);
      }

    case 's':
      if(secChar == 'u') {
        return parseDataCmd(inp, SUB);

      } else {
        return parseSDTCmd(inp, sdtAppend, numInst - currLine);
      }
      break;

    case 't':
      if(secChar == 's') {
        return parseDataCmd(inp, TST);

      } else {
        return parseDataCmd(inp, TEQ);
      }
      break;

    case 'l':
      if(secChar == 'd') {
        return parseSDTCmd(inp, sdtAppend, numInst - currLine);

      } else {
        return lslCmd(inp);
      }
      break;

    case 'r':
      return parseDataCmd(inp, RSB);
      break;

    case 'e':
      return parseDataCmd(inp, EOR);
      break;

    case 'o':
      return parseDataCmd(inp, ORR);
      break;

    case 'c':
      return parseDataCmd(inp, CMP);
      break;

    default:
      return -1;
  }
  return -1;
}
