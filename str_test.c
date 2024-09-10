// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "str_test.h"
#include "fx.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void str_test_uppercase(tinfo*);
static void str_test_uppercasen(tinfo*);
static void str_test_uppercase_ip(tinfo*);
static void str_test_lowercase(tinfo*);
static void str_test_lowercase_ip(tinfo*);
static void str_test_begins_ends(tinfo*);
static void str_test_filename_ext(tinfo*);

void str_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    tinfo->tag = "str_test_uppercase";
    str_test_uppercase(tinfo);
    tinfo->tag = "str_test_uppercasen";
    str_test_uppercasen(tinfo);
    tinfo->tag = "str_test_uppercase_ip";
    str_test_uppercase_ip(tinfo);
    tinfo->tag = "str_test_lowercase";
    str_test_lowercase(tinfo);
    tinfo->tag = "str_test_lowercase_ip";
    str_test_lowercase_ip(tinfo);
    tinfo->tag = "str_test_begins_ends";
    str_test_begins_ends(tinfo);
    tinfo->tag = "str_test_filename_ext";
    str_test_filename_ext(tinfo);
}

static void str_test_uppercase(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    const char* original = "this is a test";
    const char* expected = "THIS IS A TEST";
    char* actual = str_uppercase(original);
    check_str_eq(tinfo, expected, actual);
    free(actual);
}

static void str_test_uppercasen(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    const char* original = "this is a test of allocn";
    const char* expected = "THIS IS A TEST";
    char* actual = str_uppercasen(original, 14);
    check_str_eq(tinfo, expected, actual);
    free(actual);
}

static void str_test_uppercase_ip(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    char original[] = "this is a test";
    const char* expected = "THIS IS A TEST";
    str_uppercase_ip(original);
    check_str_eq(tinfo, expected, original);
}

static void str_test_lowercase(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    const char* original = "THIS IS A TEST";
    const char* expected = "this is a test";
    char* actual = str_lowercase(original);
    check_str_eq(tinfo, expected, actual);
    free(actual);
}

static void str_test_lowercase_ip(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    char original[] = "THIS IS A TEST";
    const char* expected = "this is a test";
    str_lowercase_ip(original);
    check_str_eq(tinfo, expected, original);
}

static void str_test_begins_ends(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    tinfo->total++;
    if (!str_begins("alpha", "alp")) {
        fprintf(stderr, "FAIL: %s !str_begins(\"alpha\", \"alp\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (str_begins("alpha", "scalp")) {
        fprintf(stderr, "FAIL: %s str_begins(\"alpha\", \"scalp\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (!str_ends("beta", "eta")) {
        fprintf(stderr, "FAIL: %s !str_ends(\"beta\", \"eta\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (str_ends("beta", "meta")) {
        fprintf(stderr, "FAIL: %s str_ends(\"beta\", \"meta\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (!str_begins_fold("alpha", "ALP")) {
        fprintf(stderr, "FAIL: %s !str_begins_fold(\"alpha\", \"ALP\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (str_begins_fold("alpha", "SCALP")) {
        fprintf(stderr, "FAIL: %s str_begins_fold(\"alpha\", \"SCALP\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (!str_ends_fold("beta", "ETA")) {
        fprintf(stderr, "FAIL: %s !str_ends_fold(\"beta\", \"ETA\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (str_ends_fold("beta", "META")) {
        fprintf(stderr, "FAIL: %s str_ends_fold(\"beta\", \"META\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;
}

static void str_test_filename_ext(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    {
        const char* filename = "file.zip";
        const char* exp = "zip";
        const char* ext = file_extension(filename);
        check_str_eq(tinfo, exp, ext);
    }
    {
        const char* filename = "file.tar.gz";
        const char* exp = "gz";
        const char* ext = file_extension(filename);
        check_str_eq(tinfo, exp, ext);
    }
    {
        const char* filename = "file.";
        const char* ext = file_extension(filename);
        check_str_eq(tinfo, "", ext);
    }
    {
        const char* filename = "file";
        const char* ext = file_extension(filename);
        check_str_eq(tinfo, "", ext);
    }
    {
        const char* filename = "...";
        const char* ext = file_extension(filename);
        check_str_eq(tinfo, "", ext);
    }
}
