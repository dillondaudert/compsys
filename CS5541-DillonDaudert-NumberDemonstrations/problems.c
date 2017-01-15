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
    printf("Result: %1.10f\n\n", f);
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
    printf("Result: %1.10f\n\n", f);
}

/*
 *Declare a variable of type double
 *Assign it the value 1/2
 *Print it
 *Assign it the value 1.0/2.0
 *Print it
 */
void prob3()
{
    float f = 1/2;
    printf("Problem3a:\nExpected: If the compiler converts to floats before doing the operation, 0.5, otherwise 0.\n");
    printf("Result: %f\n", f);
    f = 1.0/2.0;
    printf("Problem3b:\nExpected: 0.5\n");
    printf("Result: %f\n\n", f);
}

/*
 *Problem 4: Declare a double
 *Assign it the value 99999999.4499999999
 *Print it
 *Cast it to a float
 *Print the float
 */
void prob4()
{
    double d = 9999999.4499999999;
    printf("Problem 4a:\nExpected: 99999999.4499999999.\n");
    printf("Result: %g\n", d);
    float f = d;
    printf("Problem 4b:\nExpected: Something weird.\n");
    printf("Result: %f\n\n", f);
}

void prob5()
{
    int a = 30000*30000;
    printf("Problem 5a:\nExpected: 900,000,000.\n");
    printf("Result: %d\n", a);
    a = 40000*40000;
    printf("Problem 5b:\nExpected: 1,600,000,000.\n");
    printf("Result: %d\n", a);
    a = 50000*50000;
    printf("Problem 5c:\nExpected: A large negative integer.\n");
    printf("Result: %d\n", a);
    a = 60000*60000;
    printf("Problem 5d:\nExpected: A slightly smaller negative integer.\n");
    printf("Result: %d\n\n", a);
}

void prob6()
{
    float f = 1e20;
    printf("Problem 6a:\nExpected: A large number\n");
    printf("Result: %f\n", f);
    f = 1e20 + 3500000000;
    printf("Problem 6b:\nExpected: This is probably too large for a float\n");
    printf("Result: %f\n", f);
    f = 1e20 + (3500000000 * 1000000000);
    printf("Problem 6c:\nExpected: For the following, I'm not sure what to expect\n");
    printf("Result: %f\n", f);
    f = 1e20;
    int i;
    for(i = 0; i < 1000000000; i++){
        f += 3500000000;
    }
    printf("Problem 6d:\n");
    printf("Result: %f\n\n", f);
}

int main(int argc, char **argv)
{
    prob1();
    prob2();
    prob3();
    prob4();
    prob5();
    prob6();
    return 1;
}
