/*
 *  lcg.cs
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <lib/rand/lcg.h>
#include <inttypes.h>

static int rseed = 0;

/*
    Linear Congruential Generator
*/
int lcg_srand(int seed) {
    rseed = seed;
    return rseed;
}

int lcg_rand(void) {
    rseed = (rseed * 1103515245 + 12345) & 0x7FFFFFFF;
    return (uint32_t)rseed;
}