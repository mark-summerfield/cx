// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx_util_test.h"

void ini_tests(tinfo* tinfo);

#define INI_EG1                          \
    "; whole file comment\n\n"           \
    "name = John Doe\n"                  \
    "Organization = Acme Widgets Inc.\n" \
    "RANKING = 1.76\n"                   \
    "\n"                                 \
    "[Database]\n"                       \
    "Exclusive = true\n"                 \
    "; may include \"s for Windows\n"    \
    "FILE = \"payroll.dat\"\n"           \
    "Port = 143\n"                       \
    "; IP address\n"                     \
    "server = 192.0.2.62\n"

#define INI_EG2                                                      \
    "; Configuration file for Qtrac Ltd.'s comparepdfcmd program.\n" \
    "\n"                                                             \
    "[general]\n"                                                    \
    "; [appearance, chars, words]\n"                                 \
    "mode=words\n"                                                   \
    "; [10, 90]\n"                                                   \
    "highlight-transparency=80\n"                                    \
    "; [1, 1000]\n"                                                  \
    "maxdirdepth=1000\n"                                             \
    "\n"                                                             \
    "[appearance]\n"                                                 \
    "; [80, 100]\n"                                                  \
    "accuracy=100\n"                                                 \
    "; [4, 144]\n"                                                   \
    "square-size=10\n"                                               \
    "new-renderer=false\n"                                           \
    "\n"                                                             \
    "[colors]\n"                                                     \
    "change-bar=#F00\n"                                              \
    "exclusion-line=#A42929\n"                                       \
    "highlight=#FF0\n"                                               \
    "delete=#F00\n"                                                  \
    "insert=#0FF\n"                                                  \
    "replace=#F0F\n"                                                 \
    "\n"                                                             \
    "[report]\n"                                                     \
    "compact=false\n"                                                \
    "scale=1.4\n"                                                    \
    "line-tolerance=10\n"                                            \
    "normalize-hyphens=true\n"
