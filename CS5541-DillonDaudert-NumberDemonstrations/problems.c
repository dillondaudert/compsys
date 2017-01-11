/*
 * Date: 01/11/2017
 * Class: CS 5541
 * Assignment: Number Demonstrations, Assignment 1
 * Author(s): Dillon Daudert
 */

#include <stdio.h>


/*
 *Declare a variable of type float, 
 *Assign it the value 3.6
 *Print it to ten decimal places
 */
void prob1()
{
    float f = 3.6f;
    printf("Problem 1:\nExpected: 3.6000000000\n");
    printf("Result: %1.10f\n", f);
}


/*
 *Declare a float
 *Assign it hte value 1.0/10.0
 *Print it to ten decimal places
 */
void prob2()
{
    float f = 1.0/10.0;
    printf("Problem 2:\nExpected: Something close to 0.100... but not exactly.\n");
    printf("Result: %1.10f\n", f);
}

int main(int argc, char **argv)
{
    prob1();
    prob2();
    return 1;
}
