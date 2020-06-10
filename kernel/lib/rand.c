/*
 *  rand.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <lib/rand.h>
#include <lib/panic.h>

/*
    MT19937 coefficients
*/
#define MT_W (32)
#define MT_N (624)
#define MT_M (397)
#define MT_R (31)
#define MT_A (0x9908B0DF)
#define MT_U (11)
#define MT_D (0xFFFFFFFF)
#define MT_S (9)
#define MT_B (0x9D2C5680)
#define MT_T (15)
#define MT_C (0xEFC60000)
#define MT_L (18)
#define MT_F 1812433253

static int MT[MT_N - 1] = {0};
static int mt_idx = MT_N + 1;

static const unsigned int mt_lmask = (int)((uint64_t)(1 << MT_R) - 1);
static const unsigned int mt_umask = ~mt_lmask & (((uint64_t)1 << MT_W) - 1);

typedef int(*rand_algorithm_seed_func_t)(int seed);
typedef int(*rand_algorithm_rand_func_t)(void);

const rand_algorithm_seed_func_t rand_alg_seed_funcs[] = {
    lcg_srand, 
    mt_srand
};

const rand_algorithm_rand_func_t rand_alg_rand_funcs[] = {
    lcg_rand, 
    mt_rand
};

int alg = 0;
int rseed = 0;

/*
    Linear Congruential Generator
*/
int lcg_srand(int seed){
    rseed = seed;
    return rseed;
}

int lcg_rand(void){
    rseed = (rseed * 1103515245 + 12345) & 0x7FFFFFFF;
    return (uint32_t)rseed;
}

/*
    Mersenne
*/
int mt_srand(int seed){
    mt_idx = MT_N;
    rseed = seed;
    
    MT[0] = seed;
    for(int i = 1; i < MT_N - 1; i++){
        MT[i] = (MT_F * (MT[i - 1] ^ (MT[i - 1] >> (MT_W - 2))) + i) & (((uint64_t)1 << MT_W) - 1);
    }

    return rseed;
}

int mt_rand(){
    if(mt_idx >= MT_N){
        if(mt_idx > MT_N){
            panic("MT rand attempted without seed!\n");
        }
        mt_twist();
    }

    int rand = MT[mt_idx];
    rand = rand ^ ((rand >> MT_U) & MT_D);
    rand = rand ^ ((rand << MT_S) & MT_B);
    rand = rand ^ ((rand << MT_T) & MT_C);
    rand = rand ^ (rand >> MT_L);

    mt_idx++;

    return rand & (((uint64_t)1 << MT_W) - 1);
}

void mt_twist(){
    for(int i = 0; i < MT_N - 1; i++){
        int x = (MT[i] & mt_umask) + (MT[(i + 1) % MT_N] & mt_lmask);
        int xA = x >> 1;
        if (x & 1) xA = xA ^ MT_A;
        MT[i] = MT[(i + MT_M) % MT_N] ^ xA;
    }
    mt_idx = 0;
}

int set_rand_algorithm(enum rand_algorithms a){
    alg = a;
    return alg;
}

int srand(int seed){
    return rand_alg_seed_funcs[alg](seed);
}

int rand(){
    return rand_alg_rand_funcs[alg]();
}