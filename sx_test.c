#include "cx_util_test.h"
#include "sx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void sx_test_uppercase_alloc(tinfo*);
static void sx_test_uppercase_allocn(tinfo*);
static void sx_test_uppercase_ip(tinfo*);
static void sx_test_lowercase_alloc(tinfo*);
static void sx_test_lowercase_ip(tinfo*);
static void sx_test_begins_ends(tinfo*);
static void sx_test_filename_ext(tinfo*);

void sx_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    tinfo->tag = "sx_test_uppercase_alloc";
    sx_test_uppercase_alloc(tinfo);
    tinfo->tag = "sx_test_uppercase_allocn";
    sx_test_uppercase_allocn(tinfo);
    tinfo->tag = "sx_test_uppercase_ip";
    sx_test_uppercase_ip(tinfo);
    tinfo->tag = "sx_test_lowercase_alloc";
    sx_test_lowercase_alloc(tinfo);
    tinfo->tag = "sx_test_lowercase_ip";
    sx_test_lowercase_ip(tinfo);
    tinfo->tag = "sx_test_begins_ends";
    sx_test_begins_ends(tinfo);
    tinfo->tag = "sx_test_filename_ext";
    sx_test_filename_ext(tinfo);
}

static void sx_test_uppercase_alloc(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    const char* original = "this is a test";
    const char* expected = "THIS IS A TEST";
    char* actual = sx_uppercase_alloc(original);
    check_str_eq(tinfo, expected, actual);
    free(actual);
}

static void sx_test_uppercase_allocn(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    const char* original = "this is a test of allocn";
    const char* expected = "THIS IS A TEST";
    char* actual = sx_uppercase_allocn(original, 14);
    check_str_eq(tinfo, expected, actual);
    free(actual);
}

static void sx_test_uppercase_ip(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    char original[] = "this is a test";
    const char* expected = "THIS IS A TEST";
    sx_uppercase_ip(original);
    check_str_eq(tinfo, expected, original);
}

static void sx_test_lowercase_alloc(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    const char* original = "THIS IS A TEST";
    const char* expected = "this is a test";
    char* actual = sx_lowercase_alloc(original);
    check_str_eq(tinfo, expected, actual);
    free(actual);
}

static void sx_test_lowercase_ip(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    char original[] = "THIS IS A TEST";
    const char* expected = "this is a test";
    sx_lowercase_ip(original);
    check_str_eq(tinfo, expected, original);
}

static void sx_test_begins_ends(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
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

static void sx_test_filename_ext(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    {
        const char* filename = "file.zip";
        const char* exp = "zip";
        const char* ext = sx_filename_ext(filename);
        check_str_eq(tinfo, exp, ext);
    }
    {
        const char* filename = "file.tar.gz";
        const char* exp = "gz";
        const char* ext = sx_filename_ext(filename);
        check_str_eq(tinfo, exp, ext);
    }
    {
        const char* filename = "file.";
        const char* ext = sx_filename_ext(filename);
        check_str_eq(tinfo, "", ext);
    }
    {
        const char* filename = "file";
        const char* ext = sx_filename_ext(filename);
        check_str_eq(tinfo, "", ext);
    }
    {
        const char* filename = "...";
        const char* ext = sx_filename_ext(filename);
        check_str_eq(tinfo, "", ext);
    }
}
