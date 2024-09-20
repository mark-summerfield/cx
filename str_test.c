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
static void str_test_trim(tinfo*);
static void str_test_split_chr(tinfo*);
static void str_test_split_ws(tinfo*);

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
    tinfo->tag = "str_test_split_chr";
    str_test_split_chr(tinfo);
    tinfo->tag = "str_test_split_ws";
    str_test_split_ws(tinfo);
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
    if (!str_casebegins("alpha", "ALP")) {
        fprintf(stderr, "FAIL: %s !str_casebegins(\"alpha\", \"ALP\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (str_casebegins("alpha", "SCALP")) {
        fprintf(stderr, "FAIL: %s str_casebegins(\"alpha\", \"SCALP\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (!str_caseends("beta", "ETA")) {
        fprintf(stderr, "FAIL: %s !str_caseends(\"beta\", \"ETA\"\n",
                tinfo->tag);
    } else
        tinfo->ok++;

    tinfo->total++;
    if (str_caseends("beta", "META")) {
        fprintf(stderr, "FAIL: %s str_caseends(\"beta\", \"META\"\n",
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

static void str_test_split_chr(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    SplitParts parts1 = split_chr("one\ttwo\tthree\tfour", '\t');
    check_int_eq(tinfo, parts1.nparts, 4);
    check_str_eq(tinfo, parts1.parts[0], "one");
    check_str_eq(tinfo, parts1.parts[1], "two");
    check_str_eq(tinfo, parts1.parts[2], "three");
    check_str_eq(tinfo, parts1.parts[3], "four");
    split_parts_free(&parts1);

    parts1 = split_chr("elephant", '\t');
    check_int_eq(tinfo, parts1.nparts, 1);
    check_str_eq(tinfo, parts1.parts[0], "elephant");
    split_parts_free(&parts1);

    parts1 = split_chr("moveto 3.5 7.45", ' ');
    check_int_eq(tinfo, parts1.nparts, 3);
    check_str_eq(tinfo, parts1.parts[0], "moveto");
    check_str_eq(tinfo, parts1.parts[1], "3.5");
    check_str_eq(tinfo, parts1.parts[2], "7.45");
    split_parts_free(&parts1);

    parts1 = split_chr("    ", '\t');
    check_int_eq(tinfo, parts1.nparts, 1); // no tabs so whole str
    split_parts_free(&parts1);

    parts1 = split_chr("    ", ' ');
    check_int_eq(tinfo, parts1.nparts, 0);
    split_parts_free(&parts1);

    parts1 = split_chr("", '\t');
    check_int_eq(tinfo, parts1.nparts, 0);
    split_parts_free(&parts1);

    parts1 = split_chr("", ' ');
    check_int_eq(tinfo, parts1.nparts, 0);
    split_parts_free(&parts1);

    SplitParts parts2 = split_chr("width 240", ' ');
    check_int_eq(tinfo, parts2.nparts, 2);
    check_str_eq(tinfo, parts2.parts[0], "width");
    check_str_eq(tinfo, parts2.parts[1], "240");
    split_parts_free(&parts2);

    SplitParts parts3 = split_chr("height 160", ' ');
    check_int_eq(tinfo, parts3.nparts, 2);
    check_str_eq(tinfo, parts3.parts[0], "height");
    check_str_eq(tinfo, parts3.parts[1], "160");
    split_parts_free(&parts3);

    SplitParts parts4 =
        split_chr("rect x=10 y=10 w=220 h=140 fg=blue bg=green", ' ');
    check_int_eq(tinfo, parts4.nparts, 7);
    check_str_eq(tinfo, parts4.parts[0], "rect");
    check_str_eq(tinfo, parts4.parts[1], "x=10");
    check_str_eq(tinfo, parts4.parts[2], "y=10");
    check_str_eq(tinfo, parts4.parts[3], "w=220");
    check_str_eq(tinfo, parts4.parts[4], "h=140");
    check_str_eq(tinfo, parts4.parts[5], "fg=blue");
    check_str_eq(tinfo, parts4.parts[6], "bg=green");
    split_parts_free(&parts4);
}

static void str_test_split_ws(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);

    SplitParts parts1 = split_ws("  one\ttwo \tthree\tfour\n");
    check_int_eq(tinfo, parts1.nparts, 4);
    check_str_eq(tinfo, parts1.parts[0], "one");
    check_str_eq(tinfo, parts1.parts[1], "two");
    check_str_eq(tinfo, parts1.parts[2], "three");
    check_str_eq(tinfo, parts1.parts[3], "four");
    split_parts_free(&parts1);

    parts1 = split_ws("   moveto 3.5\t 7.45\n");
    check_int_eq(tinfo, parts1.nparts, 3);
    check_str_eq(tinfo, parts1.parts[0], "moveto");
    check_str_eq(tinfo, parts1.parts[1], "3.5");
    check_str_eq(tinfo, parts1.parts[2], "7.45");
    split_parts_free(&parts1);

    parts1 = split_ws("elephant");
    check_int_eq(tinfo, parts1.nparts, 1);
    check_str_eq(tinfo, parts1.parts[0], "elephant");
    split_parts_free(&parts1);

    parts1 = split_ws("   ");
    check_int_eq(tinfo, parts1.nparts, 0);
    split_parts_free(&parts1);

    parts1 = split_ws("");
    check_int_eq(tinfo, parts1.nparts, 0);
    split_parts_free(&parts1);

    SplitParts parts2 = split_ws("width 240");
    check_int_eq(tinfo, parts2.nparts, 2);
    check_str_eq(tinfo, parts2.parts[0], "width");
    check_str_eq(tinfo, parts2.parts[1], "240");
    split_parts_free(&parts2);

    SplitParts parts3 = split_ws("height 160");
    check_int_eq(tinfo, parts3.nparts, 2);
    check_str_eq(tinfo, parts3.parts[0], "height");
    check_str_eq(tinfo, parts3.parts[1], "160");
    split_parts_free(&parts3);

    SplitParts parts4 =
        split_ws("rect x=10 y=10 w=220 h=140 fg=blue bg=green");
    check_int_eq(tinfo, parts4.nparts, 7);
    check_str_eq(tinfo, parts4.parts[0], "rect");
    check_str_eq(tinfo, parts4.parts[1], "x=10");
    check_str_eq(tinfo, parts4.parts[2], "y=10");
    check_str_eq(tinfo, parts4.parts[3], "w=220");
    check_str_eq(tinfo, parts4.parts[4], "h=140");
    check_str_eq(tinfo, parts4.parts[5], "fg=blue");
    check_str_eq(tinfo, parts4.parts[6], "bg=green");
    split_parts_free(&parts4);
}
