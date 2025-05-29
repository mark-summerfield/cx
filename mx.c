// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3

#include "mx.h"
#include "cx.h"
#include <float.h>
#include <math.h>
#include <stdio.h>

inline bool is_zeroish(double a) {
    double x = fabs(a);
    return x < (DBL_EPSILON * x);
}

inline bool is_equalish(double a, double b) {
    return fabs(a - b) < (DBL_EPSILON * fabs(a + b));
}

double str_to_real(const char* s, bool* ok) {
    double v = 0;
    int n = sscanf(s, "%lg", &v);
    if (ok)
        *ok = n == 1;
    return v;
}

inline int mx_mini(int a, int b) { return a < b ? a : b; }

inline int mx_maxi(int a, int b) { return a > b ? a : b; }

inline int64_t mx_minll(int64_t a, int64_t b) { return a < b ? a : b; }

inline int64_t mx_maxll(int64_t a, int64_t b) { return a > b ? a : b; }

inline size_t mx_minzu(size_t a, size_t b) { return a < b ? a : b; }

inline size_t mx_maxzu(size_t a, size_t b) { return a > b ? a : b; }

inline double mx_mind(double a, double b) { return a < b ? a : b; }

inline double mx_maxd(double a, double b) { return a > b ? a : b; }

inline int mx_clampi(int minval, int val, int maxval) {
    return max(minval, min(val, maxval));
}

inline int64_t mx_clampll(int64_t minval, int64_t val, int64_t maxval) {
    return max(minval, min(val, maxval));
}

inline size_t mx_clampzu(size_t minval, size_t val, size_t maxval) {
    return max(minval, min(val, maxval));
}

inline double mx_clampd(double minval, double val, double maxval) {
    return max(minval, min(val, maxval));
}
