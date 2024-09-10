// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "mx.h"
#include <math.h>

#define ACCURACY 0.0000000001

inline bool is_zeroish(double a) { return fabs(a) < ACCURACY; }

inline bool is_equalish(double a, double b) {
    return fabs(a - b) < ACCURACY;
}
