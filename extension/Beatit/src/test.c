/*
A simple example of using the gfx library.
CSE 20211
9/7/2011
by Prof. Thain
*/

#include "gfx.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>

void matrixSame(int** m1, int** m2)
{
    for (int y = 0; y < 2; y++) {
      for (int x = 0; x < 2; x++) {
        if (m1[y][x] != m2[y][x]) {
          printf("FAIL: m1 = %d, m2 = %d\n",m1[y][x], m2[y][x]);
        }
      }
    }
}

int** allocMatrix(unsigned int rows, unsigned int cols) {
  int** matrix;
  unsigned int i;
  matrix = (int**) malloc (rows * sizeof(int*));
  if (!matrix) {
    return NULL;
  }
  matrix[0] = (int*) malloc (rows*cols*sizeof(int));
  if (!matrix[0]) {
    free(matrix);
    return NULL;
  }
  for (i = 1; i < rows; i++) {
    matrix[i] = matrix[i-1] + cols;
  }

  for (int y = 0; y < 2; y++) {
    for (int x = 0; x < 2; x++) {
      matrix[y][x] = rand();
    }
  }
  return matrix;
}

int main(int argc, const char * argv[]) {
  int** m1 = allocMatrix(2, 2);
  int** m2 = allocMatrix(2, 2);
  free(m2[0]);
  m2[0] = memcpy(m2[0], m1[0], sizeof(int)*2*2);
  for (int i = 1; i < 2; i++) {
    m2[i] = m2[i-1] + 2;
  }
  matrixSame(m1, m2);
}
