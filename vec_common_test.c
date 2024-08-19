// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_common_test.h"
#include <stdio.h>

void vec_check_found(tinfo* tinfo, const ptrdiff_t i1, const ptrdiff_t i2) {
    tinfo->total++;
    if (i1 == i2) {
        tinfo->ok++;
    } else {
        fprintf(stderr, "FAIL: %s vec_{find,search}() %td != %td\n",
                tinfo->tag, i1, i2);
    }
}
