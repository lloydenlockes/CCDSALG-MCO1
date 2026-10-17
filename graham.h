#ifndef GRAHAM_H
#define GRAHAM_H

#define MAX_N 32768   /* 2^15 : STRICT REQUIREMENT #1 and #3 */

typedef struct { double x, y; } Point;

/* ---------- Stack (array based, capacity MAX_N) ---------- */
typedef struct {
    Point items[MAX_N];
    int top;               /* index of top element, -1 when empty */
} Stack;

void  CREATE(Stack *S);
int   ISEMPTY(const Stack *S);
int   ISFULL(const Stack *S);
void  PUSH(Stack *S, Point elem);
Point POP(Stack *S);
Point TOP(const Stack *S);
Point NEXT_TO_TOP(const Stack *S);

/* ---------- Graham's scan ----------
   pts : input points (n of them, modified/reordered)
   hull: output array (size >= n); returns m = number of hull points,
         hull[0] is the anchor, followed by CCW order.            */
int graham_scan_slow(Point *pts, int n, Point *hull);  /* insertion sort */
int graham_scan_fast(Point *pts, int n, Point *hull);  /* merge sort     */

#endif
