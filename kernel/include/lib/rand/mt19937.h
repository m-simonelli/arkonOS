/*
 *  mt19937.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _lib_rand_mt19937_h
#define _lib_rand_mt19937_h

int mt_srand(int seed);
/* Lower 32 bits are useable for random */
int mt_rand();
void mt_twist();

#endif