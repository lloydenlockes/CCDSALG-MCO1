/* Generates test_<n>.txt for n = 2^6 ... 2^15 (no stdlib.h: own random generator) */
#include <stdio.h>

static unsigned long long state = 12345ULL;

/* 64-bit LCG; returns a double in [0,1) */
static double rnd(void) {
    state = state * 6364136223846793005ULL + 1442695040888963407ULL;
    return (double)(state >> 11) / 9007199254740992.0;   /* / 2^53 */
}

int main(void) {
    for (int e = 6; e <= 15; e++) {
        int n = 1 << e;
        char name[64];
        sprintf(name, "test_%d.txt", n);
        FILE *f = fopen(name, "w");
        if (!f) return 1;
        fprintf(f, "%d\n", n);
        for (int i = 0; i < n; i++)
            fprintf(f, "%.6f %.6f\n", rnd() * 2000 - 1000, rnd() * 2000 - 1000);
        fclose(f);
    }
    return 0;
}
