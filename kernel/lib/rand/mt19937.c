/*
 *  mt19937.c
 *  Copyright Marco Simonelli 2020
 *  You are free to redistribute/modify this code under the
 *  terms of the GPL version 3 (see the file LICENSE)
 */

#include <lib/rand/mt19937.h>
#include <inttypes.h>
#include <lib/panic.h>

/*
    MT19937 coefficients
*/
#define MT_19937_W (32)
#define MT_19937_N (624)
#define MT_19937_M (397)
#define MT_19937_R (31)
#define MT_19937_A (0x9908B0DF)
#define MT_19937_U (11)
#define MT_19937_D (0xFFFFFFFF)
#define MT_19937_S (9)
#define MT_19937_B (0x9D2C5680)
#define MT_19937_T (15)
#define MT_19937_C (0xEFC60000)
#define MT_19937_L (18)
#define MT_19937_F 1812433253

static int MT[MT_19937_N - 1] = {0};
static int mt_idx = MT_19937_N + 1;

static int rseed = 0;

static const unsigned int mt_lmask = (int)((uint64_t)(1 << MT_19937_R) - 1);
static const unsigned int mt_umask = ~mt_lmask & (((uint64_t)1 << MT_19937_W) - 1);

/*
    Mersenne Twister as implemented in MT19937
*/
int mt_srand(int seed) {
    mt_idx = MT_19937_N;
    rseed = seed;

    MT[0] = seed;
    for (int i = 1; i < MT_19937_N - 1; i++) {
        MT[i] = (MT_19937_F * (MT[i - 1] ^ (MT[i - 1] >> (MT_19937_W - 2))) + i) &
                (((uint64_t)1 << MT_19937_W) - 1);
    }

    return rseed;
}

int mt_rand() {
    if (mt_idx >= MT_19937_N) {
        if (mt_idx > MT_19937_N) {
            panic("MT rand attempted without seed!\n");
        }
        mt_twist();
    }

    int rand = MT[mt_idx];
    rand = rand ^ ((rand >> MT_19937_U) & MT_19937_D);
    rand = rand ^ ((rand << MT_19937_S) & MT_19937_B);
    rand = rand ^ ((rand << MT_19937_T) & MT_19937_C);
    rand = rand ^ (rand >> MT_19937_L);

    mt_idx++;

    return rand & (((uint64_t)1 << MT_19937_W) - 1);
}

void mt_twist() {
    for (int i = 0; i < MT_19937_N - 1; i++) {
        int x = (MT[i] & mt_umask) + (MT[(i + 1) % MT_19937_N] & mt_lmask);
        int xA = x >> 1;
        if (x & 1) xA = xA ^ MT_19937_A;
        MT[i] = MT[(i + MT_19937_M) % MT_19937_N] ^ xA;
    }
    mt_idx = 0;
}