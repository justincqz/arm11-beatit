#include <string.h>
#include <stdint.h>
#include <stdlib.h>

/* Checks if a given character exists in a string, and returns 1 if it does.*/
uint8_t chrExists(char* s, char c) {
  while(*s != '\0') {
    if (*s == c){
      return 1;
    }
    s++;
  }
  return 0;
}

// Strips the string at the delimiter (and trailing characters)
// Returns a copy of the string if the character is not found
char* stripStr(char* s, char delimiter){
  int nSize = 0;
  char* res;

  while (nSize <= sizeof(s)){
    if(s[nSize] == delimiter) {
      nSize++;
      break;
    }
    nSize++;
  }

  res = malloc(nSize * sizeof(char));
  strncpy(res, s, nSize);
  res[nSize - 1] = '\0';
  return res;
}

// Clear leading and trailing delimiters
char* cleanDelim(char* inp, char* delim) {
  if (inp == NULL) {
    return inp;
  }

  while(chrExists(delim, *inp) == 1 && *inp != '\0') {
    inp++;
  }

  if (strlen(inp) > 1) {
    for (int i = strlen(inp) - 1; i >= 0; i--) {
      if (chrExists(delim, inp[i]) == 0) {
        inp[i + 1] = '\0';
        return inp;
      }
    }
  }

  return inp;
}

// Rotates x left by n
unsigned rotl(unsigned x, unsigned n) {
  return (x << n) | ((x >> (32 - n)) & ~(-1 << n));
}

// Calculates the amount of right rotate required to fit the 32 bit into 8 bits
int countRRot(uint32_t v) {
  uint32_t t = v;
  for (int i = 0; i < 31; i++) {
    if ((rotl(t, i) & 0xFFFFFF00) == 0) {
      return 32 - i;
    }
  }
  return -1;
}
