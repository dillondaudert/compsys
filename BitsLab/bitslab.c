/* 
 * Date: 01/25/2017
 * Class: CS 5541
 * Assignment: Bits Lab
 * Author: Dillon Daudert
 *
 * 
 * bitslab.c - Source file with solutions to the Lab.
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to Assignment 2 by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     Note that '=' is not counted; you may use as many of these as you 
     want without penalty.
  2. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 */


#endif
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Points: 2
 */
int bitAnd(int x, int y) {
    /*Use the fact that ~(a ^ b) = ~a v ~b*/
    return ~(~x | ~y);
}





/* 
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Points: 2
 */
int getByte(int x, int n) {
    /*Shift by 8*2^n bits for the correct byte, and & with 0xFF*/
    int byte_mask = 0xFF;
    x >>= ((3 - n) << 3);

    return x & byte_mask;
}





/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Points: 3 
 */
int logicalShift(int x, int n) {
    /*Use shifts on a mask and take its complement. 
     * Split the mask shifts into two parts to avoid shifting 
     * by the word size.*/
    int mask = ~0x0;
    x >>= n;
    mask <<= (32 - n - 1);
    mask <<= 1;
    return x & ~mask;
}





/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Points: 4
 */
int bitCount(int x) {
  return 2;
}



/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Points: 4 
 */
int bang(int x) {
    /* Logical Or the halves of x together, reducing down to the least 
     * significant bit having 1 if x != 0. Logical And with 1 to get rid
     * of the rest of the bits in the number, and XOR for final answer.
     */
    int byte2 = x | (x >> 16);
    int byte1 = byte2 | (byte2 >> 8);
    int bit4 = byte1 | (byte1 >> 4);
    int bit2 = bit4 | (bit4 >> 2);
    int bit1 = bit2 | (bit2 >> 1);

    return (bit1 & 1) ^ 1;
}




/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Points: 1
 */
int tmin(void) {
    return 0x1 << 31;
}




/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Points: 2
 */
int fitsBits(int x, int n) {
  return 2;
}




/* 
 * divpwr2 - Compute x/(2^n), for 0 <= n <= 30
 *  Round toward zero
 *   Examples: divpwr2(15,1) = 7, divpwr2(-33,4) = -2
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Points: 2
 */
int divpwr2(int x, int n) {
    return x >> n;
}




/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Points: 2
 */
int negate(int x) {
    /* Two's complement
     */
    return ~x + 1;
}




/* 
 * isPositive - return 1 if x > 0, return 0 otherwise 
 *   Example: isPositive(-1) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Points: 3
 */
int isPositive(int x) {
    /* If we were only looking at nonnegative, then we could just check
     * the sign bit. Sign we have to account for 0, we need to look at the
     * edge case where the sign bit flips (0 and tmin). If we subtract one
     * and the sign before and after is 0, then x > 0.*/
    int sign = x >> 31;
    int zero_sign = (x - 1) >> 31;
    return ~(sign | zero_sign) & 0x1;
}




/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Points: 3
 */
int isLessOrEqual(int x, int y) {
    /* Check if x - y <= 0.
     * 
     */
    return 2;
}




/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Points: 4
 */
unsigned float_neg(unsigned uf) {
    /* Since the input unsigned int is the bit-level representation of a
     * float, I just reverse the parity of the sign bit.
     */
    unsigned sign = 0x1 << 31;
    return uf ^ sign;
}




/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Points: 4
 */
unsigned float_i2f(int x) {
    /* Iterate over the integer to calculate the floating point #
     */

    //Get the sign bit
    int x_as_f = 0x0 | (x >> 31);
    int exp = 127;
    int mant = 
    //Iterate over x (skipping sign), count exp and copy mantissa
    x_tmp = x;
    int b = 1;
    int exp_found = 0;
    while(b < 32){
        x_tmp <<= 1;
        x_as_f <<= 1;

        if(!exp_found){
            //If leading bit is nonzero, begin counting exponent
            if((0x1 << (31-b))&x_tmp){
                exp_found = 1;
            }
        }else{
            //Leading bit found
            exp += 1;

        }

        b += 1;
    }

  return 2;
}




/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Points: 4
 */
unsigned float_twice(unsigned uf) {
  return 2;
}
