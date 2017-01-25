#include <stdio.h>
#include "bitslab.h"
int main() {
  printf("Test with main.\n");
  printf("bitAnd Result: %d\n", bitAnd(15,3));
  printf("logicalShift REsult: %x\n", logicalShift(0x87654321, 4));
}
