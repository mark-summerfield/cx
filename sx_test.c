#include "cx_util_test.h"
#include "sx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void sx_test_uppercase_alloc(tinfo*);
void sx_test_uppercase_ip(tinfo*);
void sx_test_lowercase_alloc(tinfo*);
void sx_test_lowercase_ip(tinfo*);
void sx_test_begins_ends(tinfo*);
void sx_test_comparisons(tinfo*);

void sx_tests(tinfo* tinfo) {
    sx_test_uppercase_alloc(tinfo);
    sx_test_uppercase_ip(tinfo);
    sx_test_lowercase_alloc(tinfo);
    sx_test_lowercase_ip(tinfo);
    sx_test_begins_ends(tinfo);
    sx_test_comparisons(tinfo);
}

void sx_test_uppercase_alloc(tinfo* tinfo) {
    const char* original = "this is a test";
    const char* expected = "THIS IS A TEST";
    char* actual = sx_uppercase_alloc(original);
    check_str_eq(tinfo, expected, actual);
    free(actual);
}

void sx_test_uppercase_ip(tinfo* tinfo) {
    char original[] = "this is a test";
    const char* expected = "THIS IS A TEST";
    sx_uppercase_ip(original);
    check_str_eq(tinfo, expected, original);
}

void sx_test_lowercase_alloc(tinfo* tinfo) {
    const char* original = "THIS IS A TEST";
    const char* expected = "this is a test";
    char* actual = sx_lowercase_alloc(original);
    check_str_eq(tinfo, expected, actual);
    free(actual);
}

void sx_test_lowercase_ip(tinfo* tinfo) {
    char original[] = "THIS IS A TEST";
    const char* expected = "this is a test";
    sx_lowercase_ip(original);
    check_str_eq(tinfo, expected, original);
}

void sx_test_begins_ends(tinfo* tinfo) {
    tinfo->total++;
    if (!sx_begins("alpha", "alp")) {
        fprintf(stderr, "FAIL: %s !sx_begins(\"alpha\", \"alp\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (sx_begins("alpha", "scalp")) {
        fprintf(stderr, "FAIL: %s sx_begins(\"alpha\", \"scalp\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (!sx_ends("beta", "eta")) {
        fprintf(stderr, "FAIL: %s !sx_ends(\"beta\", \"eta\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (sx_ends("beta", "meta")) {
        fprintf(stderr, "FAIL: %s sx_ends(\"beta\", \"meta\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (!sx_begins_fold("alpha", "ALP")) {
        fprintf(stderr, "FAIL: %s !sx_begins_fold(\"alpha\", \"ALP\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (sx_begins_fold("alpha", "SCALP")) {
        fprintf(stderr, "FAIL: %s sx_begins_fold(\"alpha\", \"SCALP\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (!sx_ends_fold("beta", "ETA")) {
        fprintf(stderr, "FAIL: %s !sx_ends_fold(\"beta\", \"ETA\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (sx_ends_fold("beta", "META")) {
        fprintf(stderr, "FAIL: %s sx_ends_fold(\"beta\", \"META\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
}

void sx_test_comparisons(tinfo* tinfo) {
    tinfo->total++;
    if (!sx_eq("bravo", "bravo")) {
        fprintf(stderr, "FAIL: %s !sx_eq(\"bravo\", \"bravo\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    if (!sx_eq_fold("BRAVO", "bravo")) {
        fprintf(stderr, "FAIL: %s !sx_eq_fold(\"BRAVO\", \"bravo\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    if (!sx_lt("bravo", "charlie")) {
        fprintf(stderr, "FAIL: %s !sx_lt(\"bravo\", \"charlie\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    if (!sx_lt_fold("bravo", "CHARLIE")) {
        fprintf(stderr, "FAIL: %s !sx_lt_fold(\"bravo\", \"CHARLIE\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    if (!sx_le("bravo", "charlie")) {
        fprintf(stderr, "FAIL: %s !sx_le(\"bravo\", \"charlie\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    if (!sx_le_fold("bravo", "CHARLIE")) {
        fprintf(stderr, "FAIL: %s !sx_le_fold(\"bravo\", \"CHARLIE\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    if (!sx_le("charlie", "charlie")) {
        fprintf(stderr, "FAIL: %s !sx_le(\"charlie\", \"charlie\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    if (!sx_le_fold("CHARLIE", "CHARLIE")) {
        fprintf(stderr, "FAIL: %s !sx_le_fold(\"CHARLIE\", \"CHARLIE\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    if (sx_gt("bravo", "charlie")) {
        fprintf(stderr, "FAIL: %s !sx_gt(\"bravo\", \"charlie\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    if (sx_gt_fold("bravo", "CHARLIE")) {
        fprintf(stderr, "FAIL: %s !sx_gt_fold(\"bravo\", \"CHARLIE\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    if (sx_ge("bravo", "charlie")) {
        fprintf(stderr, "FAIL: %s !sx_ge(\"bravo\", \"charlie\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    if (sx_ge_fold("bravo", "CHARLIE")) {
        fprintf(stderr, "FAIL: %s !sx_ge_fold(\"bravo\", \"CHARLIE\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    if (!sx_ge("charlie", "charlie")) {
        fprintf(stderr, "FAIL: %s !sx_ge(\"charlie\", \"charlie\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
    tinfo->total++;
    if (!sx_ge_fold("CHARLIE", "CHARLIE")) {
        fprintf(stderr, "FAIL: %s !sx_ge_fold(\"CHARLIE\", \"CHARLIE\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
}
