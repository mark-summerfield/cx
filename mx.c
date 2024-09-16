// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "mx.h"
#include "cx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ACCURACY 0.0000000001

inline bool is_zeroish(double a) { return fabs(a) < ACCURACY; }

inline bool is_equalish(double a, double b) {
    return fabs(a - b) < ACCURACY;
}

inline int mx_mini(int a, int b) { return a < b ? a : b; }

inline int mx_maxi(int a, int b) { return a > b ? a : b; }

inline int64_t mx_minll(int64_t a, int64_t b) { return a < b ? a : b; }

inline int64_t mx_maxll(int64_t a, int64_t b) { return a > b ? a : b; }

inline size_t mx_minzu(size_t a, size_t b) { return a < b ? a : b; }

inline size_t mx_maxzu(size_t a, size_t b) { return a > b ? a : b; }

inline double mx_mind(double a, double b) { return a < b ? a : b; }

inline double mx_maxd(double a, double b) { return a > b ? a : b; }
