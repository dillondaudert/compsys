#include <stdio.h>
#include "bitslab.h"
int main() {
  printf("Test with main.\n");
  printf("bitAnd Result: %d\n", bitAnd(15,3));
  printf("logicalShift REsult: %x\n", logicalShift(0x87654321, 31));
  printf("getByte Result: %x\n", getByte(0x87654321, 0));
  printf("bang Result: %x\n", bang(-31));
  printf("tmin Result: %d\n", tmin());
  printf("divpwr2 result1: %d, 2: %d\n", divpwr2(15, 1), divpwr2(-33,4));
  printf("negate result: %d\n", negate(-10));
  printf("isPositive result: %d\n", isPositive(0));
}
