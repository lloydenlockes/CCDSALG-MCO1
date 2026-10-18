/* Generates test_<n>.txt for n = 2^6 ... 2^15 (no stdlib.h: own random generator) */
#include <stdio.h>


static unsigned int rng = 12345;



static double rnd(void) {
    rng = rng * 1664525 + 1013904223;
    return rng / 4294967296.0;      
}

int main(void) {
    for (int n = 64; n <= 32768; n = n * 2) {  
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
