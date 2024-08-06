#pragma once

#include "vaargs.h"

#define va_test(...) CONC(va_test, NARGS(__VA_ARGS__))(__VA_ARGS__)
int va_test1(int);
int va_test2(int, int);

typedef struct {
    int i;
} opt_arg_int;

#define opt_test(...) opt_test_((opt_arg_int){.i = 11, __VA_ARGS__})
int opt_test_(opt_arg_int arg);
