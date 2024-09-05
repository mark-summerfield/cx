// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "deq_str_test.h"
#include "deq_str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void check_size_and_contents(tinfo* tinfo, DeqStr* deq,
                                    const char* expected, const char* first,
                                    const char* last);

void deq_str_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    DeqStr deq1 = deq_str_alloc(OWNS);
    check_size_and_contents(tinfo, &deq1, "", "", "");
    check_isnull(tinfo, deq_str_first(&deq1));
    check_isnull(tinfo, deq_str_last(&deq1));

    deq_str_push(&deq1, strdup("a"));
    check_size_and_contents(tinfo, &deq1, "a", "a", "a");
    deq_str_push(&deq1, strdup("bc"));
    check_size_and_contents(tinfo, &deq1, "a|bc", "a", "bc");
    deq_str_push(&deq1, strdup("def"));
    check_size_and_contents(tinfo, &deq1, "a|bc|def", "a", "def");
    deq_str_push(&deq1, strdup("ghij"));
    check_size_and_contents(tinfo, &deq1, "a|bc|def|ghij", "a", "ghij");
    deq_str_push(&deq1, strdup("klmno"));
    check_size_and_contents(tinfo, &deq1, "a|bc|def|ghij|klmno", "a",
                            "klmno");

    char* value = deq_str_pop(&deq1);
    check_str_eq(tinfo, value, "klmno");
    free(value);
    value = deq_str_pop(&deq1);
    check_str_eq(tinfo, value, "ghij");
    free(value);
    check_size_and_contents(tinfo, &deq1, "a|bc|def", "a", "def");

    deq_str_push_first(&deq1, strdup("X"));
    deq_str_push_first(&deq1, strdup("YZ"));
    check_size_and_contents(tinfo, &deq1, "YZ|X|a|bc|def", "YZ", "def");

    deq_str_push(&deq1, strdup("@"));
    deq_str_push(&deq1, strdup("##"));
    check_size_and_contents(tinfo, &deq1, "YZ|X|a|bc|def|@|##", "YZ", "##");

    deq_str_clear(&deq1);
    check_size_and_contents(tinfo, &deq1, "", "", "");
    check_isnull(tinfo, deq_str_first(&deq1));
    check_isnull(tinfo, deq_str_last(&deq1));
    deq_str_free(&deq1);

    DeqStr deq2 = deq_str_alloc(BORROWS);
    check_size_and_contents(tinfo, &deq2, "", "", "");
    check_isnull(tinfo, deq_str_first(&deq2));
    check_isnull(tinfo, deq_str_last(&deq2));

    deq_str_push(&deq2, "a");
    check_size_and_contents(tinfo, &deq2, "a", "a", "a");
    deq_str_push(&deq2, "bc");
    check_size_and_contents(tinfo, &deq2, "a|bc", "a", "bc");
    deq_str_push(&deq2, "def");
    check_size_and_contents(tinfo, &deq2, "a|bc|def", "a", "def");
    deq_str_push(&deq2, "ghij");
    check_size_and_contents(tinfo, &deq2, "a|bc|def|ghij", "a", "ghij");
    deq_str_push(&deq2, "klmno");
    check_size_and_contents(tinfo, &deq2, "a|bc|def|ghij|klmno", "a",
                            "klmno");

    value = deq_str_pop(&deq2);
    check_str_eq(tinfo, value, "klmno");
    value = deq_str_pop(&deq2);
    check_str_eq(tinfo, value, "ghij");
    check_size_and_contents(tinfo, &deq2, "a|bc|def", "a", "def");

    deq_str_push_first(&deq2, "X");
    deq_str_push_first(&deq2, "YZ");
    check_size_and_contents(tinfo, &deq2, "YZ|X|a|bc|def", "YZ", "def");

    deq_str_push(&deq2, "@");
    deq_str_push(&deq2, "##");
    check_size_and_contents(tinfo, &deq2, "YZ|X|a|bc|def|@|##", "YZ", "##");

    deq_str_clear(&deq2);
    check_size_and_contents(tinfo, &deq2, "", "", "");
    check_isnull(tinfo, deq_str_first(&deq2));
    check_isnull(tinfo, deq_str_last(&deq2));
    deq_str_free(&deq2);
}

static void check_size_and_contents(tinfo* tinfo, DeqStr* deq,
                                    const char* expected, const char* first,
                                    const char* last) {
    bool empty = !strlen(expected);
    check_bool_eq(tinfo, empty, deq_str_isempty(deq));
    if (!deq_str_isempty(deq)) {
        check_str_eq(tinfo, first, deq_str_first(deq));
        check_str_eq(tinfo, last, deq_str_last(deq));
        char* actual = deq_str_join(deq, "|");
        check_str_eq(tinfo, actual, expected);
        free(actual);
    } else
        check_str_eq(tinfo, "", expected);
}
