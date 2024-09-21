// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vecs_test.h"
#include "exit.h"
#include <stdio.h>

void check_found(tinfo* tinfo, const int i1, const int i2) {
    tinfo->total++;
    if (i1 == i2)
        tinfo->ok++;
    else
        WARN("FAIL: %s vec_{find,search}() %d != %d\n", tinfo->tag, i1, i2);
}
