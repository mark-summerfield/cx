// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "map_str_real.h"
#include <stdlib.h>
#include <string.h>

static MapStrRealNode* node_add(MapStrRealNode* node, char* key,
                                double value, bool* added,
                                Ownership ownership);
static MapStrRealNode* node_alloc(char* key, double value);
static bool node_is_red(const MapStrRealNode* node);
static void node_color_flip(MapStrRealNode* node);
static MapStrRealNode* node_add_rotation(MapStrRealNode* node);
static MapStrRealNode* node_rotate_left(MapStrRealNode* node);
static MapStrRealNode* node_rotate_right(MapStrRealNode* node);
static void node_remove_all(MapStrRealNode* node, Ownership ownership);
static MapStrRealNode* node_remove(MapStrRealNode* node, const char* key,
                                   bool* deleted, Ownership ownership);
static MapStrRealNode* node_move_red_left(MapStrRealNode* node);
static MapStrRealNode* node_move_red_right(MapStrRealNode* node);
static MapStrRealNode* node_remove_right(MapStrRealNode* node,
                                         const char* key, bool* deleted,
                                         Ownership ownership);
static MapStrRealNode* node_fixup(MapStrRealNode* node);
static const MapStrRealNode* node_first(const MapStrRealNode* node);
static MapStrRealNode* node_remove_minimum(MapStrRealNode* node,
                                           Ownership ownership);
static int str_real_pair_cmp(const void* p1, const void* p2);
static void push_to_vec(Vec* vec, const MapStrRealNode* node);
static void push_to_vec_str(VecStr* vec, const MapStrRealNode* node);

inline MapStrReal map_str_real_alloc(Ownership ownership) {
    return (MapStrReal){._root = NULL, ._size = 0, ._ownership = ownership};
}

inline void map_str_real_free(MapStrReal* map) { map_str_real_clear(map); }

void map_str_real_clear(MapStrReal* map) {
    assert_notnull(map);
    node_remove_all(map->_root, map->_ownership);
    map->_root = NULL;
    map->_size = 0;
}

static void node_remove_all(MapStrRealNode* node, Ownership ownership) {
    if (node) {
        node_remove_all(node->left, ownership);
        node_remove_all(node->right, ownership);
        if (ownership == Owns)
            free(node->key);
        free(node);
    }
}

static MapStrRealNode* node_alloc(char* key, double value) {
    assert_notnull(key);
    MapStrRealNode* node = malloc(sizeof(MapStrRealNode));
    assert_alloc(node);
    node->left = node->right = NULL;
    node->key = key;
    node->value = value;
    node->_red = true;
    return node;
}

bool map_str_real_set(MapStrReal* map, char* key, double value) {
    assert_notnull(map);
    bool added = false;
    map->_root = node_add(map->_root, key, value, &added, map->_ownership);
    map->_root->_red = false;
    if (added)
        map->_size++;
    return added;
}

static MapStrRealNode* node_add(MapStrRealNode* node, char* key,
                                double value, bool* added,
                                Ownership ownership) {
    if (!node) { // If key was in the tree it would go here
        *added = true;
        return node_alloc(key, value);
    }
    if (node_is_red(node->left) && node_is_red(node->right))
        node_color_flip(node);
    int cmp = strcmp(key, node->key);
    if (cmp == 0) {
        if (ownership == Owns && node->key != key) {
            free(node->key); // both keys have the same string but
            node->key = key; // are different pointers
        }
        node->value = value;
        return node;
    }
    if (cmp < 0)
        node->left = node_add(node->left, key, value, added, ownership);
    else // cmp > 0
        node->right = node_add(node->right, key, value, added, ownership);
    return node_add_rotation(node);
}

static inline bool node_is_red(const MapStrRealNode* node) {
    return node != NULL && node->_red;
}

static void node_color_flip(MapStrRealNode* node) {
    node->_red = !node->_red;
    if (node->left)
        node->left->_red = !node->left->_red;
    if (node->right)
        node->right->_red = !node->right->_red;
}

static MapStrRealNode* node_add_rotation(MapStrRealNode* node) {
    if (node_is_red(node->right) && !node_is_red(node->left))
        node = node_rotate_left(node);
    if (node_is_red(node->left) && node_is_red(node->left->left))
        node = node_rotate_right(node);
    return node;
}

