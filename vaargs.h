// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3
#pragma once

/* // 1+ args example (cannot be used for 0|1 i.e., optional arg)
    #define afunc(...) CONC(afunc, NARGS(__VA_ARGS__))(__VA_ARGS__)
    int afunc1(int a) { return afunc2(3, a); }
    int afunc2(int a, int b) { return a + b; }
*/

#define NARGS(...) NARGS_(__VA_ARGS__, 5, 4, 3, 2, 1, 0)
#define NARGS_(_5, _4, _3, _2, _1, N, ...) N

#define CONC(A, B) CONC_(A, B)
#define CONC_(A, B) A##B
