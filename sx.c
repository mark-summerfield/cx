// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "sx.h"
#include "cx.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static int partition(char** array, size_t low, size_t high);
static void swap(char** array, size_t i, size_t j);

bool sx_ends(const char* s, const char* suffix) {
    size_t size = strlen(s);
    if (size < strlen(suffix))
        return false;
    return sx_eq(s + size - strlen(suffix), suffix);
}

bool sx_ends_fold(const char* s, const char* suffix) {
    size_t size = strlen(s);
    if (size < strlen(suffix))
        return false;
    return sx_eq_fold(s + size - strlen(suffix), suffix);
}

void sx_uppercase_ip(char* s) {
    assert(s && "can't uppercase NULL");
    while (*s) {
        *s = toupper(*s);
        s++;
    }
}

char* sx_uppercase_alloc(const char* s) {
    assert(s && "can't uppercase NULL");
    char* u = malloc(strlen(s) + 1);
    assert_alloc(u);
    int i = 0;
    while (*s) {
        u[i++] = toupper(*s);
        s++;
    }
    u[i] = 0;
    return u;
}

void sx_lowercase_ip(char* s) {
    assert(s && "can't lowercase NULL");
    while (*s) {
        *s = tolower(*s);
        s++;
    }
}

char* sx_lowercase_alloc(const char* s) {
    assert(s && "can't lowercase NULL");
    char* u = malloc(strlen(s) + 1);
    assert_alloc(u);
    int i = 0;
    while (*s) {
        u[i++] = tolower(*s);
        s++;
    }
    u[i] = 0;
    return u;
}

void sx_qsort(char** array, size_t low, size_t high) {
    if (low < high) {
        size_t partition_index = partition(array, low, high);
        sx_qsort(array, low, partition_index - 1);
        sx_qsort(array, partition_index + 1, high);
    }
}

static int partition(char** array, size_t low, size_t high) {
    const char* pivot = array[low];
    size_t i = low;
    size_t j = high;
    while (i < j) {
        while (i <= high - 1 && (strcmp(array[i], pivot) <= 0))
            i++;
        while (j >= low + 1 && (strcmp(array[j], pivot) > 0))
            j--;
        if (i < j)
            swap(array, i, j);
    }
    swap(array, low, j);
    return j;
}

static void swap(char** array, size_t i, size_t j) {
    char* t = array[i];
    array[i] = array[j];
    array[j] = t;
}