static MapStrRealNode* node_rotate_left(MapStrRealNode* node) {
    MapStrRealNode* x = node->right;
    node->right = x->left;
    x->left = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static MapStrRealNode* node_rotate_right(MapStrRealNode* node) {
    MapStrRealNode* x = node->left;
    node->left = x->right;
    x->right = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

bool map_str_real_remove(MapStrReal* map, const char* key) {
    assert_notnull(map);
    bool deleted = false;
    if (map->_root) {
        map->_root =
            node_remove(map->_root, key, &deleted, map->_ownership);
        if (map->_root)
            map->_root->_red = false;
    }
    if (deleted)
        map->_size--;
    return deleted;
}

static MapStrRealNode* node_remove(MapStrRealNode* node, const char* key,
                                   bool* deleted, Ownership ownership) {
    int cmp = strcmp(key, node->key);
    if (cmp < 0) {
        if (node->left) {
            if (!node_is_red(node->left) && !node_is_red(node->left->left))
                node = node_move_red_left(node);
            node->left = node_remove(node->left, key, deleted, ownership);
        }
    } else {
        if (node_is_red(node->left))
            node = node_rotate_right(node);
        if (cmp == 0 && !node->right) {
            if (ownership == Owns)
                free(node->key);
            free(node);
            *deleted = true;
            return NULL;
        }
        if (node->right)
            node = node_remove_right(node, key, deleted, ownership);
    }
    return node_fixup(node);
}

static MapStrRealNode* node_move_red_left(MapStrRealNode* node) {
    node_color_flip(node);
    if (node->right && node_is_red(node->right->left)) {
        node->right = node_rotate_right(node->right);
        node = node_rotate_left(node);
        node_color_flip(node);
    }
    return node;
}

static MapStrRealNode* node_move_red_right(MapStrRealNode* node) {
    node_color_flip(node);
    if (node->left && node_is_red(node->left->left)) {
        node = node_rotate_right(node);
        node_color_flip(node);
    }
    return node;
}

static MapStrRealNode* node_remove_right(MapStrRealNode* node,
                                         const char* key, bool* deleted,
                                         Ownership ownership) {
    if (!node_is_red(node->right) && !node_is_red(node->right->left))
        node = node_move_red_right(node);
    if (strcmp(key, node->key) == 0) {
        const MapStrRealNode* first = node_first(node->right);
        if (ownership == Owns)
            free(node->key);
        node->key = first->key;
        node->right = node_remove_minimum(node->right, ownership);
        *deleted = true;
    } else
        node->right = node_remove(node->right, key, deleted, ownership);
    return node;
}

static MapStrRealNode* node_fixup(MapStrRealNode* node) {
    if (node_is_red(node->right))
        node = node_rotate_left(node);
    if (node_is_red(node->left) && node_is_red(node->left->left))
        node = node_rotate_right(node);
    if (node_is_red(node->left) && node_is_red(node->right))
        node_color_flip(node);
    return node;
}

static const MapStrRealNode* node_first(const MapStrRealNode* node) {
    while (node->left)
        node = node->left;
    return node;
}

static MapStrRealNode* node_remove_minimum(MapStrRealNode* node,
                                           Ownership ownership) {
    if (!node->left) {
        if (ownership == Owns)
            free(node->key);
        free(node);
        return NULL;
    }
    if (!node_is_red(node->left) && !node_is_red(node->left->left))
        node = node_move_red_left(node);
    node->left = node_remove_minimum(node->left, ownership);
    return node_fixup(node);
}

double map_str_real_get(const MapStrReal* map, const char* key, bool* ok) {
    assert_notnull(map);
    MapStrRealNode* node = map->_root;
    while (node) {
        int cmp = strcmp(key, node->key);
        if (cmp < 0)
            node = node->left;
        else if (cmp > 0)
            node = node->right;
        else {
            *ok = true;
            return node->value;
        }
    }
    *ok = false;
    return 0.0;
}

bool map_str_real_contains(const MapStrReal* map, const char* key) {
    assert_notnull(map);
    MapStrRealNode* node = map->_root;
    while (node) {
        int cmp = strcmp(key, node->key);
        if (cmp < 0)
            node = node->left;
        else if (cmp > 0)
            node = node->right;
        else
            return true;
    }
    return false;
}

static int str_real_pair_cmp(const void* p1, const void* p2) {
    return strcmp((*(const StrRealPair**)p1)->key,
                  (*(const StrRealPair**)p2)->key);
}

Vec map_str_real_to_vec(const MapStrReal* map) {
    assert_notnull(map);
    Vec vec = vec_alloc(map->_size, str_real_pair_cmp, NULL);
    push_to_vec(&vec, map->_root);
    return vec;
}

static void push_to_vec(Vec* vec, const MapStrRealNode* node) {
    if (node) {
        push_to_vec(vec, node->left);
        StrRealPair* pair = malloc(sizeof(StrRealPair));
        assert_alloc(pair);
        pair->key = node->key;
        pair->value = node->value;
        vec_push(vec, pair);
        push_to_vec(vec, node->right);
    }
}

VecStr map_str_real_keys(const MapStrReal* map) {
    assert_notnull(map);
    VecStr vec = vec_str_alloc_custom(map->_size, Borrows);
    push_to_vec_str(&vec, map->_root);
    return vec;
}

static void push_to_vec_str(VecStr* vec, const MapStrRealNode* node) {
    if (node) {
        push_to_vec_str(vec, node->left);
        vec_str_push(vec, node->key);
        push_to_vec_str(vec, node->right);
    }
}
