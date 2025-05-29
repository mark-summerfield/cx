// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3
#pragma once

#include "vaargs.h"

#define va_test(...) CONC(va_test, NARGS(__VA_ARGS__))(__VA_ARGS__)
int va_test1(int);
int va_test2(int, int);
