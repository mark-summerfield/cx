// Copyright © 2024 Mark Summerfield. All rights reserved.

#define _GNU_SOURCE // for asprintf

#include "ini.h"
#include "fx.h"
#include "mx.h"
#include "str.h"
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOT_FOUND -2
#define LINE_MAX 1024

static void parse_text(Ini* ini, const char* text);
static void parse_line(Ini* ini, const char* line, char* section);
static void parse_item(Ini* ini, const char* p, const char* section);
static void save_to_stream(Ini* ini, FILE* file);
static void item_write(const IniItem* item, FILE* file);
static IniItem* item_alloc(char* key, char* value, int sectid);
static void item_destroy(void* item_);
static int item_cmp(const void* item1_, const void* item2_);
static int find_sectid(const Ini* ini, const char* section);
static int maybe_add_section(Ini* ini, const char* section);
static IniItem* find_item(Ini* ini, const char* section, const char* key);

inline Ini ini_alloc() {
    return (Ini){NULL, vec_str_alloc(),
                 vec_alloc(0, item_cmp, item_destroy)};
}

Ini ini_alloc_from_file(const char* filename, bool* ok) {
    Ini ini = ini_alloc();
    bool reply = ini_merge_from_file(&ini, filename);
    if (ok)
        *ok = reply;
    return ini;
}

Ini ini_alloc_from_str(const char* text) {
    Ini ini = ini_alloc();
    ini_merge_from_str(&ini, text);
    return ini;
}

bool ini_merge_from_file(Ini* ini, const char* filename) {
    assert(filename && ".ini filename is required");
    bool ok;
    char* text = file_read(filename, &ok);
    parse_text(ini, text);
    free(text);
    return ok;
}

void ini_merge_from_str(Ini* ini, const char* text) {
    assert(text && ".ini text is required");
    parse_text(ini, text);
}

void ini_free(Ini* ini) {
    vec_free(&ini->items);
    vec_str_free(&ini->sections);
    free(ini->comment);
}

static void parse_text(Ini* ini, const char* text) {
    char line[LINE_MAX] = "";
    char section[LINE_MAX] = "";
    const char* p = text;
    while (p) {
        const char* q = strchr(p, '\n');
        size_t size = min(LINE_MAX - 1, q ? (int)(q - p) : (int)strlen(p));
        strncpy(line, p, size);
        line[size] = 0;
        parse_line(ini, line, section);
        if (!q)
            break;
        p = q + 1;
    }
}

// Every comment, section, and key-value pair occupies a single line.
// Comments:    /^\s*[;#].+$/
// Sections:    /^\[[^]+\]\s*$/
// Key-values:  /^[^[:=]+\s*[:=]\s*.+$/
static void parse_line(Ini* ini, const char* line, char* section) {
    const char* p = str_trim_left(line);
    if (!p || !*p || *p == ';' || *p == '#')
        return;      // skip blank lines and comments
    if (*p == '[') { // section
        p++;
        const char* q = strchr(p, ']');
        if (q) {
            size_t size = min(LINE_MAX - 1, (int)(q - p));
            strncpy(section, p, size);
            section[size] = 0;
        } else
            warn("invalid section: %s\n", p);
    } else
        parse_item(ini, p, section);
}

static void parse_item(Ini* ini, const char* p, const char* section) {
    size_t i = strcspn(p, "=:");
    if (!i) {
        warn("invalid key-value item, no = or : separator: %s\n", p);
        return;
    }
    const char* q = p + i - 1; // set q to just _before_ the =
    char* key = str_trimn(p, q - p);
    q += 2; // skip _past_ the =
    if (!q) {
        warn("invalid key-value item, missing value: %s\n", p);
        return;
    }
    char* value = str_trim(q);
    int sectid = maybe_add_section(ini, section);
    IniItem* item = find_item(ini, section, key);
    if (item) { // duplicate entry; replace value
        if (!str_caseeq(item->value, value)) {
            free(item->value);
            item->value = value;
        }
    } else {
        item = item_alloc(key, value, sectid); // item owns key and value
        vec_push(&ini->items, item);
    }
}

bool ini_save(Ini* ini, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        warn(NULL);
        return false;
    }
    save_to_stream(ini, file);
    if (fclose(file)) {
        warn(NULL);
        return false;
    }
    return true;
}

