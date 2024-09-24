// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "deq_int_test.h"
#include "deq_int.h"
#include "deqs.h"
#include "exit.h"
#include <stdio.h>

static void check_size_and_contents(tinfo* tinfo, DeqInt* deq, int size,
                                    int* ints);

void deq_int_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    DeqInt deq = deq_int_alloc();
    check_size_and_contents(tinfo, &deq, 0, NULL);

    int a[20] = {0};
    for (int i = 1; i < 11; ++i) {
        a[i - 1] = i;
        deq_int_push(&deq, i);
        check_size_and_contents(tinfo, &deq, i, a);
    }
    check_size_and_contents(tinfo, &deq, 10, a);

    int x = deq_int_pop(&deq);
    check_int_eq(tinfo, x, 10);
    x = deq_int_pop(&deq);
    check_int_eq(tinfo, x, 9);
    x = deq_int_pop_first(&deq);
    check_int_eq(tinfo, x, 1);
    x = deq_int_pop_first(&deq);
    check_int_eq(tinfo, x, 2);
    check_size_and_contents(tinfo, &deq, 6, &a[2]);

    deq_int_push_first(&deq, 10);
    deq_int_push_first(&deq, 20);
    a[0] = 20;
    a[1] = 10;
    check_size_and_contents(tinfo, &deq, 8, a);
    deq_int_push(&deq, 90);
    deq_int_push(&deq, 100);
    a[8] = 90;
    a[9] = 100;
    check_size_and_contents(tinfo, &deq, 10, a);

    deq_int_clear(&deq);
    check_size_and_contents(tinfo, &deq, 0, NULL);
    deq_int_free(&deq);
}

static void check_size_and_contents(tinfo* tinfo, DeqInt* deq, int size,
                                    int* ints) {
    check_bool_eq(tinfo, size == 0, DEQ_ISEMPTY(deq));
    check_int_eq(tinfo, size, DEQ_SIZE(deq));
    if (size) {
        tinfo->total++;
        if (ints[0] != deq_int_first(deq))
            WARN("FAIL: %s check_size_and_contents: wrong first\n",
                 tinfo->tag);
        else
            tinfo->ok++;
        tinfo->total++;
        if (ints[size - 1] != deq_int_last(deq))
            WARN("FAIL: %s check_size_and_contents: wrong last\n",
                 tinfo->tag);
        else
            tinfo->ok++;
        int i = 0;
        for (DeqIntNode* node = deq->head; node; node = node->next) {
            tinfo->total++;
            if (i == size) {
                WARN("FAIL: %s check_size_and_contents: too few values\n",
                     tinfo->tag);
                return;
            } else
                tinfo->ok++;
            tinfo->total++;
            if (ints[i] != node->value) {
                WARN("FAIL: %s check_size_and_contents: %d != %d\n",
                     tinfo->tag, ints[i], node->value);
                return;
            } else
                tinfo->ok++;
            i++;
        }
    }
}
