/*
 *  rand.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _lib_rand_h
#define _lib_rand_h

#include <inttypes.h>

enum rand_algorithms {
    RAND_ALGORITHM_LCG = 0,
    RAND_ALGORITHM_MT
};

/*
    Random generator seed functions
*/
int lcg_srand(int seed);
int mt_srand(int seed);

/*
    Random generator rand functions
*/
/* Lower 31 bits are useable for random */
int lcg_rand(void);
/* Lower 32 bits are useable for random */
int mt_rand();

/*
    Other random functions
*/
void mt_twist();

/*
    Actual rand functinos
*/

/*!
    @function srand
    Set the seed of the currently set random generator (defaults to LCG)

    @param seed
    The seed to be set

    @return
    The returned value from the seed function, usually the seed
!*/
int srand(int seed);

/*!
    @function rand
    Get a random value from the currently set random generator (defaults to LCG)

    @return
    The returned value from the set rand function
!*/
int rand();

/*!
    @function set_rand_algorithm
    Set the random generator algorithm as defined in `enum rand_algorithms`

    @return
    The set algorithm
!*/
int set_rand_algorithm(enum rand_algorithms a);

#endif /* _lib_rand_h */