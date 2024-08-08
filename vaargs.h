#pragma once

/* 1+ args example:
    #define myfunc(...) CONC(myfunc, NARGS(__VA_ARGS__))(__VA_ARGS__)
    int myfunc1(int a) { return myfunc2(3, a); }
    int myfunc2(int a, int b) { return a + b; }
*/

#define NARGS(...) NARGS_(__VA_ARGS__, 5, 4, 3, 2, 1, 0)
#define NARGS_(_5, _4, _3, _2, _1, N, ...) N

#define CONC(A, B) CONC_(A, B)
#define CONC_(A, B) A##B
