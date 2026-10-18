#include <stdio.h>

/* Timing without any extra header: clock() lives in the C runtime library, so we
   only declare its prototype here instead of including <time.h>.
   clock() returns processor time in ticks */
extern long clock(void);
#define TICKS_PER_SEC 1000.0
#include "graham.h"

/* ===================== Stack ===================== */
void  CREATE(Stack *S)            { S->top = -1; }
int   ISEMPTY(const Stack *S)     { return S->top == -1; }
int   ISFULL(const Stack *S)      { return S->top == MAX_N - 1; }
static void stack_error(const char *msg) { fprintf(stderr, "Stack error: %s\n", msg); }

void  PUSH(Stack *S, Point e) {
    if (ISFULL(S)) { stack_error("overflow"); return; }
    S->items[++S->top] = e;
}
Point POP(Stack *S) {
    Point none = {0.0, 0.0};
    if (ISEMPTY(S)) { stack_error("underflow"); return none; }
    return S->items[S->top--];
}
Point TOP(const Stack *S) {
    Point none = {0.0, 0.0};
    if (ISEMPTY(S)) { stack_error("TOP on empty stack"); return none; }
    return S->items[S->top];
}
Point NEXT_TO_TOP(const Stack *S) {
    Point none = {0.0, 0.0};
    if (S->top < 1) { stack_error("NEXT-TO-TOP needs 2 elements"); return none; }
    return S->items[S->top - 1];
}

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
   Because P0 has the lowest y (then lowest x), all angles lie in [0, pi),
   so the cross product gives a valid total order (no atan2 needed).
   Equal angle -> closer point first. */
static int cmp_angle(Point a, Point b) {
    double c = cross(P0, a, b);
    if (c > 0) return -1;
    if (c < 0) return  1;
    double da = dist2(P0, a), db = dist2(P0, b);
    return (da < db) ? -1 : (da > db) ? 1 : 0;
}

/* ===================== "Slow" sort: insertion sort O(n^2) ===================== */
static void insertion_sort(Point *a, int n) {
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

/* ===================== "Fast" sort: merge sort O(n log n) ===================== */
static Point tmp_buf[MAX_N];

static void merge(Point *a, int lo, int mid, int hi) {
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi)
        tmp_buf[k++] = (cmp_angle(a[i], a[j]) <= 0) ? a[i++] : a[j++];
    while (i <= mid) tmp_buf[k++] = a[i++];
    while (j <= hi)  tmp_buf[k++] = a[j++];
    for (k = lo; k <= hi; k++) a[k] = tmp_buf[k];
}
static void merge_sort_rec(Point *a, int lo, int hi) {
    if (lo >= hi) return;
    int mid = lo + (hi - lo) / 2;
    merge_sort_rec(a, lo, mid);
    merge_sort_rec(a, mid + 1, hi);
    merge(a, lo, mid, hi);
}
static void merge_sort(Point *a, int n) { merge_sort_rec(a, 0, n - 1); }

/* ===================== Graham's scan ===================== */
static Stack S;       /* static: too large for the call stack */
static Point work[MAX_N];

static int graham_scan(Point *pts, int n, Point *hull, void (*sort_fn)(Point *, int),
                       const char *label) {
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
    sort_fn(work, k);

    /* 4. among points with the same angle keep only the farthest one */
    int q = 0;
    for (int i = 0; i < k; i++)
        if (i == k - 1 || cross(P0, work[i], work[i + 1]) != 0)
            work[q++] = work[i];

    /* 5. scan */
    int m = 0;
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
    m = S.top + 1;
    for (int i = 0; i < m; i++) hull[i] = S.items[i];

    long t_end = clock();
    double ms = 1000.0 * (double)(t_end - t_start) / TICKS_PER_SEC;
    printf("[%s] n = %d, hull points = %d, elapsed time = %.3f ms\n",
           label, n, m, ms);
    return m;
}

int graham_scan_slow(Point *pts, int n, Point *hull) {
    return graham_scan(pts, n, hull, insertion_sort, "Graham scan + insertion sort");
}
int graham_scan_fast(Point *pts, int n, Point *hull) {
    return graham_scan(pts, n, hull, merge_sort, "Graham scan + merge sort");
}
