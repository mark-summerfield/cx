// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Returns true if double a is effectively zero.
bool is_zeroish(double a);

// Returns true if doubles a and b are effectively equal.
bool is_equalish(double a, double b);

// Returns a double converted from s and sets ok (if not NULL) to true;
// otherwise returns zero and sets ok (if not NULL) to false.
double str_to_real(const char* s, bool* ok);

//% Just call min(x, y) and let the generic macro choose one of these:
int mx_mini(int a, int b);
int mx_maxi(int a, int b);
int64_t mx_minll(int64_t a, int64_t b);
int64_t mx_maxll(int64_t a, int64_t b);
size_t mx_minzu(size_t a, size_t b);
size_t mx_maxzu(size_t a, size_t b);
double mx_mind(double a, double b);
double mx_maxd(double a, double b);

//% Just call clamp(minval, val, maxval) and let the generic macro choose
//% one of these:
int mx_clampi(int minval, int val, int maxval);
int64_t mx_clampll(int64_t minval, int64_t val, int64_t maxval);
size_t mx_clampzu(size_t minval, size_t val, size_t maxval);
double mx_clampd(double minval, double val, double maxval);

#define GEN_LINE(type, function) \
    type:                        \
    function

// min(a, b) where both may be int, int64_t, or size_t. See also max().
#define min(T, U)                                                      \
    _Generic((T), GEN_LINE(int, mx_mini), GEN_LINE(default, mx_minll), \
             GEN_LINE(size_t, mx_minzu), GEN_LINE(double, mx_mind))(T, U)

// max(a, b) where both may be int, int64_t, or size_t. See also min().
#define max(T, U)                                                      \
    _Generic((T), GEN_LINE(int, mx_maxi), GEN_LINE(default, mx_maxll), \
             GEN_LINE(size_t, mx_maxzu), GEN_LINE(double, mx_maxd))(T, U)

// clamp(minval, val, maxval) where all may be int, int64_t, size_t, or
// double
#define clamp(T, U, V)                                                     \
    _Generic((T), GEN_LINE(int, mx_clampi), GEN_LINE(default, mx_clampll), \
             GEN_LINE(size_t, mx_clampzu),                                 \
             GEN_LINE(double, mx_clampd))(T, U, V)
