#include "va_test.h"

int va_test0() { return va_test2(3, 11); }
int va_test1(int a) { return va_test2(3, a); }
int va_test2(int a, int b) { return a + b; }
