#include <stdio.h>
#include <stdint.h>
#include "poly.h"
#include "params.h"
#include "cpucycles.h"

#define SAMPLES 10000

int main(void) {
    poly p;
    uint8_t msg[KYBER_INDCPA_MSGBYTES];
    uint64_t timings_near_zero[SAMPLES];
    uint64_t timings_near_half[SAMPLES];

    // Group A: coefficients near 0
    for (int s = 0; s < SAMPLES; s++) {
        for (int i = 0; i < KYBER_N; i++) {
            p.coeffs[i] = (i % 7); // small values near 0
        }
        uint64_t start = cpucycles();
        poly_tomsg(msg, &p);
        uint64_t end = cpucycles();
        timings_near_zero[s] = end - start;
    }

    // Group B: coefficients near q/2
    for (int s = 0; s < SAMPLES; s++) {
        for (int i = 0; i < KYBER_N; i++) {
            p.coeffs[i] = (KYBER_Q / 2) + (i % 7); // near q/2
        }
        uint64_t start = cpucycles();
        poly_tomsg(msg, &p);
        uint64_t end = cpucycles();
        timings_near_half[s] = end - start;
    }

    // Dump raw timings to files for later stats in Python
    FILE *fa = fopen("group_a.csv", "w");
    FILE *fb = fopen("group_b.csv", "w");
    for (int s = 0; s < SAMPLES; s++) {
        fprintf(fa, "%llu\n", (unsigned long long)timings_near_zero[s]);
        fprintf(fb, "%llu\n", (unsigned long long)timings_near_half[s]);
    }
    fclose(fa);
    fclose(fb);

    printf("Done. Wrote group_a.csv and group_b.csv\n");
    return 0;
}
