// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "fx_test.h"
#include "fx.h"
#include "str.h"
#include <stdio.h>

void fx_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    {
        tinfo->total++;
        const char* ext = file_ext("archive.tar.gz");
        if (!str_eq("gz", ext)) {
            fprintf(stderr, "FAIL: %s expected gz go %s\n", tinfo->tag,
                    ext);
        } else
            tinfo->ok++;
    }
    {
        tinfo->total++;
        const char* ext = file_ext("README");
        if (!str_eq("", ext)) {
            fprintf(stderr, "FAIL: %s expected \"\" go %s\n", tinfo->tag,
                    ext);
        } else
            tinfo->ok++;
    }
    char config_filename[FILENAME_SIZE + 1];
    char* e = "/home/mark/.config/cx_test.ini";
    bool exists = file_get_ini_name(config_filename, NULL, "cx_test");
    tinfo->total++;
    if (exists)
        fprintf(stderr, "FAIL: %s expected «%s» not to exist\n", tinfo->tag,
                config_filename);
    else
        tinfo->ok++;
    tinfo->total++;
    if (!str_eq(e, config_filename)) {
        fprintf(stderr, "FAIL: %s expected «%s» got «%s»\n", tinfo->tag, e,
                config_filename);
    } else
        tinfo->ok++;
    e = "/home/mark/.config/qtrac_cx_test.ini";
    exists = file_get_ini_name(config_filename, "qtrac", "cx_test");
    tinfo->total++;
    if (exists)
        fprintf(stderr, "FAIL: %s expected «%s» not to exist\n", tinfo->tag,
                config_filename);
    else
        tinfo->ok++;
    tinfo->total++;
    if (!str_eq(e, config_filename)) {
        fprintf(stderr, "FAIL: %s expected «%s» got «%s»\n", tinfo->tag, e,
                config_filename);
    } else
        tinfo->ok++;
}
