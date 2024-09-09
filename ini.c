// Copyright © 2024 Mark Summerfield. All rights reserved.

#define _GNU_SOURCE // for asprintf

#include "ini.h"
#include "fx.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOT_FOUND -2

static bool load_file(Ini* ini);
bool parse_text(Ini* ini, const char* text);
static void save_file(Ini* ini, FILE* file);
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
        load_file(&ini);
    return ini;
}

Ini ini_alloc_from_str(const char* filename, const char* text) {
    assert(filename && ".ini filename is required");
    assert(text && ".ini text is required");
    Ini ini = {strdup(filename), vec_str_alloc(),
               vec_alloc(0, item_cmp, item_destroy)};
    parse_text(&ini, text);
    return ini;
}

void ini_free(Ini* ini) {
    free(ini->filename);
    vec_str_free(&ini->sections);
    vec_free(&ini->items);
}

static bool load_file(Ini* ini) {
    bool ok = true;
    char* text = NULL;
    FILE* file = fopen(ini->filename, "rb");
    if (!file) {
        ok = false;
        goto end;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    text = malloc(size + 1);
    fread(text, size, 1, file);
    parse_text(ini, text);
    if (fclose(file)) {
        ok = false;
        goto end;
    }
end:
    free(text);
    return ok;
}

bool parse_text(Ini* ini, const char* text) {
    // Use INI_UNNAMED_SECTION when needed
    return false; // TODO
}

char* ini_save_to_str(Ini* ini) {
    char* p = NULL;
    size_t size;
    FILE* file = open_memstream(&p, &size);
    if (!file)
        goto end;
    save_file(ini, file);
    if (fclose(file))
        goto end;
end:
    return p;
}

bool ini_save(Ini* ini) {
    bool ok = true;
    FILE* file = fopen(ini->filename, "w");
    if (!file) {
        ok = false;
        goto end;
    }
    save_file(ini, file);
    if (fclose(file)) {
        ok = false;
        goto end;
    }
end:
    return ok;
}

static void save_file(Ini* ini, FILE* file) {
    int prev_sectid = INI_NO_SECTION;
    vec_sort(&ini->items); // sectid x key
    for (int i = 0; i < vec_size(&ini->items); ++i) {
        const IniItem* item = vec_get(&ini->items, i);
        if (item->sectid != prev_sectid) { // new section
            prev_sectid = item->sectid;
            if (prev_sectid != INI_NO_SECTION) {
                const char* section =
                    vec_str_get(&ini->sections, prev_sectid);
                fprintf(file, "[%s]\n", section);
            }
        }
        if (item->comment)
            fprintf(file, "; %s\n", item->comment);
        fprintf(file, "%s = %s\n", item->key, item->value);
    }
}

bool ini_equal(Ini* ini1, Ini* ini2) {
    if (vec_str_size(&ini1->items) != vec_str_size(&ini2->items))
        return false;
    vec_sort(&ini1->items);
    vec_sort(&ini2->items);
    for (int i = 0; i < vec_size(&ini1->items); ++i) {
        const IniItem* item1 = vec_get(&ini1->items, i);
        const IniItem* item2 = vec_get(&ini2->items, i);
        if (!(item1->sectid == item2->sectid &&
              str_eq_fold(item1->key, item2->key) &&
              str_eq_fold(item1->value, item2->value)))
            return false;
    }
    return true;
}

static int find_sectid(const Ini* ini, const char* section) {
    if (!section)
        return INI_NO_SECTION;
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
