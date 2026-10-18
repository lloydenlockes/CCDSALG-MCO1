/* Generates test_<n>.txt for n = 2^6 ... 2^15 (no stdlib.h: own random generator) */
#include <stdio.h>


static unsigned int state = 12345;

/* 32-bit LCG; returns a double in [0,1) */
static double rnd(void) {
    state = state * 1664525 + 1013904223;
    return state / 4294967296.0;      /* / 2^32 */
}

int main(void) {
    for (int n = 64; n <= 32768; n = n * 2) {   /* 64, 128, 256, ..., 32768 */
        char name[64];
        sprintf(name, "test_%d.txt", n);        /* e.g. "test_256.txt" */

        FILE *f = fopen(name, "w");
        if (!f) return 1;

        fprintf(f, "%d\n", n);                  /* first line: number of points */
        for (int i = 0; i < n; i++)             /* then n lines of "x y" */
            fprintf(f, "%.6f %.6f\n", rnd() * 2000 - 1000, rnd() * 2000 - 1000);

        fclose(f);
    }
    return 0;
}
