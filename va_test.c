// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3

#include "va_test.h"

int va_test1(int a) { return va_test2(3, a); }
int va_test2(int a, int b) { return a + b; }
