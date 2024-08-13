#include "cx_util_test.h"
#include "sx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sx_test_new_uppercase(counts_pair*);
void sx_test_uppercase_ip(counts_pair*);
void sx_test_new_lowercase(counts_pair*);
void sx_test_lowercase_ip(counts_pair*);
void sx_test_begins_ends(counts_pair*);
void sx_test_comparisons(counts_pair*);

void sx_tests(counts_pair* counts) {
    sx_test_new_uppercase(counts);
    sx_test_uppercase_ip(counts);
    sx_test_new_lowercase(counts);
    sx_test_lowercase_ip(counts);
    sx_test_begins_ends(counts);
    sx_test_comparisons(counts);
}

void sx_test_new_uppercase(counts_pair* counts) {
    const char* original = "this is a test";
    const char* expected = "THIS IS A TEST";
    char* actual = sx_new_uppercase(original);
    check_str_eq(counts, expected, actual);
    free(actual);
}

void sx_test_uppercase_ip(counts_pair* counts) {
    char original[] = "this is a test";
    const char* expected = "THIS IS A TEST";
    sx_uppercase_ip(original);
    check_str_eq(counts, expected, original);
}

void sx_test_new_lowercase(counts_pair* counts) {
    const char* original = "THIS IS A TEST";
    const char* expected = "this is a test";
    char* actual = sx_new_lowercase(original);
    check_str_eq(counts, expected, actual);
    free(actual);
}

void sx_test_lowercase_ip(counts_pair* counts) {
    char original[] = "THIS IS A TEST";
    const char* expected = "this is a test";
    sx_lowercase_ip(original);
    check_str_eq(counts, expected, original);
}

void sx_test_begins_ends(counts_pair* counts) {
    counts->total++;
    if (!sx_begins("alpha", "alp")) {
        fprintf(stderr, "FAIL: !sx_begins(\"alpha\", \"alp\"\n");
    } else
        counts->ok++;

    counts->total++;
    if (sx_begins("alpha", "scalp")) {
        fprintf(stderr, "FAIL: sx_begins(\"alpha\", \"scalp\"\n");
    } else
        counts->ok++;

    counts->total++;
    if (!sx_ends("beta", "eta")) {
        fprintf(stderr, "FAIL: !sx_ends(\"beta\", \"eta\"\n");
    } else
        counts->ok++;

    counts->total++;
    if (sx_ends("beta", "meta")) {
        fprintf(stderr, "FAIL: sx_ends(\"beta\", \"meta\"\n");
    } else
        counts->ok++;

    counts->total++;
    if (!sx_begins_fold("alpha", "ALP")) {
        fprintf(stderr, "FAIL: !sx_begins_fold(\"alpha\", \"ALP\"\n");
    } else
        counts->ok++;

    counts->total++;
    if (sx_begins_fold("alpha", "SCALP")) {
        fprintf(stderr, "FAIL: sx_begins_fold(\"alpha\", \"SCALP\"\n");
    } else
        counts->ok++;

    counts->total++;
    if (!sx_ends_fold("beta", "ETA")) {
        fprintf(stderr, "FAIL: !sx_ends_fold(\"beta\", \"ETA\"\n");
    } else
        counts->ok++;

    counts->total++;
    if (sx_ends_fold("beta", "META")) {
        fprintf(stderr, "FAIL: sx_ends_fold(\"beta\", \"META\"\n");
    } else
        counts->ok++;
}

void sx_test_comparisons(counts_pair* counts) {
    counts->total++;
    if (!sx_eq("bravo", "bravo")) {
        fprintf(stderr, "FAIL: !sx_eq(\"bravo\", \"bravo\"\n");
    } else
        counts->ok++;
    counts->total++;
    if (!sx_eq_fold("BRAVO", "bravo")) {
        fprintf(stderr, "FAIL: !sx_eq_fold(\"BRAVO\", \"bravo\"\n");
    } else
        counts->ok++;
    counts->total++;
    if (!sx_lt("bravo", "charlie")) {
        fprintf(stderr, "FAIL: !sx_lt(\"bravo\", \"charlie\"\n");
    } else
        counts->ok++;
    counts->total++;
    if (!sx_lt_fold("bravo", "CHARLIE")) {
        fprintf(stderr, "FAIL: !sx_lt_fold(\"bravo\", \"CHARLIE\"\n");
    } else
        counts->ok++;
    counts->total++;
    if (!sx_le("bravo", "charlie")) {
        fprintf(stderr, "FAIL: !sx_le(\"bravo\", \"charlie\"\n");
    } else
        counts->ok++;
    counts->total++;
    if (!sx_le_fold("bravo", "CHARLIE")) {
        fprintf(stderr, "FAIL: !sx_le_fold(\"bravo\", \"CHARLIE\"\n");
    } else
        counts->ok++;
    counts->total++;
    if (!sx_le("charlie", "charlie")) {
        fprintf(stderr, "FAIL: !sx_le(\"charlie\", \"charlie\"\n");
    } else
        counts->ok++;
    counts->total++;
    if (!sx_le_fold("CHARLIE", "CHARLIE")) {
        fprintf(stderr, "FAIL: !sx_le_fold(\"CHARLIE\", \"CHARLIE\"\n");
    } else
        counts->ok++;
    counts->total++;
    if (sx_gt("bravo", "charlie")) {
        fprintf(stderr, "FAIL: !sx_gt(\"bravo\", \"charlie\"\n");
    } else
        counts->ok++;
    counts->total++;
    if (sx_gt_fold("bravo", "CHARLIE")) {
        fprintf(stderr, "FAIL: !sx_gt_fold(\"bravo\", \"CHARLIE\"\n");
    } else
        counts->ok++;
    counts->total++;
    if (sx_ge("bravo", "charlie")) {
        fprintf(stderr, "FAIL: !sx_ge(\"bravo\", \"charlie\"\n");
    } else
        counts->ok++;
    counts->total++;
    if (sx_ge_fold("bravo", "CHARLIE")) {
        fprintf(stderr, "FAIL: !sx_ge_fold(\"bravo\", \"CHARLIE\"\n");
    } else
        counts->ok++;
    counts->total++;
    if (!sx_ge("charlie", "charlie")) {
        fprintf(stderr, "FAIL: !sx_ge(\"charlie\", \"charlie\"\n");
    } else
        counts->ok++;
    counts->total++;
    if (!sx_ge_fold("CHARLIE", "CHARLIE")) {
        fprintf(stderr, "FAIL: !sx_ge_fold(\"CHARLIE\", \"CHARLIE\"\n");
    } else
        counts->ok++;
}
