#include <stdio.h>
#include "graham_slow.h"

static Point Points[MAX_N];
static Point HullPoints[MAX_N];

int main(void) {
    char InputFileName[256];
    char OutputFileName[256];
    int NumPoints = 0;
    int HullSize = 0;
    int Index = 0;
    FILE *InputFile = NULL;
    FILE *OutputFile = NULL;

    /*1: Get filenames from user */
    printf("Enter input filename (with extension): ");
    if (scanf("%255s", InputFileName) != 1) {
        printf("Error: Failed to read input filename.\n");
        return 1;
    }

    printf("Enter output filename (with extension): ");
    if (scanf("%255s", OutputFileName) != 1) {
        printf("Error: Failed to read output filename.\n");
        return 1;
    }

    /* 2: Open and read input text file */
    InputFile = fopen(InputFileName, "r");
    if (InputFile == NULL) {
        printf("Error: Could not open file %s for reading.\n", InputFileName);
        return 1;
    }

    if (fscanf(InputFile, "%d", &NumPoints) != 1 || NumPoints < 1 || NumPoints > MAX_N) {
        printf("Error: Invalid number of points (must be between 1 and %d).\n", MAX_N);
        fclose(InputFile);
        return 1;
    }

    for (Index = 0; Index < NumPoints; Index++) {
        if (fscanf(InputFile, "%lf %lf", &Points[Index].x, &Points[Index].y) != 2) {
            printf("Error: Bad point data at line %d.\n", Index + 2);
            fclose(InputFile);
            return 1;
        }
    }
    fclose(InputFile);

    /* 3: Run Graham Scan (Slow Version) */
    HullSize = graham_scan_slow(Points, NumPoints, HullPoints);

    /* 4: Write convex hull results to output file */
    OutputFile = fopen(OutputFileName, "w");
    if (OutputFile == NULL) {
        printf("Error: Could not open file %s for writing.\n", OutputFileName);
        return 1;
    }

    fprintf(OutputFile, "%d\n", HullSize);
    for (Index = 0; Index < HullSize; Index++) {
        /* format required: 6 digits after decimal point */
        fprintf(OutputFile, "%.6f %.6f\n", HullPoints[Index].x, HullPoints[Index].y);
    }
    fclose(OutputFile);

    printf("Process completed successfully.\n");
    return 0;
}
