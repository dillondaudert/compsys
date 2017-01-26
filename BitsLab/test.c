#include <stdio.h>
#include "bitslab.h"
int main() {
  printf("Test with main.\n");
  printf("bitAnd Result: %d\n", bitAnd(15,3));
  printf("logicalShift REsult: %x\n", logicalShift(0x87654321, 31));
  printf("getByte Result: %x\n", getByte(0x87654321, 0));
  printf("bang Result: %x\n", bang(-31));
  printf("tmin Result: %d\n", tmin());
}
