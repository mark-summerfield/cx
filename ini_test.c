// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "ini_test.h"
#include "ini.h"
#include <stdio.h>
#include <stdlib.h>

void test1(tinfo* tinfo);
void test2(tinfo* tinfo);
void test3(tinfo* tinfo);
void test4(tinfo* tinfo);
static void check_reply(tinfo* tinfo, IniReply actual, IniReply expected);
static const char* reply_to_str(IniReply reply);

void ini_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    // test1(tinfo);
    // test2(tinfo);
    // test3(tinfo);
    test4(tinfo);
}

void test1(tinfo* tinfo) {
    if (tinfo->verbose)
        printf("%s/test1\n", tinfo->tag);
    Ini ini1 = ini_alloc("/tmp/test1.ini");

    bool value = true;
    IniReply reply = ini_get_bool(&ini1, NULL, "cando", &value);
    check_reply(tinfo, reply, IniItemNotFound);
    check_bool_eq(tinfo, value, true);

    ini_set_bool(&ini1, NULL, "cando", value);
    reply = ini_get_bool(&ini1, NULL, "cando", &value);
    check_reply(tinfo, reply, IniItemFound);
    check_bool_eq(tinfo, value, true);

    ini_free(&ini1);
}

void test2(tinfo* tinfo) {
    if (tinfo->verbose)
        printf("%s/test2\n", tinfo->tag);
    Ini ini1 = ini_alloc("/tmp/test2.ini");
    ini_set_comment(&ini1, NULL, NULL, "whole file comment");
    ini_set_str(&ini1, NULL, "name", "John Doe");
    ini_set_str(&ini1, NULL, "Organization", "Acme Widgets Inc.");
    ini_set_real(&ini1, NULL, "RANKING", 1.76);
    char* section = "Database";
    ini_set_str(&ini1, section, "server", "192.0.2.62");
    ini_set_comment(&ini1, section, "server", "IP address");
    ini_set_int(&ini1, section, "Port", 143);
    ini_set_bool(&ini1, section, "Exclusive", true);
    ini_set_str(&ini1, section, "FILE", "\"payroll.dat\"");
    ini_set_comment(&ini1, section, "file", "may include \"s for Windows");

    char* text = ini_save_to_str(&ini1);
    check_str_eq(tinfo, text, INI_EG1);
    free(text);

    {
        const char* v = ini_get_str(&ini1, NULL, "name");
        check_str_eq(tinfo, v, "John Doe");
    }
    {
        const char* v = ini_get_str(&ini1, NULL, "organization");
        check_str_eq(tinfo, v, "Acme Widgets Inc.");
    }
    check_isnull(tinfo, ini_get_str(&ini1, NULL, "no such key"));
    double r;
    bool ok = ini_get_real(&ini1, NULL, "ranking", &r);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, 1.76, r);
    ok = ini_get_real(&ini1, NULL, "no such key", &r);
    check_bool_eq(tinfo, ok, false);
    check_real_eq(tinfo, 1.76, r);
    {
        const char* v = ini_get_str(&ini1, section, "server");
        check_str_eq(tinfo, v, "192.0.2.62");
    }
    {
        const char* v = ini_get_str(&ini1, section, "file");
        check_str_eq(tinfo, v, "\"payroll.dat\"");
    }
    ok = false;
    IniReply reply = ini_get_bool(&ini1, section, "exclusive", &ok);
    check_bool_eq(tinfo, ok, true);
    check_int_eq(tinfo, reply, IniItemFound);
    reply = ini_get_bool(&ini1, section, "no such key", &ok);
    check_bool_eq(tinfo, ok, true);
    check_int_eq(tinfo, reply, IniItemNotFound);
    int port = 8080;
    reply = ini_get_int(&ini1, section, "no such key", &port);
    check_int_eq(tinfo, reply, IniItemNotFound);
    check_int_eq(tinfo, port, 8080);
    reply = ini_get_int(&ini1, section, "port", &port);
    check_int_eq(tinfo, reply, IniItemFound);
    check_int_eq(tinfo, port, 143);

    ini_free(&ini1);
}

