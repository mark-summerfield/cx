// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "vaargs.h"

#define va_test(...) CONC(va_test, NARGS(__VA_ARGS__))(__VA_ARGS__)
int va_test1(int);
int va_test2(int, int);
