// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3

#include "fx_test.h"
#include "exit.h"
#include "fx.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>

void check_file_change_ext(tinfo* tinfo, const char* filename,
                           const char* ext, const char* expected);

void fx_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    {
        tinfo->total++;
        const char* ext = file_ext("archive.tar.gz");
        if (!str_eq("gz", ext))
            WARN("FAIL: %s expected gz go %s\n", tinfo->tag, ext);
        else
            tinfo->ok++;
    }
    {
        tinfo->total++;
        const char* ext = file_ext("README");
        if (!str_eq("", ext))
            WARN("FAIL: %s expected \"\" go %s\n", tinfo->tag, ext);
        else
            tinfo->ok++;
    }
    char config_filename[FILENAME_SIZE + 1];
    char* e = "/home/mark/.config/cx_test.ini";
    bool exists = file_get_ini_name(config_filename, NULL, "cx_test");
    tinfo->total++;
    if (exists)
        WARN("FAIL: %s expected «%s» not to exist\n", tinfo->tag,
             config_filename);
    else
        tinfo->ok++;
    tinfo->total++;
    if (!str_eq(e, config_filename))
        WARN("FAIL: %s expected «%s» got «%s»\n", tinfo->tag, e,
             config_filename);
    else
        tinfo->ok++;
    e = "/home/mark/.config/qtrac_cx_test.ini";
    exists = file_get_ini_name(config_filename, "qtrac", "cx_test");
    tinfo->total++;
    if (exists)
        WARN("FAIL: %s expected «%s» not to exist\n", tinfo->tag,
             config_filename);
    else
        tinfo->ok++;
    tinfo->total++;
    if (!str_eq(e, config_filename))
        WARN("FAIL: %s expected «%s» got «%s»\n", tinfo->tag, e,
             config_filename);
    else
        tinfo->ok++;

    // append
    check_file_change_ext(tinfo, "/home/mark/README", ".md",
                          "/home/mark/README.md");
    // replace
    check_file_change_ext(tinfo, "/home/mark/apple.cgi", ".png",
                          "/home/mark/apple.png");
    // replace with longer
    check_file_change_ext(tinfo, "/home/mark/archive.tar", ".tar.gz",
                          "/home/mark/archive.tar.gz");
    // replace with shorter
    check_file_change_ext(tinfo, "/home/mark/archive.tar-gz", ".tgz",
                          "/home/mark/archive.tgz");
}

void check_file_change_ext(tinfo* tinfo, const char* filename,
                           const char* ext, const char* expected) {
    tinfo->total++;
    char* name = file_change_ext(filename, ext);
    if (!str_eq(expected, name))
        WARN("FAIL: %s expected «%s» got «%s»\n", tinfo->tag, expected,
             name);
    else
        tinfo->ok++;
    free(name);
}
