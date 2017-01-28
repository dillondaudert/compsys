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
  printf("esLessOrEqual result: %d\n", isLessOrEqual(4, 5));
  printf("float_neg result: %x\n", float_neg(0x3FFFFFFF));
  printf("float_i2f result: %x\n", float_i2f(100000));
  printf("float_i2f result: %x\n", float_i2f(0));
  printf("float_twice result: %x\n", float_twice(0x3dcccccd));
  printf("bitcount result: %d\n", bitCount(7));
}
