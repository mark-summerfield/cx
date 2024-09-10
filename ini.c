// Copyright © 2024 Mark Summerfield. All rights reserved.

#define _GNU_SOURCE // for asprintf

#include "ini.h"
#include "fx.h"
#include "str.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOT_FOUND -2

static Ini alloc(const char* filename);
static void read_file(Ini* ini);
static void parse_text(Ini* ini, const char* text);
static void save_to_file(Ini* ini, FILE* file);
static void item_write(const IniItem* item, FILE* file);
static IniItem* item_alloc(char* key, char* value, int sectid);
static void item_destroy(void* item_);
static int item_cmp(const void* item1_, const void* item2_);
static int find_sectid(const Ini* ini, const char* section);
static int maybe_add_section(Ini* ini, const char* section);
static IniItem* find_item(Ini* ini, const char* section, const char* key);

Ini ini_alloc(const char* filename) {
    Ini ini = alloc(filename);
    if (is_file(filename))
        read_file(&ini);
    return ini;
}

Ini ini_alloc_from_str(const char* filename, const char* text) {
    assert(text && ".ini text is required");
    Ini ini = alloc(filename);
    parse_text(&ini, text);
    return ini;
}

static Ini alloc(const char* filename) {
    assert(filename && ".ini filename is required");
    return (Ini){strdup(filename), NULL, vec_str_alloc(),
                 vec_alloc(0, item_cmp, item_destroy)};
}

void ini_free(Ini* ini) {
    vec_free(&ini->items);
    vec_str_free(&ini->sections);
    free(ini->comment);
    free(ini->filename);
}

static void read_file(Ini* ini) {
    char* text = NULL;
    FILE* file = fopen(ini->filename, "rb");
    if (!file) {
        warn(NULL);
        return;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    text = malloc(size + 1);
    fread(text, size, 1, file);
    if (fclose(file))
        warn(NULL);
    parse_text(ini, text);
    free(text);
}

// Every comment, section, and key-value pair occupies a single line.
// Comments:    /^\s*[;#].+$/
// Sections:    /^\[[^]+\]\s*$/
// Key-values:  /^[^[:=]+\s*[:=]\s*.+$/
static void parse_text(Ini* ini, const char* text) {
    // Use INI_NO_SECTION when needed
    // TODO
}

char* ini_save_to_str(Ini* ini) {
    char* p = NULL;
    size_t size;
    FILE* file = open_memstream(&p, &size);
    if (!file) {
        warn(NULL);
        return p;
    }
    save_to_file(ini, file);
    if (fclose(file))
        warn(NULL);
    return p;
}

bool ini_save(Ini* ini) {
    FILE* file = fopen(ini->filename, "w");
    if (!file) {
        warn(NULL);
        return false;
    }
    save_to_file(ini, file);
    if (fclose(file)) {
        warn(NULL);
        return false;
    }
    return true;
}

static void save_to_file(Ini* ini, FILE* file) {
    bool nl = false;
    if (ini->comment) {
        fprintf(file, "; %s\n", ini->comment);
        nl = true;
    }
    vec_sort(&ini->items); // sectid x key
    int prev_sectid = NOT_FOUND;
    for (int i = 0; i < vec_size(&ini->items); ++i) {
        const IniItem* item = vec_get(&ini->items, i);
        if (item->sectid != prev_sectid) { // new section
            prev_sectid = item->sectid;
            if (prev_sectid != INI_NO_SECTION) {
                const char* section =
                    vec_str_get(&ini->sections, prev_sectid);
                fprintf(file, "\n[%s]\n", section);
            } else if (nl)
                fprintf(file, "\n");
        }
        item_write(item, file);
    }
}

static void item_write(const IniItem* item, FILE* file) {
    if (item->comment)
        fprintf(file, "; %s\n", item->comment);
    fprintf(file, "%s = %s\n", item->key, item->value);
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
    for (int i = 0; i < vec_str_size(&ini->sections); ++i)
        if (str_eq_fold(section, vec_str_get(&ini->sections, i)))
            return i;
    return NOT_FOUND;
}

static IniItem* find_item(Ini* ini, const char* section, const char* key) {
    int sectid = find_sectid(ini, section);
    if (sectid == NOT_FOUND)
        return NULL;
    for (int i = 0; i < vec_size(&ini->items); ++i) {
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
        const char* v = bool_to_str(value);
        item = item_alloc(strdup(key), strdup(v), sectid);
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
        char* v;
        asprintf(&v, "%d", value);
        item = item_alloc(strdup(key), v, sectid);
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
        char* v;
        asprintf(&v, "%lg", value);
        item = item_alloc(strdup(key), v, sectid);
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
    if (!section && !key) {
        if (!ini->comment || !str_eq(ini->comment, comment)) {
            free(ini->comment);
            ini->comment = strdup(comment);
        }
        return true;
    }
    IniItem* item = find_item(ini, section, key);
    if (!item)
        return false;
    if (!item->comment || !str_eq(item->comment, comment)) {
        free(item->comment);
        item->comment = strdup(comment);
    }
    return true;
}

// Compares section & key only
static int item_cmp(const void* item1_, const void* item2_) {
    const IniItem* item1 = *(const IniItem**)item1_;
    const IniItem* item2 = *(const IniItem**)item2_;
    if (item1->sectid < item2->sectid)
        return -1;
    if (item1->sectid > item2->sectid)
        return 1;
    return strcasecmp(item1->key, item2->key);
}

// Takes ownership of key and value
static IniItem* item_alloc(char* key, char* value, int sectid) {
    IniItem* item = malloc(sizeof(IniItem));
    item->key = key;
    item->value = value;
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
