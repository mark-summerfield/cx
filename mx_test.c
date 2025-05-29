// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3

#include "mx_test.h"
#include "exit.h"
#include "mx.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>

void mx_tests(tinfo* tinfo) {
    char s[28] = "";
    commas(s, 1234567890);
    tinfo->total++;
    char* e = "1,234,567,890";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, 123456789);
    tinfo->total++;
    e = "123,456,789";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, 12345678);
    tinfo->total++;
    e = "12,345,678";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, 1234567);
    tinfo->total++;
    e = "1,234,567";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, 123456);
    tinfo->total++;
    e = "123,456";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, 12345);
    tinfo->total++;
    e = "12,345";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, 1234);
    tinfo->total++;
    e = "1,234";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, 123);
    tinfo->total++;
    e = "123";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, 12);
    tinfo->total++;
    e = "12";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, 1);
    tinfo->total++;
    e = "1";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, 0);
    tinfo->total++;
    e = "0";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, -1234567890);
    tinfo->total++;
    e = "-1,234,567,890";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, -123456789);
    tinfo->total++;
    e = "-123,456,789";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, -12345678);
    tinfo->total++;
    e = "-12,345,678";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, -1234567);
    tinfo->total++;
    e = "-1,234,567";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, -123456);
    tinfo->total++;
    e = "-123,456";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, -12345);
    tinfo->total++;
    e = "-12,345";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, -1234);
    tinfo->total++;
    e = "-1,234";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, -123);
    tinfo->total++;
    e = "-123";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, -12);
    tinfo->total++;
    e = "-12";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
    commas(s, -1);
    tinfo->total++;
    e = "-1";
    if (!str_eq(s, e))
        WARN("FAIL: %s «%s» != «%s»\n", tinfo->tag, s, e);
    else
        tinfo->ok++;
}
