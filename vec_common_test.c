// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "vec_common_test.h"
#include <stdio.h>

void vec_check_found(tinfo* tinfo, const vec_found_index* i1,
                     const vec_found_index* i2) {
    tinfo->total++;
    if ((i1->found == i2->found) &&
        (!i1->found || (i1->index == i2->index))) {
        tinfo->ok++;
    } else {
        fprintf(stderr, "FAIL: %s vec_search()\n", tinfo->tag);
    }
}
