//
// Created by Yoram Boccia on 24/05/2018.
//

int const INSTRUCTION_LENGTH = 32;
char const ONE = '1';
char const EOF = '\0';

//instruction from file to address
int read(char *fileName, int *address);

//helper function to convert binary to decimal and save in the address
int convert(char *buffer, int *address);

//write(Storage storage)

