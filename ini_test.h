// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx_util_test.h"

void ini_tests(tinfo* tinfo);

#define INI_EG1                         \
    "; whole file comment\n\n"          \
    "name = John Doe\n"                 \
    "Organization = Acme Things Inc.\n" \
    "RANKING = 1.76\n"                  \
    "\n"                                \
    "[Database]\n"                      \
    "Exclusive = true\n"                \
    "; may include \"s for Windows\n"   \
    "FILE = \"payroll.dat\"\n"          \
    "Port = 143\n"                      \
    "; IP address\n"                    \
    "server = 192.0.2.62\n"

#define INI_EG2                        \
    "; Configuration file for MyApp\n" \
    "\n"                               \
    "[Window]\n"                       \
    "height = 561\n"                   \
    "; 1.0-3.5\n"                      \
    "scale = 1.15\n"                   \
    "; [Base Gleam Gtk Oxy Plastic]\n" \
    "theme = Oxy\n"                    \
    "width = 407\n"                    \
    "x = 101\n"                        \
    "y = 343\n"                        \
    "\n"                               \
    "[UI]\n"                           \
    "; 1-8\n"                          \
    "LastCategory = 1\n"               \
    "LastRegex = (?P<lig>f[fil])\n"    \
    "; 0-4\n"                          \
    "LastTab = 0\n"

#define INI_EG3                                               \
    "; Config file for MyApp\n"                               \
    "\n"                                                      \
    "[Window]\n"                                              \
    "height = 480\n"                                          \
    "; 1.0-3.5\n"                                             \
    "scale = 1.7\n"                                           \
    "; [Base Gleam Gtk Oxy Plastic]\n"                        \
    "theme = Gleam\n"                                         \
    "width = 640\n"                                           \
    "x = 199\n"                                               \
    "y = 343\n"                                               \
    "\n"                                                      \
    "[UI]\n"                                                  \
    "; 1-8\n"                                                 \
    "LastCategory = 2\n"                                      \
    "LastRegex = ^\\s*(?P<key>\\S+)\\s*=\\s*(?P<value>.+)$\n" \
    "; 0-4\n"                                                 \
    "LastTab = 3\n"
