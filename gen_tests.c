/* Generates test files test_<n>.txt for n = 2^6 ... 2^15 */
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    srand(12345);
    for (int e = 6; e <= 15; e++) {
        int n = 1 << e;
        char name[64];
        sprintf(name, "test_%d.txt", n);
        FILE *f = fopen(name, "w");
        fprintf(f, "%d\n", n);
        for (int i = 0; i < n; i++)
            fprintf(f, "%.6f %.6f\n", (rand() / (double)RAND_MAX) * 2000 - 1000,
                                       (rand() / (double)RAND_MAX) * 2000 - 1000);
        fclose(f);
    }
    return 0;
}
