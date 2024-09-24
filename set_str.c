// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "set_str.h"
#include <stdlib.h>
#include <string.h>

static void add_to_difference(SetStr* set, const SetStrNode* node,
                              const SetStr* set2);
static void push_to_vec(VecStr* vec, const SetStrNode* node);
static void visit(SetStrNode* node, void* state, visitor_fn visitor);
static SetStrNode* node_add(SetStrNode* node, char* value, bool* added);
static SetStrNode* node_alloc(char* value);
static bool node_is_red(const SetStrNode* node);
static void node_color_flip(SetStrNode* node);
static SetStrNode* node_add_rotation(SetStrNode* node);
static SetStrNode* node_rotate_left(SetStrNode* node);
static SetStrNode* node_rotate_right(SetStrNode* node);
static void node_remove_all(SetStrNode* node, bool owns);
static SetStrNode* node_remove(SetStrNode* node, const char* value,
                               bool* deleted, bool owns);
static SetStrNode* node_move_red_left(SetStrNode* node);
static SetStrNode* node_move_red_right(SetStrNode* node);
static SetStrNode* node_remove_right(SetStrNode* node, const char* value,
                                     bool* deleted, bool owns);
static SetStrNode* node_fixup(SetStrNode* node);
static const SetStrNode* node_first(const SetStrNode* node);
static SetStrNode* node_remove_minimum(SetStrNode* node, bool owns);
static int node_max_depth(const SetStrNode* node);

inline SetStr set_str_alloc(bool owns) {
    return (SetStr){._root = NULL, ._size = 0, ._owns = owns};
}

inline void set_str_free(SetStr* set) { set_str_clear(set); }

void set_str_clear(SetStr* set) {
    assert_notnull(set);
    node_remove_all(set->_root, set->_owns);
    set->_root = NULL;
    set->_size = 0;
}

static void node_remove_all(SetStrNode* node, bool owns) {
    if (node) {
        node_remove_all(node->left, owns);
        node_remove_all(node->right, owns);
        if (owns)
            free(node->value);
        free(node);
    }
}

static SetStrNode* node_alloc(char* value) {
    assert_notnull(value);
    SetStrNode* node = malloc(sizeof(SetStrNode));
    assert_alloc(node);
    node->left = node->right = NULL;
    node->value = value;
    node->_red = true;
    return node;
}

bool set_str_add(SetStr* set, char* value) {
    assert_notnull(set);
    bool added = false;
    set->_root = node_add(set->_root, value, &added);
    set->_root->_red = false;
    if (added)
        set->_size++;
    return added;
}

static SetStrNode* node_add(SetStrNode* node, char* value, bool* added) {
    if (!node) { // If value was in the tree it would go here
        *added = true;
        return node_alloc(value);
    }
    if (node_is_red(node->left) && node_is_red(node->right))
        node_color_flip(node);
    int cmp = strcmp(value, node->value);
    if (cmp < 0)
        node->left = node_add(node->left, value, added);
    else if (cmp > 0)
        node->right = node_add(node->right, value, added);
    return node_add_rotation(node);
}

static inline bool node_is_red(const SetStrNode* node) {
    return node != NULL && node->_red;
}

static void node_color_flip(SetStrNode* node) {
    node->_red = !node->_red;
    if (node->left)
        node->left->_red = !node->left->_red;
    if (node->right)
        node->right->_red = !node->right->_red;
}

static SetStrNode* node_add_rotation(SetStrNode* node) {
    if (node_is_red(node->right) && !node_is_red(node->left))
        node = node_rotate_left(node);
    if (node_is_red(node->left) && node_is_red(node->left->left))
        node = node_rotate_right(node);
    return node;
}

