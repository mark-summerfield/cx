// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "ini.h"
#include "fx.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOT_FOUND -1

#define STRING 's'
#define BOOL 'b'
#define INT 'i'
#define REAL 'r'

static void load_file(Ini* ini, const char* filename);
static void item_destroy(void* item_);
static int item_cmp(const void* item1_, const void* item2_);
static int find_sectid(const Ini* ini, const char* section);
static IniItem* find_item(Ini* ini, const char* section, const char* key);

Ini ini_alloc(const char* filename) {
    assert(filename && ".ini filename is required");
    Ini ini = {strdup(filename), vec_str_alloc(),
               vec_alloc(0, item_cmp, item_destroy)};
    if (is_file(filename))
        load_file(&ini, filename);
    return ini;
}

void ini_free(Ini* ini) {
    free(ini->filename);
    vec_str_free(&ini->sections);
    vec_free(&ini->items);
}

static void load_file(Ini* ini, const char* filename) {
    puts("load_file"); // TODO
}

bool ini_save(Ini* ini) {
    puts("ini_save"); // TODO
    return false;     // TODO
}

static int find_sectid(const Ini* ini, const char* section) {
    for (int i = 0; i < vec_str_size(&ini->sections); i++)
        if (str_eq_fold(section, vec_str_get(&ini->sections, i)))
            return i;
    return NOT_FOUND;
}

static IniItem* find_item(Ini* ini, const char* section, const char* key) {
    int sectid = find_sectid(ini, section);
    if (sectid == NOT_FOUND)
        return NULL;
    for (int i = 0; i < vec_size(&ini->items); i++) {
        IniItem* item = vec_get(&ini->items, i);
        if (item->sectid == sectid && str_eq_fold(item->key, key))
            return item;
    }
    return NULL;
}

IniReply ini_get_bool(Ini* ini, const char* section, const char* key,
                      bool* value) {
    IniItem* item = find_item(ini, section, key);
    if (!item)
        return IniItemNotFound;
    if (item->kind == BOOL) {
        *value = item->b;
        return IniItemFound;
    }
    if (item->kind != STRING)
        return IniInvalidValue;
    if (str_eq_fold(item->value, "true") || str_eq_fold(item->value, "t") ||
        str_eq_fold(item->value, "yes") || str_eq_fold(item->value, "y") ||
        str_eq_fold(item->value, "on") || str_eq_fold(item->value, "1")) {
        item->kind = BOOL;
        *value = item->b = true;
        return IniItemFound;
    }
    if (str_eq_fold(item->value, "false") ||
        str_eq_fold(item->value, "f") || str_eq_fold(item->value, "no") ||
        str_eq_fold(item->value, "n") || str_eq_fold(item->value, "off") ||
        str_eq_fold(item->value, "0")) {
        item->kind = BOOL;
        *value = item->b = false;
        return IniItemFound;
    }
    return IniInvalidValue;
}

IniReply ini_get_int(Ini* ini, const char* section, const char* key,
                     int* value) {
    IniItem* item = find_item(ini, section, key);
    if (!item)
        return IniItemNotFound;
    if (item->kind == INT) {
        *value = item->i;
        return IniItemFound;
    }
    if (item->kind != STRING)
        return IniInvalidValue;
    int n = sscanf(item->value, "%d", value);
    if (n == 1) {
        item->kind = INT;
        item->i = *value;
        return IniItemFound;
    }
    return IniInvalidValue;
}

IniReply ini_get_real(Ini* ini, const char* section, const char* key,
                      double* value) {
    IniItem* item = find_item(ini, section, key);
    if (!item)
        return IniItemNotFound;
    if (item->kind == REAL) {
        *value = item->r;
        return IniItemFound;
    }
    if (item->kind != STRING)
        return IniInvalidValue;
    int n = sscanf(item->value, "%lg", value);
    if (n == 1) {
        item->kind = REAL;
        item->i = *value;
        return IniItemFound;
    }
    return IniInvalidValue;
}

const char* ini_get_str(Ini* ini, const char* section, const char* key) {
    IniItem* item = find_item(ini, section, key);
    if (!item || item->kind != STRING)
        return NULL;
    return item->value;
}

void ini_set_bool(Ini* ini, const char* section, const char* key,
                  bool value) {}

void ini_set_int(Ini* ini, const char* section, const char* key,
                 int value) {}

void ini_set_real(Ini* ini, const char* section, const char* key,
                  double value) {}

void ini_set_str(Ini* ini, const char* section, const char* key,
                 const char* value) {}

void ini_set_comment(Ini* ini, const char* section, const char* key,
                     const char* comment) {}

// Compares section & key only
static int item_cmp(const void* item1_, const void* item2_) {
    const IniItem* item1 = item1_;
    const IniItem* item2 = item2_;
    if (item1->sectid < item2->sectid)
        return -1;
    if (item1->sectid > item2->sectid)
        return 1;
    return strcasecmp(item1->key, item2->key);
}

static void item_destroy(void* item_) {
    IniItem* item = item_;
    free(item->key);
    free(item->value);
    free(item->comment);
}
