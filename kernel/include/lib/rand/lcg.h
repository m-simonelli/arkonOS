/*
 *  lcg.h
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#ifndef _lib_rand_lcg_h
#define _lib_rand_lcg_h

int lcg_srand(int seed);
/* Lower 31 bits are useable for random */
int lcg_rand(void);

#endif