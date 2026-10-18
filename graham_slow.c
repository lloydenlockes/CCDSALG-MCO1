#include <stdio.h>
#include "graham_slow.h"

/* Timing without any extra header: clock() lives in the C runtime library, so we
   only declare its prototype here instead of including <time.h>.
   (Ticks per second: 1000000 on Linux/macOS, 1000 on Windows.) */
extern long clock(void);
#ifdef _WIN32
#define TICKS_PER_SEC 1000.0
#else
#define TICKS_PER_SEC 1000000.0
#endif

/* ===================== Geometry helpers ===================== */
/* cross product of (b-a) x (c-a): >0 means a->b->c is a left (CCW) turn */
static double cross(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}
static double dist2(Point a, Point b) {
    double dx = a.x - b.x, dy = a.y - b.y;
    return dx * dx + dy * dy;
}

static Point P0;   /* anchor used by the comparison */

/* Returns <0 if a comes before b in polar-angle order around P0.
   P0 has the lowest y (then lowest x), so all angles lie in [0, pi) and the
   cross product gives a valid total order (no atan2 needed).
   Equal angle -> closer point first. */
static int cmp_angle(Point a, Point b) {
    double c = cross(P0, a, b);
    if (c > 0) return -1;
    if (c < 0) return  1;
    double da = dist2(P0, a), db = dist2(P0, b);
    return (da < db) ? -1 : (da > db) ? 1 : 0;
}

/* ===================== "Slow" sort: insertion sort O(n^2) ===================== */
static void sort_points(Point *a, int n) {
    for (int i = 1; i < n; i++) {
        Point key = a[i];
        int j = i - 1;
        while (j >= 0 && cmp_angle(a[j], key) > 0) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

/* ===================== Graham's scan ===================== */
static Stack S;            /* static: too large for the call stack */
static Point work[MAX_N];

int graham_scan_slow(Point *pts, int n, Point *hull) {
    if (n <= 0) return 0;

    long t_start = clock();

    /* 1. find anchor: lowest y, ties -> lowest x */
    int ai = 0;
    for (int i = 1; i < n; i++)
        if (pts[i].y < pts[ai].y || (pts[i].y == pts[ai].y && pts[i].x < pts[ai].x))
            ai = i;
    P0 = pts[ai];

    /* 2. collect all other points, dropping duplicates of the anchor */
    int k = 0;
    for (int i = 0; i < n; i++)
        if (!(pts[i].x == P0.x && pts[i].y == P0.y))
            work[k++] = pts[i];

    /* 3. sort by polar angle around the anchor */
    sort_points(work, k);

    /* 4. among points with the same angle keep only the farthest one */
    int q = 0;
    for (int i = 0; i < k; i++)
        if (i == k - 1 || cross(P0, work[i], work[i + 1]) != 0)
            work[q++] = work[i];

    /* 5. scan */
    CREATE(&S);
    PUSH(&S, P0);
    if (q >= 1) PUSH(&S, work[0]);
    if (q >= 2) {
        PUSH(&S, work[1]);
        for (int i = 2; i < q; i++) {
            while (S.top >= 1 && cross(NEXT_TO_TOP(&S), TOP(&S), work[i]) <= 0)
                POP(&S);
            PUSH(&S, work[i]);
        }
    }

    /* 6. copy stack bottom->top = anchor first, CCW order */
    int m = S.top + 1;
    for (int i = 0; i < m; i++) hull[i] = S.items[i];

    long t_end = clock();
    double ms = 1000.0 * (double)(t_end - t_start) / TICKS_PER_SEC;
    printf("[Graham scan + insertion sort] n = %d, hull points = %d, elapsed time = %.3f ms\n", n, m, ms);
    return m;
}
