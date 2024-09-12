// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define COMMA_I64_SIZE 28

bool is_zeroish(double a);

bool is_equalish(double a, double b);

// s should be COMMA_I64_SIZE
void commas(char* s, int64_t n);

int mx_mini(int a, int b);
int mx_maxi(int a, int b);
int64_t mx_minll(int64_t a, int64_t b);
int64_t mx_maxll(int64_t a, int64_t b);
size_t mx_minzu(size_t a, size_t b);
size_t mx_maxzu(size_t a, size_t b);
double mx_mind(double a, double b);
double mx_maxd(double a, double b);

#define GEN_LINE(type, function) \
    type:                        \
    function

#define min(T, U)                                                      \
    _Generic((T), GEN_LINE(int, mx_mini), GEN_LINE(default, mx_minll), \
             GEN_LINE(size_t, mx_minzu), GEN_LINE(double, mx_mind))(T, U)

#define max(T, U)                                                      \
    _Generic((T), GEN_LINE(int, mx_maxi), GEN_LINE(default, mx_maxll), \
             GEN_LINE(size_t, mx_maxzu), GEN_LINE(double, mx_maxd))(T, U)
