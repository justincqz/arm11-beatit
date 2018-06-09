#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "strutils.h"
#include "arm11def.h"
#include "arm11struct.h"
#include "parseStr.h"
#include "assencode.h"

FILE* openFile(char *path);
uint32_t* firstPass(Sym_t** symTable, FILE* fp);
uint32_t* secondPass(Sym_t** symTable, FILE* fp, uint32_t* noInst);
int writeFile(char* outFile, uint32_t* binTable, uint32_t noInst);

int main(int argc, char **argv) {
  FILE* inpF;
  uint32_t* binT;
  uint32_t numInst;
  //uint32_t sizeSymT;

  if(argc != 3) {
      printf("Usage: %s <inputFile> <outputFile>\n", argv[0]);
      return -1;
  }

  // Assuming that all files will have less than 32 labels
  // TODO: Support more than 32 labels
  Sym_t** symT = calloc(32, sizeof(Sym_t*));

  inpF = openFile(argv[1]);

  // First pass, generates the symbol table and counts the number of instructions and size of symTable
  uint32_t* out;
  out = firstPass(symT, inpF);
  numInst = out[0];
  //sizeSymT = out[1];

  // Second pass, generates the array of bitcodes to write to file
  binT = secondPass(symT, inpF, &numInst);
  fclose(inpF);



  // Write binary file
  writeFile(argv[2], binT, numInst);
  
  free(out);
  free(binT);
  for (int i = 0; i < 32; i ++) {
    delete_symbol(symT[i]);
  }
  free(symT);
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
  if (fp == NULL) {
    perror("No such file");
    exit(EXIT_FAILURE);
}

  fwrite(binTable, sizeof(uint32_t), noInst, fp);
  fclose(fp);
  return 0;
}

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

// Returns an array of 32 byte
uint32_t* secondPass(Sym_t** symTable, FILE* fp, uint32_t* noInst){
  uint32_t totalSize = *noInst;
  if(fseek(fp, 0, SEEK_SET) == -1) {
    perror("File IO has failed.");
    exit(EXIT_FAILURE);
}
  char* currStr = calloc(512, sizeof(char));
  uint32_t* byteTable = calloc(totalSize, sizeof(uint32_t));
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
    // Empty line, go to next
    if(chrExists(tmp, ':') == 0) {
      uint32_t *sdtAppend = calloc(1, sizeof(uint32_t));
      byteTable[lineNum] = parseStr(tmp, symTable, lineNum, totalSize, sdtAppend);

        // If a sdt instruction was called
      if (sdtAppend[0] != 0) {
        byteTable = realloc(byteTable, sizeof(uint32_t) * (totalSize + 1));
        byteTable[totalSize] = sdtAppend[0];
        *noInst = totalSize + 1;
        totalSize = *noInst;
      }
      lineNum++;
      free(sdtAppend);
    }
  }
  free(currStr);
  return byteTable;
}

void delete_symTable(Sym_t** symT, int size) {
  for(int i = 0; i < size; i++) {
    delete_symbol(symT[i]);
  }
}
