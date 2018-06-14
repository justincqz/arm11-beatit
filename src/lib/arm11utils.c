#include <stdint.h>

uint32_t extract_bits(int input, int lowerBit, int upperBit) {
  return (((1 << (upperBit - lowerBit + 1)) - 1) & (input >> lowerBit));
}
