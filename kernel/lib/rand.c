/*
 *  rand.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <lib/panic.h>
#include <lib/rand.h>

#include <lib/rand/mt19937.h>
#include <lib/rand/lcg.h>

typedef int (*rand_algorithm_seed_func_t)(int seed);
typedef int (*rand_algorithm_rand_func_t)(void);

const rand_algorithm_seed_func_t rand_alg_seed_funcs[] = {lcg_srand, mt_srand};
const rand_algorithm_rand_func_t rand_alg_rand_funcs[] = {lcg_rand, mt_rand};

int alg = 0;

int set_rand_algorithm(enum rand_algorithms a) {
    alg = a;
    return alg;
}

int srand(int seed) { return rand_alg_seed_funcs[alg](seed); }

int rand() { return rand_alg_rand_funcs[alg](); }