void test3(tinfo* tinfo) {
    if (tinfo->verbose)
        printf("%s/test3\n", tinfo->tag);
    Ini ini1 = ini_alloc_from_str(INI_EG1);
    char* section = "DATABASE";
    ini_set_comment(&ini1, NULL, NULL, " whole file comment\n");
    ini_set_comment(&ini1, section, "server", "IP address");
    ini_set_comment(&ini1, section, "FILE", "may include \"s for Windows");

    char* text = ini_save_to_str(&ini1);
    check_casestr_eq(tinfo, text, INI_EG1);
    free(text);

    ini_set_str(&ini1, NULL, "NAME", "John Doe\n");
    ini_set_str(&ini1, NULL, "organization", "\tAcme Widgets Inc.\t");
    ini_set_real(&ini1, NULL, "Ranking", 1.76);
    ini_set_str(&ini1, section, "Server", "192.0.2.62");
    ini_set_int(&ini1, section, "port", 143);
    ini_set_bool(&ini1, section, "EXCLUSIVE", true);
    ini_set_str(&ini1, section, "file", "\"payroll.dat\"");

    text = ini_save_to_str(&ini1);
    check_casestr_eq(tinfo, text, INI_EG1);
    free(text);

    {
        const char* v = ini_get_str(&ini1, NULL, "name");
        check_str_eq(tinfo, v, "John Doe");
    }
    {
        const char* v = ini_get_str(&ini1, NULL, "organization");
        check_str_eq(tinfo, v, "Acme Widgets Inc.");
    }
    check_isnull(tinfo, ini_get_str(&ini1, NULL, "no such key"));
    double r;
    bool ok = ini_get_real(&ini1, NULL, "ranking", &r);
    check_bool_eq(tinfo, ok, true);
    check_real_eq(tinfo, 1.76, r);
    ok = ini_get_real(&ini1, NULL, "no such key", &r);
    check_bool_eq(tinfo, ok, false);
    check_real_eq(tinfo, 1.76, r);
    {
        const char* v = ini_get_str(&ini1, section, "server");
        check_str_eq(tinfo, v, "192.0.2.62");
    }
    {
        const char* v = ini_get_str(&ini1, section, "file");
        check_str_eq(tinfo, v, "\"payroll.dat\"");
    }
    ok = false;
    IniReply reply = ini_get_bool(&ini1, section, "exclusive", &ok);
    check_bool_eq(tinfo, ok, true);
    check_int_eq(tinfo, reply, IniItemFound);
    reply = ini_get_bool(&ini1, section, "no such key", &ok);
    check_bool_eq(tinfo, ok, true);
    check_int_eq(tinfo, reply, IniItemNotFound);
    int port = 8080;
    reply = ini_get_int(&ini1, section, "no such key", &port);
    check_int_eq(tinfo, reply, IniItemNotFound);
    check_int_eq(tinfo, port, 8080);
    reply = ini_get_int(&ini1, section, "port", &port);
    check_int_eq(tinfo, reply, IniItemFound);
    check_int_eq(tinfo, port, 143);

    ini_free(&ini1);
}

void test4(tinfo* tinfo) {
    if (tinfo->verbose)
        printf("%s/test4\n", tinfo->tag);
    // Create with defaults
    Ini ini1 = ini_alloc_from_str(INI_EG2);
    // Add comments
    ini_set_comment(&ini1, NULL, NULL, "Configuration for MyApp");
    char* section = "Window";
    ini_set_comment(&ini1, section, "scale", "1.0-3.5");
    ini_set_comment(&ini1, section, "theme",
                    "[Base Gleam Gtk Oxy Plastic]");
    section = "UI";
    ini_set_comment(&ini1, section, "LastCategory", "1-8");
    ini_set_comment(&ini1, section, "LastTab", "0-4");

    char* text = ini_save_to_str(&ini1);
    check_casestr_eq(tinfo, text, INI_EG2);
    free(text);

    /*
    char* section = "...";
    {
        const char* v = ini_get_str(&ini1, section, "Mode");
        check_str_eq(tinfo, v, "words");
    }
    // TODO etc
    */

    ini_free(&ini1);
}

static void check_reply(tinfo* tinfo, IniReply actual, IniReply expected) {
    tinfo->total++;
    if (actual != expected)
        fprintf(stderr, "FAIL: %s check_reply: %s != %s\n", tinfo->tag,
                reply_to_str(actual), reply_to_str(expected));
    else
        tinfo->ok++;
}

static const char* reply_to_str(IniReply reply) {
    switch (reply) {
    case IniItemNotFound:
        return "IniItemNotFound";
    case IniInvalidValue:
        return "IniInvalidValue";
    case IniItemFound:
        return "IniItemFound";
    default:
        fprintf(stderr, "invalid reply %d\n", reply);
        return "***INVALID***";
    }
}
