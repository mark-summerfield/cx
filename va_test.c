#include "va_test.h"

int opt_test_(opt_arg_int arg) { return va_test2(3, arg.i); }
int va_test1(int a) { return va_test2(3, a); }
int va_test2(int a, int b) { return a + b; }
