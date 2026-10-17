#include <stdio.h>
#include "graham.h"

static Point pts[MAX_N];
static Point hull[MAX_N];

int main(void) {
    char inname[256], outname[256];
    int n;

    printf("Enter input filename (with extension): ");
    if (scanf("%255s", inname) != 1) return 1;
    printf("Enter output filename (with extension): ");
    if (scanf("%255s", outname) != 1) return 1;

    FILE *fin = fopen(inname, "r");
    if (!fin) { perror("Cannot open input file"); return 1; }

    if (fscanf(fin, "%d", &n) != 1 || n < 1 || n > MAX_N) {
        fprintf(stderr, "Invalid n (must be 1..%d)\n", MAX_N);
        fclose(fin);
        return 1;
    }
    for (int i = 0; i < n; i++) {
        if (fscanf(fin, "%lf %lf", &pts[i].x, &pts[i].y) != 2) {
            fprintf(stderr, "Bad data at point %d\n", i + 1);
            fclose(fin);
            return 1;
        }
    }
    fclose(fin);

    int m = graham_scan_slow(pts, n, hull);

    FILE *fout = fopen(outname, "w");
    if (!fout) { perror("Cannot open output file"); return 1; }
    fprintf(fout, "%d\n", m);
    for (int i = 0; i < m; i++)
        fprintf(fout, "%.6f %.6f\n", hull[i].x, hull[i].y);
    fclose(fout);
    return 0;
}
