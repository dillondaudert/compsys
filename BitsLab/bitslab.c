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
    /* Brute force since I can't think of any other way
     *
     */
    int bit_count = 0;
    bit_count += (0x1 & x);
    bit_count += ((0x1 << 1) & x) >> 1;
    bit_count += ((0x1 << 2) & x) >> 2;
    bit_count += ((0x1 << 3) & x) >> 3;
    bit_count += ((0x1 << 4) & x) >> 4;
    bit_count += ((0x1 << 5) & x) >> 5;
    bit_count += ((0x1 << 6) & x) >> 6;
    bit_count += ((0x1 << 7) & x) >> 7;
    bit_count += ((0x1 << 8) & x) >> 8;
    bit_count += ((0x1 << 9) & x) >> 9;
    bit_count += ((0x1 << 10) & x) >> 10;
    bit_count += ((0x1 << 11) & x) >> 11;
    bit_count += ((0x1 << 12) & x) >> 12;
    bit_count += ((0x1 << 13) & x) >> 13;
    bit_count += ((0x1 << 14) & x) >> 14;
    bit_count += ((0x1 << 15) & x) >> 15;
    bit_count += ((0x1 << 16) & x) >> 16;
    bit_count += ((0x1 << 17) & x) >> 17;
    bit_count += ((0x1 << 18) & x) >> 18;
    bit_count += ((0x1 << 19) & x) >> 19;
    bit_count += ((0x1 << 20) & x) >> 20;
    bit_count += ((0x1 << 21) & x) >> 21;
    bit_count += ((0x1 << 22) & x) >> 22;
    bit_count += ((0x1 << 23) & x) >> 23;
    bit_count += ((0x1 << 24) & x) >> 24;
    bit_count += ((0x1 << 25) & x) >> 25;
    bit_count += ((0x1 << 26) & x) >> 26;
    bit_count += ((0x1 << 27) & x) >> 27;
    bit_count += ((0x1 << 28) & x) >>28;
    bit_count += ((0x1 << 29) & x) >>29;
    bit_count += ((0x1 << 30) & x) >> 30;
    bit_count += ((0x1 << 31) & x) >> 31;
    return bit_count;
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
    /* 5, 3 = 00000101, 000
     *-4, 3 = 11111100, 000
     */
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
    int sign = 0x1 & (x >> 31);
    return (x >> n) + sign;
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
     * the sign bit. Since we have to account for 0, we need to look at the
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
    /* Check if 0 <= y + x.
     * Add x to y and return the opposite of the sign bit
     */
    y -= x;

    return !(y & (0x1 << 31));
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

    unsigned x_as_f;

    //If the integer is negative, need to do 2's complement
    if(x < 0){
        //set sign bit
        x_as_f = 0x80000000;
        //Set x to positive value
        x = -x;
    }else{
        x_as_f = 0x0;
    }

    //Iterate over x (skipping sign), count exp and copy mantissa
    int x_tmp = x;
    int lead_pos = 1;
    int lead_found = 0;
    int exp = 0;

    while(lead_pos <= 32){
        //If we haven't found the leading bit yet, decrement the exponent
        //Since only 23 of the bits in the mantissa can be copied, decrement
        if(!lead_found){
            if((0x1 << (32-lead_pos))&x_tmp){
                //set leading 1 to 0
                x_tmp ^= (0x1 << (32-lead_pos));
                lead_found = 1;
                exp = 32 - lead_pos;
            }
        }
        lead_pos += 1;
    }

    int mant_offset = 32 - exp - 9;
    if(exp != 0) {exp += 127;}
    
    //OR mantissa into the FP number
    if(mant_offset < 0) {
        x_as_f |= (x_tmp >> -mant_offset);
    }else{
        x_as_f |= (x_tmp<<mant_offset);
    }

    //OR exponent into FP number
    x_as_f |= (exp << 23);

    return x_as_f;
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
    /*
     * For normalized, add 1 to exponent
     * For denormalized, shift mantissa 1
     *
     */
    
    //Extract exponent and mantissa
    int exp = (0xFF << 23) & uf;
    int mant = 0x007FFFFF & uf;
    printf("exp: %x, mant: %x\n", exp, mant);
    exp >>= 23;


    if(exp == 0){//Denormalized
        mant <<= 1;
        uf &= 0xFF800000;
        uf |= mant;
        
    }else if((exp + 1)%256 > exp){
        exp += 1;
        printf("exp+1=%x\n", exp<<23);
        uf &= (0x807FFFFF);
        uf |=(exp<<23);
    }


    return uf;
}
