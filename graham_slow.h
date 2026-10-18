#ifndef GRAHAM_SLOW_H
#define GRAHAM_SLOW_H

#include "stack.h"

/* Graham's scan using insertion sort (quadratic, "slow").
   pts : input points (n of them, will be reordered)
   hull: output array (size >= n)
   returns m = number of hull points; hull[0] is the anchor, then CCW order.
   Prints the elapsed time in ms. */
int graham_scan_slow(Point *pts, int n, Point *hull);

#endif
