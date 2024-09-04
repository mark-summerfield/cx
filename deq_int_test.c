// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "deq_int_test.h"
#include "deq_int.h"
#include <stdio.h>

static void check_size_and_contents(tinfo* tinfo, DeqInt* deq, int size,
                                    int* ints);

void deq_int_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    DeqInt deq = deq_int_alloc();
    check_size_and_contents(tinfo, &deq, 0, NULL);

    int a[100] = {0};
    for (int i = 1; i < 6; ++i) {
        a[i - 1] = i;
        deq_int_push(&deq, i);
        check_size_and_contents(tinfo, &deq, i, a);
    }
    // TODO deq_int_push_first
    // TODO deq_int_pop
    // TODO deq_int_pop_first

    deq_int_clear(&deq);
    check_size_and_contents(tinfo, &deq, 0, NULL);
    deq_int_free(&deq);
}

static void check_size_and_contents(tinfo* tinfo, DeqInt* deq, int size,
                                    int* ints) {
    check_bool_eq(tinfo, size == 0, deq_int_isempty(deq));
    check_int_eq(tinfo, size, deq_int_size(deq));
    if (size) {
        int i = 0;
        for (DeqIntNode* node = deq->head; node; node = node->next) {
            printf("check_size_and_contents i=%d %d %p\n", i, node->value,
                   node);
            tinfo->total++;
            if (i == size) {
                fprintf(
                    stderr,
                    "FAIL: %s check_size_and_contents: too few values\n",
                    tinfo->tag);
                return;
            } else
                tinfo->ok++;
            tinfo->total++;
            if (ints[i] != node->value) {
                fprintf(stderr,
                        "FAIL: %s check_size_and_contents: %d != %d\n",
                        tinfo->tag, ints[i], node->value);
                return;
            } else
                tinfo->ok++;
            i++;
        }
    }
}
