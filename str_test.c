// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3

#include "str_test.h"
#include "exit.h"
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
static void str_test_trim(tinfo*);

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
    tinfo->tag = "str_test_trim";
    str_test_trim(tinfo);
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
    if (!str_begins("alpha", "alp"))
        WARN("FAIL: %s !str_begins(\"alpha\", \"alp\"\n", tinfo->tag);
    else
        tinfo->ok++;

    tinfo->total++;
    if (str_begins("alpha", "scalp"))
        WARN("FAIL: %s str_begins(\"alpha\", \"scalp\"\n", tinfo->tag);
    else
        tinfo->ok++;

    tinfo->total++;
    if (!str_ends("beta", "eta"))
        WARN("FAIL: %s !str_ends(\"beta\", \"eta\"\n", tinfo->tag);
    else
        tinfo->ok++;

    tinfo->total++;
    if (str_ends("beta", "meta"))
        WARN("FAIL: %s str_ends(\"beta\", \"meta\"\n", tinfo->tag);
    else
        tinfo->ok++;

    tinfo->total++;
    if (!str_casebegins("alpha", "ALP"))
        WARN("FAIL: %s !str_casebegins(\"alpha\", \"ALP\"\n", tinfo->tag);
    else
        tinfo->ok++;

    tinfo->total++;
    if (str_casebegins("alpha", "SCALP"))
        WARN("FAIL: %s str_casebegins(\"alpha\", \"SCALP\"\n", tinfo->tag);
    else
        tinfo->ok++;

    tinfo->total++;
    if (!str_caseends("beta", "ETA"))
        WARN("FAIL: %s !str_caseends(\"beta\", \"ETA\"\n", tinfo->tag);
    else
        tinfo->ok++;

    tinfo->total++;
    if (str_caseends("beta", "META"))
        WARN("FAIL: %s str_caseends(\"beta\", \"META\"\n", tinfo->tag);
    else
        tinfo->ok++;
}

static void str_test_filename_ext(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    {
        const char* filename = "file.zip";
        const char* exp = "zip";
        const char* ext = file_ext(filename);
        check_str_eq(tinfo, exp, ext);
    }
    {
        const char* filename = "file.tar.gz";
        const char* exp = "gz";
        const char* ext = file_ext(filename);
        check_str_eq(tinfo, exp, ext);
    }
    {
        const char* filename = "file.";
        const char* ext = file_ext(filename);
        check_str_eq(tinfo, "", ext);
    }
    {
        const char* filename = "file";
        const char* ext = file_ext(filename);
        check_str_eq(tinfo, "", ext);
    }
    {
        const char* filename = "...";
        const char* ext = file_ext(filename);
        check_str_eq(tinfo, "", ext);
    }
}

static void str_test_trim(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    char* s = NULL;
    char* t = (char*)str_trim_left(s);
    check_isnull(tinfo, t);
    s = "";
    t = (char*)str_trim_left(s);
    check_isnull(tinfo, t);
    s = " \t\n \t\n ";
    t = (char*)str_trim_left(s);
    check_isnull(tinfo, t);
    s = "\t a typical string with leading ws";
    t = (char*)str_trim_left(s);
    check_str_eq(tinfo, t, "a typical string with leading ws");

    s = NULL;
    t = (char*)str_trim(s);
    check_isnull(tinfo, t);
    s = "";
    t = (char*)str_trim(s);
    check_isnull(tinfo, t);
    s = " \t\n \t\n ";
    t = (char*)str_trim(s);
    check_isnull(tinfo, t);
    s = "a string with no leading or trailing ws";
    t = (char*)str_trim(s);
    check_str_eq(tinfo, t, s);
    free(t);
    s = "\t a typical string with leading ws and newline\r\n";
    t = (char*)str_trim(s);
    check_str_eq(tinfo, t, "a typical string with leading ws and newline");
    free(t);
    s = "         \t key = \tvalue \n";
    t = (char*)str_trim(s);
    check_str_eq(tinfo, t, "key = \tvalue");
    free(t);

    s = NULL;
    t = str_trimn(s, 0);
    check_isnull(tinfo, t);
    s = "";
    t = str_trimn(s, 0);
    check_isnull(tinfo, t);
    s = " \t\n \t\n ";
    t = str_trimn(s, 0);
    check_isnull(tinfo, t);
    s = "a string with no leading or trailing ws";
    t = str_trimn(s, 0);
    check_str_eq(tinfo, t, s);
    free(t);
    s = " a key = a value etc.\n";
    t = str_trimn(s, 0);
    check_str_eq(tinfo, t, "a key = a value etc.");
    free(t);
    char* q = strchr(s, '=');
    q--;
    t = str_trimn(s, q - s);
    check_str_eq(tinfo, t, "a key");
    free(t);
    q += 2;
    t = str_trimn(q, 0);
    check_str_eq(tinfo, t, "a value etc.");
    free(t);
    s = "\t a typical string with leading ws and newline\r\n";
    t = str_trimn(s, 0);
    check_str_eq(tinfo, t, "a typical string with leading ws and newline");
    free(t);
    s = "         \t key = \tvalue \n";
    t = str_trimn(s, 0);
    check_str_eq(tinfo, t, "key = \tvalue");
    free(t);
    s = " x ";
    t = str_trimn(s, 2);
    check_str_eq(tinfo, t, "x");
    free(t);
    s = "x";
    t = str_trimn(s, 1);
    check_str_eq(tinfo, t, "x");
    free(t);
}
