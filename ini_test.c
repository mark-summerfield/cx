#include "ini_test.h"
#include "ini.h"
#include <stdio.h>

void test1(tinfo* tinfo);
static void check_reply(tinfo* tinfo, IniReply actual, IniReply expected);
static const char* reply_to_str(IniReply reply);

const char* EG1 =
    "name = John Doe\n"
    "organization = Acme Widgets Inc.\n"
    "\n"
    "[database]\n"
    "; use IP address in case network name resolution is not working\n"
    "server = 192.0.2.62     \n"
    "port = 143\n"
    "file = \"payroll.dat\"\n";

void ini_tests(tinfo* tinfo) {
    if (tinfo->verbose)
        puts(tinfo->tag);
    test1(tinfo);
}

void test1(tinfo* tinfo) {
    if (tinfo->verbose)
        printf("%s/test1\n", tinfo->tag);
    Ini ini1 = ini_alloc("/tmp/test1.ini");

    bool value = true;
    IniReply reply =
        ini_get_bool(&ini1, INI_UNNAMED_SECTION, "cando", &value);
    check_reply(tinfo, reply, IniItemNotFound);
    check_bool_eq(tinfo, value, true);

    ini_set_bool(&ini1, INI_UNNAMED_SECTION, "cando", value);
    reply = ini_get_bool(&ini1, INI_UNNAMED_SECTION, "cando", &value);
    check_reply(tinfo, reply, IniItemFound);
    check_bool_eq(tinfo, value, true);

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
