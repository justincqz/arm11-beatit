#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "strutils.h"
#include "arm11def.h"
#include "arm11struct.h"
#include "arm11io.h"
#include "parsestr.h"
#include "assencode.h"

uint32_t* firstPass(Sym_t** symTable, FILE* fp);
uint32_t* secondPass(Sym_t** symTable, FILE* fp, uint32_t* noInst, size_t sizeOfSymT);
void delete_symTable(Sym_t** symT, int size);

int main(int argc, char **argv) {
  FILE* inpF;
  uint32_t* binT;
  uint32_t numInst;
  size_t sizeSymT;

  if(argc != 3) {
      printf("Usage: %s <inputFile> <outputFile>\n", argv[0]);
      return -1;
  }

  Sym_t** symT = calloc(32, sizeof(Sym_t*));
  inpF = ass_read(argv[1]);

  // First pass
  uint32_t* out;
  out = firstPass(symT, inpF);
  numInst = out[0];
  sizeSymT = out[1];

  // Second pass
  binT = secondPass(symT, inpF, &numInst, sizeSymT);
  fclose(inpF);

  // Write binary file
  ass_write(argv[2], binT, numInst);

  free(out);
  free(binT);
  delete_symTable(symT, sizeSymT);
  return EXIT_SUCCESS;
}

// Generates the symbol table, counts the number of lines and size of symTable
uint32_t* firstPass(Sym_t** symTable, FILE* fp) {
  uint32_t lineNum = 0;
  int currSym = 0;
  char* currStr = calloc(512, sizeof(char));
  char *tmp = currStr;

  while(fgets(tmp, 512, fp) != NULL) {
    tmp = cleanDelim(tmp, " \n");
    if(chrExists(tmp, ':') == 0 && *tmp != '\0'){
      lineNum++;
    } else {
      Sym_t* sym = new_symbol();
      sym->name = stripStr(tmp, ':');
      sym->lineNum = lineNum;
      symTable[currSym] = sym;
      currSym++;
    }
  }

  free(currStr);
  uint32_t* out = calloc(2, sizeof(uint32_t));
  out[0] = lineNum;
  out[1] = currSym;
  return out;
}

// Generates the array of bitcodes to write to file
uint32_t* secondPass(Sym_t** symTable, FILE* fp, uint32_t* noInst, size_t sizeOfSymT){
  uint32_t totalSize = *noInst;

  if(fseek(fp, 0, SEEK_SET) == -1) {
    perror("File IO has failed.");
    exit(EXIT_FAILURE);
  }

  char* currStr = calloc(512, sizeof(char));
  uint32_t* binArray = calloc(totalSize, sizeof(uint32_t));
  int lineNum = 0;
  char *tmp = currStr;

  while (fgets(tmp, 512, fp) != NULL) {
    tmp = cleanDelim(tmp, " \n");
    if (*tmp == '\0') {
      continue;
    }
    if (*tmp == '/') {
      continue;
    }

    printf("Current Inp: %s\n", tmp);
    if(chrExists(tmp, ':') == 0) {
      uint32_t *sdtAppend = calloc(1, sizeof(uint32_t));
      binArray[lineNum] = parseStr(tmp, symTable, lineNum, totalSize, sdtAppend, sizeOfSymT);

      if (sdtAppend[0] != 0) {
        binArray = realloc(binArray, sizeof(uint32_t) * (totalSize + 1));
        binArray[totalSize] = sdtAppend[0];
        *noInst = ++totalSize;
      }

      lineNum++;
      free(sdtAppend);
    }
  }
  free(currStr);
  return binArray;
}

void delete_symTable(Sym_t** symT, int size) {
  for(int i = 0; i < size; i++) {
    delete_symbol(symT[i]);
  }
  free(symT);
}
