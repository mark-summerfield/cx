// Copyright © 2024 Mark Summerfield. All rights reserved.

#define _GNU_SOURCE // for asprintf

#include "ini.h"
#include "fx.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOT_FOUND -1
#define UNNAMED_SECTION "!"

static void load_file(Ini* ini, const char* filename);
static IniItem* item_alloc(const char* key, const char* value, int sectid);
static void item_destroy(void* item_);
static int item_cmp(const void* item1_, const void* item2_);
static int find_sectid(const Ini* ini, const char* section);
static int maybe_add_section(Ini* ini, const char* section);
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
    bool ok = true;
    FILE* file = fopen(ini->filename, "w");
    if (!file) {
        ok = false;
        goto end;
    }
    int prev_sectid = NOT_FOUND;
    vec_sort(&ini->items); // sectid x key
    for (int i = 0; i < vec_size(&ini->items); ++i) {
        const IniItem* item = vec_get(&ini->items, i);
        if (item->sectid != prev_sectid) { // new section
            prev_sectid = item->sectid;
            const char* section = vec_str_get(&ini->sections, prev_sectid);
            if (!str_eq(section, UNNAMED_SECTION))
                fprintf(file, "[%s]\n", section);
        }
        if (item->comment)
            fprintf(file, "; %s\n", item->comment);
        fprintf(file, "%s = %s\n", item->key, item->value);
    }
    if (fclose(file)) {
        ok = false;
        goto end;
    }
end:
    return ok;
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

static int maybe_add_section(Ini* ini, const char* section) {
    int sectid = find_sectid(ini, section);
    if (sectid == NOT_FOUND) {
        vec_str_push(&ini->sections, strdup(section));
        sectid = vec_str_size(&ini->sections) - 1;
    }
    return sectid;
}

IniReply ini_get_bool(const Ini* ini, const char* section, const char* key,
                      bool* value) {
    const IniItem* item = find_item((Ini*)ini, section, key);
    if (!item)
        return IniItemNotFound;
    if (str_eq_fold(item->value, "true") || str_eq_fold(item->value, "t") ||
        str_eq_fold(item->value, "yes") || str_eq_fold(item->value, "y") ||
        str_eq_fold(item->value, "on") || str_eq_fold(item->value, "1")) {
        *value = true;
        return IniItemFound;
    }
    if (str_eq_fold(item->value, "false") ||
        str_eq_fold(item->value, "f") || str_eq_fold(item->value, "no") ||
        str_eq_fold(item->value, "n") || str_eq_fold(item->value, "off") ||
        str_eq_fold(item->value, "0")) {
        *value = false;
        return IniItemFound;
    }
    return IniInvalidValue;
}

IniReply ini_get_int(const Ini* ini, const char* section, const char* key,
                     int* value) {
    const IniItem* item = find_item((Ini*)ini, section, key);
    if (!item)
        return IniItemNotFound;
    int n = sscanf(item->value, "%d", value);
    return (n == 1) ? IniItemFound : IniInvalidValue;
}

IniReply ini_get_real(const Ini* ini, const char* section, const char* key,
                      double* value) {
    const IniItem* item = find_item((Ini*)ini, section, key);
    if (!item)
        return IniItemNotFound;
    int n = sscanf(item->value, "%lg", value);
    return (n == 1) ? IniItemFound : IniInvalidValue;
}

const char* ini_get_str(const Ini* ini, const char* section,
                        const char* key) {
    const IniItem* item = find_item((Ini*)ini, section, key);
    if (!item)
        return NULL;
    return item->value;
}

void ini_set_bool(Ini* ini, const char* section, const char* key,
                  bool value) {
    IniItem* item = find_item(ini, section, key);
    if (item) {
        const char* s = bool_to_str(value);
        if (!str_eq_fold(item->value, s)) {
            free(item->value);
            item->value = strdup(s);
        }
    } else {
        int sectid = maybe_add_section(ini, section);
        const char* s = bool_to_str(value);
        item = item_alloc(strdup(key), strdup(s), sectid);
        vec_push(&ini->items, item);
    }
}

void ini_set_int(Ini* ini, const char* section, const char* key,
                 int value) {
    IniItem* item = find_item(ini, section, key);
    if (item) {
        int i;
        int n = sscanf(item->value, "%d", &i);
        if (n != 1 || i != value) {
            free(item->value);
            asprintf(&item->value, "%d", value);
        }
    } else {
        int sectid = maybe_add_section(ini, section);
        char* s;
        asprintf(&s, "%d", value);
        item = item_alloc(strdup(key), s, sectid);
        vec_push(&ini->items, item);
    }
}

void ini_set_real(Ini* ini, const char* section, const char* key,
                  double value) {
    IniItem* item = find_item(ini, section, key);
    if (item) {
        free(item->value);
        asprintf(&item->value, "%lg", value);
    } else {
        int sectid = maybe_add_section(ini, section);
        char* s;
        asprintf(&s, "%lg", value);
        item = item_alloc(strdup(key), s, sectid);
        vec_push(&ini->items, item);
    }
}

void ini_set_str(Ini* ini, const char* section, const char* key,
                 const char* value) {
    IniItem* item = find_item(ini, section, key);
    if (item) {
        if (!str_eq(item->value, value)) {
            free(item->value);
            item->value = strdup(value);
        }
    } else {
        int sectid = maybe_add_section(ini, section);
        item = item_alloc(strdup(key), strdup(value), sectid);
        vec_push(&ini->items, item);
    }
}

bool ini_set_comment(Ini* ini, const char* section, const char* key,
                     const char* comment) {
    IniItem* item = find_item(ini, section, key);
    if (!item)
        return false;
    if (!str_eq(item->comment, comment)) {
        free(item->comment);
        item->comment = strdup(comment);
    }
    return true;
}

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

static IniItem* item_alloc(const char* key, const char* value, int sectid) {
    IniItem* item = malloc(sizeof(IniItem));
    item->key = strdup(key);
    item->value = strdup(value);
    item->comment = NULL;
    item->sectid = sectid;
    return item;
}

static void item_destroy(void* item_) {
    IniItem* item = item_;
    free(item->key);
    free(item->value);
    free(item->comment);
}
