#ifndef STRUTILS_H
#define STRUTILS_H

uint8_t chrExists(char* s, char c);

char* stripStr(char* s, char delimiter);

char* cleanDelim(char* inp, char* delim);

unsigned rotl(unsigned x, unsigned n);

int countRRot(uint32_t v);

#endif
