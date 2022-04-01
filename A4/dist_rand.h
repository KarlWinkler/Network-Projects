#ifndef RAND_H
#define RAND_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Parameters for random number generation. */
#define MAX_INT 2147483647       /* Maximum positive integer 2^31 - 1 */

/* Generate a random floating point number uniformly distributed in [0,1] */
double Uniform01(){
    double randnum;
    /* get a random positive integer from random() */
    randnum = (double) 1.0 * rand();
    /* divide by max int to get something in the range 0.0 to 1.0  */
    randnum = randnum / (1.0 * MAX_INT);
    return( randnum );
}

/* Generate a random floating point number from an exponential    */
/* distribution with mean mu.                                     */
double Exponential(double mu){
    double randnum, ans;

    randnum = Uniform01();
    ans = -(mu) * log(randnum);
    return( ans );
}

#endif