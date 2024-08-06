// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "va_test.h"
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool va_tests();

int main() {
    int total = 0;
    int ok = 0;

    total++;
    if (va_tests())
        ok++;

    printf("%s %d/%d\n", (ok == total) ? "OK" : "FAIL", ok, total);
}

bool va_tests() {
    /*
    int j = va_test();
    if (j != 14) {
        err(EXIT_FAILURE, "FAIL: va_test() expecte 14 go %d\n", j);
    }
    */
    int i = va_test(5);
    if (i != 8) {
        err(EXIT_FAILURE, "FAIL: va_test() expecte 8 go %d\n", i);
    }
    i = va_test(2, 9);
    if (i != 11) {
        err(EXIT_FAILURE, "FAIL: va_test() expecte 11 go %d\n", i);
    }
    return true;
}