static SetStrNode* node_rotate_left(SetStrNode* node) {
    SetStrNode* x = node->right;
    node->right = x->left;
    x->left = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static SetStrNode* node_rotate_right(SetStrNode* node) {
    SetStrNode* x = node->left;
    node->left = x->right;
    x->right = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

bool set_str_remove(SetStr* set, const char* value) {
    assert_notnull(set);
    bool deleted = false;
    if (set->_root) {
        set->_root = node_remove(set->_root, value, &deleted, set->_owns);
        if (set->_root)
            set->_root->_red = false;
    }
    if (deleted)
        set->_size--;
    return deleted;
}

static SetStrNode* node_remove(SetStrNode* node, const char* value,
                               bool* deleted, bool owns) {
    int cmp = strcmp(value, node->value);
    if (cmp < 0) {
        if (node->left) {
            if (!node_is_red(node->left) && !node_is_red(node->left->left))
                node = node_move_red_left(node);
            node->left = node_remove(node->left, value, deleted, owns);
        }
    } else {
        if (node_is_red(node->left))
            node = node_rotate_right(node);
        if (cmp == 0 && !node->right) {
            if (owns)
                free(node->value);
            free(node);
            *deleted = true;
            return NULL;
        }
        if (node->right)
            node = node_remove_right(node, value, deleted, owns);
    }
    return node_fixup(node);
}

static SetStrNode* node_move_red_left(SetStrNode* node) {
    node_color_flip(node);
    if (node->right && node_is_red(node->right->left)) {
        node->right = node_rotate_right(node->right);
        node = node_rotate_left(node);
        node_color_flip(node);
    }
    return node;
}

static SetStrNode* node_move_red_right(SetStrNode* node) {
    node_color_flip(node);
    if (node->left && node_is_red(node->left->left)) {
        node = node_rotate_right(node);
        node_color_flip(node);
    }
    return node;
}

static SetStrNode* node_remove_right(SetStrNode* node, const char* value,
                                     bool* deleted, bool owns) {
    if (!node_is_red(node->right) && !node_is_red(node->right->left))
        node = node_move_red_right(node);
    if (strcmp(value, node->value) == 0) {
        const SetStrNode* first = node_first(node->right);
        if (owns)
            free(node->value);
        node->value = first->value;
        node->right = node_remove_minimum(node->right, owns);
        *deleted = true;
    } else
        node->right = node_remove(node->right, value, deleted, owns);
    return node;
}

static SetStrNode* node_fixup(SetStrNode* node) {
    if (node_is_red(node->right))
        node = node_rotate_left(node);
    if (node_is_red(node->left) && node_is_red(node->left->left))
        node = node_rotate_right(node);
    if (node_is_red(node->left) && node_is_red(node->right))
        node_color_flip(node);
    return node;
}

static const SetStrNode* node_first(const SetStrNode* node) {
    while (node->left)
        node = node->left;
    return node;
}

static SetStrNode* node_remove_minimum(SetStrNode* node, bool owns) {
    if (!node->left) {
        if (owns)
            free(node->value);
        free(node);
        return NULL;
    }
    if (!node_is_red(node->left) && !node_is_red(node->left->left))
        node = node_move_red_left(node);
    node->left = node_remove_minimum(node->left, owns);
    return node_fixup(node);
}

SetStr set_str_copy(const SetStr* set, bool owns) {
    assert_notnull(set);
    SetStr set2 = set_str_alloc(owns);
    VecStr vec = set_str_to_vec(set, BORROWS);
    for (int i = 0; i < VEC_SIZE(&vec); i++) {
        char* value = vec_str_get(&vec, i);
        set_str_add(&set2, owns ? strdup(value) : value);
    }
    vec_str_free(&vec);
    return set2;
}

bool set_str_equal(const SetStr* set1, const SetStr* set2) {
    assert_notnull(set1);
    assert_notnull(set2);
    if (set1->_size != set2->_size)
        return false;
    VecStr vec1 = set_str_to_vec(set1, BORROWS);
    VecStr vec2 = set_str_to_vec(set2, BORROWS);
    bool equal = vec_str_equal(&vec1, &vec2);
    vec_str_free(&vec2);
    vec_str_free(&vec1);
    return equal;
}

bool set_str_contains(const SetStr* set, const char* value) {
    assert_notnull(set);
    SetStrNode* node = set->_root;
    while (node) {
        int cmp = strcmp(value, node->value);
        if (cmp < 0)
            node = node->left;
        else if (cmp > 0)
            node = node->right;
        else
            return true;
    }
    return false;
}

SetStr set_str_difference(const SetStr* set1, const SetStr* set2,
                          bool owns) {
    assert_notnull(set1);
    assert_notnull(set2);
    SetStr set = set_str_alloc(owns);
    add_to_difference(&set, set1->_root, set2);
    return set;
}

static void add_to_difference(SetStr* set, const SetStrNode* node,
                              const SetStr* set2) {
    if (node) {
        if (!set_str_contains(set2, node->value))
            set_str_add(set, node->value);
        add_to_difference(set, node->left, set2);
        add_to_difference(set, node->right, set2);
    }
}

SetStr set_str_intersection(const SetStr* set1, const SetStr* set2,
                            bool owns) {
    assert_notnull(set1);
    assert_notnull(set2);
    SetStr set = set_str_alloc(owns);
    VecStr vec1 = set_str_to_vec(set1, BORROWS);
    VecStr vec2 = set_str_to_vec(set2, BORROWS);
    int i = 0;
    int j = 0;
    while (i < vec1._size && j < vec2._size) {
        char* v1 = vec1._values[i];
        char* v2 = vec2._values[j];
        int cmp = strcmp(v1, v2);
        if (cmp == 0) {
            set_str_add(&set, owns ? strdup(v1) : v1);
            i++;
            j++;
        } else if (cmp < 0)
            i++;
        else
            j++;
    }
    vec_str_free(&vec2);
    vec_str_free(&vec1);
    return set;
}

SetStr set_str_union(const SetStr* set1, const SetStr* set2, bool owns) {
    assert_notnull(set1);
    assert_notnull(set2);
    SetStr set = set_str_copy(set1, owns);
    set_str_unite(&set, set2);
    return set;
}

void set_str_unite(SetStr* set1, const SetStr* set2) {
    assert_notnull(set1);
    assert_notnull(set2);
    VecStr vec = set_str_to_vec(set2, BORROWS);
    if (set1->_owns) // only add new values to avoid freeing dups
        for (int i = 0; i < VEC_SIZE(&vec); ++i) {
            char* value = vec_str_get(&vec, i);
            if (!set_str_contains(set1, value))
                set_str_add(set1, strdup(value));
        }
    else // no problem to add (or not if dup) borrowed pointer
        for (int i = 0; i < VEC_SIZE(&vec); ++i)
            set_str_add(set1, vec_str_get(&vec, i));
    vec_str_free(&vec);
}

VecStr set_str_to_vec(const SetStr* set, bool owns) {
    assert_notnull(set);
    VecStr vec = vec_str_alloc_custom(set->_size, owns);
    push_to_vec(&vec, set->_root);
    return vec;
}

static void push_to_vec(VecStr* vec, const SetStrNode* node) {
    if (node) {
        push_to_vec(vec, node->left);
        vec_str_push(vec,
                     vec_str_owns(vec) ? strdup(node->value) : node->value);
        push_to_vec(vec, node->right);
    }
}

char* set_str_join(const SetStr* set, const char* sep) {
    assert_notnull(set);
    VecStr vec = set_str_to_vec(set, BORROWS);
    char* s = vec_str_join(&vec, sep);
    vec_str_free(&vec);
    return s;
}

void set_str_visit(const SetStr* set, void* state, visitor_fn visitor) {
    assert_notnull(set);
    visit(set->_root, state, visitor);
}

static void visit(SetStrNode* node, void* state, visitor_fn visitor) {
    if (node) {
        visit(node->left, state, visitor);
        visitor(node->value, state);
        visit(node->right, state, visitor);
    }
}

inline int set_str_max_depth(const SetStr* set) {
    return node_max_depth(set->_root);
}

static int node_max_depth(const SetStrNode* node) {
    if (!node)
        return 0;
    int left = node_max_depth(node->left);
    int right = node_max_depth(node->right);
    return (left > right) ? (left + 1) : (right + 1);
}