char* ini_save_to_str(Ini* ini) {
    char* p = NULL;
    size_t size;
    FILE* file = open_memstream(&p, &size);
    if (!file) {
        warn(NULL);
        return p;
    }
    save_to_stream(ini, file);
    if (fclose(file))
        warn(NULL);
    return p;
}

static void save_to_stream(Ini* ini, FILE* file) {
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

static int find_sectid(const Ini* ini, const char* section) {
    if (!section || !strlen(section))
        return INI_NO_SECTION;
    for (int i = 0; i < vec_str_size(&ini->sections); ++i)
        if (str_caseeq(section, vec_str_get(&ini->sections, i)))
            return i;
    return NOT_FOUND;
}

static IniItem* find_item(Ini* ini, const char* section, const char* key) {
    int sectid = find_sectid(ini, section);
    if (sectid == NOT_FOUND)
        return NULL;
    for (int i = 0; i < vec_size(&ini->items); ++i) {
        IniItem* item = vec_get(&ini->items, i);
        if (item->sectid == sectid && str_caseeq(item->key, key))
            return item;
    }
    return NULL;
}

static int maybe_add_section(Ini* ini, const char* section) {
    int sectid = find_sectid(ini, section);
    if (sectid == NOT_FOUND) {
        vec_str_push(&ini->sections, str_trim(section));
        sectid = vec_str_size(&ini->sections) - 1;
    }
    return sectid;
}

IniReply ini_get_bool(const Ini* ini, const char* section, const char* key,
                      bool* value) {
    const IniItem* item = find_item((Ini*)ini, section, key);
    if (!item)
        return IniItemNotFound;
    if (str_caseeq(item->value, "true") || str_caseeq(item->value, "t") ||
        str_caseeq(item->value, "yes") || str_caseeq(item->value, "y") ||
        str_caseeq(item->value, "on") || str_caseeq(item->value, "1")) {
        *value = true;
        return IniItemFound;
    }
    if (str_caseeq(item->value, "false") || str_caseeq(item->value, "f") ||
        str_caseeq(item->value, "no") || str_caseeq(item->value, "n") ||
        str_caseeq(item->value, "off") || str_caseeq(item->value, "0")) {
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
        if (!str_caseeq(item->value, s)) {
            free(item->value);
            item->value = str_trim(s);
        }
    } else {
        int sectid = maybe_add_section(ini, section);
        const char* v = bool_to_str(value);
        item = item_alloc(str_trim(key), str_trim(v), sectid);
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
            if (asprintf(&item->value, "%d", value) == -1)
                warn(NULL);
        }
    } else {
        int sectid = maybe_add_section(ini, section);
        char* v;
        if (asprintf(&v, "%d", value) == -1)
            warn(NULL);
        item = item_alloc(str_trim(key), v, sectid);
        vec_push(&ini->items, item);
    }
}

void ini_set_real(Ini* ini, const char* section, const char* key,
                  double value) {
    IniItem* item = find_item(ini, section, key);
    if (item) {
        free(item->value);
        if (asprintf(&item->value, "%lg", value) == -1)
            warn(NULL);
    } else {
        int sectid = maybe_add_section(ini, section);
        char* v;
        if (asprintf(&v, "%lg", value) == -1)
            warn(NULL);
        item = item_alloc(str_trim(key), v, sectid);
        vec_push(&ini->items, item);
    }
}

void ini_set_str(Ini* ini, const char* section, const char* key,
                 const char* value) {
    IniItem* item = find_item(ini, section, key);
    if (item) {
        if (!str_eq(item->value, value)) {
            free(item->value);
            item->value = str_trim(value);
        }
    } else {
        int sectid = maybe_add_section(ini, section);
        item = item_alloc(str_trim(key), str_trim(value), sectid);
        vec_push(&ini->items, item);
    }
}

bool ini_set_comment(Ini* ini, const char* section, const char* key,
                     const char* comment) {
    if (!section && !key) {
        if (!ini->comment || !str_eq(ini->comment, comment)) {
            free(ini->comment);
            ini->comment = str_trim(comment);
        }
        return true;
    }
    IniItem* item = find_item(ini, section, key);
    if (!item)
        return false;
    if (!item->comment || !str_eq(item->comment, comment)) {
        free(item->comment);
        item->comment = str_trim(comment);
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
    assert_alloc(item);
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
