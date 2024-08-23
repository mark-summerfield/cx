// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "set_int.h"
#include <stdlib.h>
#include <string.h>

static void add_to_difference(SetInt* set, const SetIntNode* node,
                              const SetInt* set2);
static void push_to_vec(VecInt* vec, const SetIntNode* node);
static SetIntNode* node_add(SetIntNode* node, int value, bool* added);
static SetIntNode* node_alloc(int value);
static bool node_is_red(const SetIntNode* node);
static void node_color_flip(SetIntNode* node);
static SetIntNode* node_add_rotation(SetIntNode* node);
static SetIntNode* node_rotate_left(SetIntNode* node);
static SetIntNode* node_rotate_right(SetIntNode* node);
static void node_remove_all(SetIntNode* node);
static SetIntNode* node_remove(SetIntNode* node, int value, bool* deleted);
static SetIntNode* node_move_red_left(SetIntNode* node);
static SetIntNode* node_move_red_right(SetIntNode* node);
static SetIntNode* node_remove_right(SetIntNode* node, int value,
                                     bool* deleted);
static SetIntNode* node_fixup(SetIntNode* node);
static const SetIntNode* node_first(const SetIntNode* node);
static SetIntNode* node_remove_minimum(SetIntNode* node);
static SetIntNode* node_copy(const SetIntNode* node);
static int node_max_depth(const SetIntNode* node);

inline SetInt set_int_alloc() {
    return (SetInt){
        ._root = NULL,
        ._size = 0,
    };
}

inline void set_int_free(SetInt* set) { set_int_clear(set); }

void set_int_clear(SetInt* set) {
    assert_notnull(set);
    node_remove_all(set->_root);
    set->_root = NULL;
    set->_size = 0;
}

static void node_remove_all(SetIntNode* node) {
    if (node) {
        node_remove_all(node->left);
        node_remove_all(node->right);
        free(node);
    }
}

static SetIntNode* node_alloc(int value) {
    SetIntNode* node = malloc(sizeof(SetIntNode));
    assert_alloc(node);
    node->left = node->right = NULL;
    node->value = value;
    node->_red = true;
    return node;
}

bool set_int_add(SetInt* set, int value) {
    assert_notnull(set);
    bool added = false;
    set->_root = node_add(set->_root, value, &added);
    set->_root->_red = false;
    if (added)
        set->_size++;
    return added;
}

static SetIntNode* node_add(SetIntNode* node, int value, bool* added) {
    if (!node) { // If value was in the tree it would go here
        *added = true;
        return node_alloc(value);
    }
    if (node_is_red(node->left) && node_is_red(node->right))
        node_color_flip(node);
    if (value < node->value)
        node->left = node_add(node->left, value, added);
    else if (value > node->value)
        node->right = node_add(node->right, value, added);
    return node_add_rotation(node);
}

static inline bool node_is_red(const SetIntNode* node) {
    return node != NULL && node->_red;
}

static void node_color_flip(SetIntNode* node) {
    node->_red = !node->_red;
    if (node->left)
        node->left->_red = !node->left->_red;
    if (node->right)
        node->right->_red = !node->right->_red;
}

static SetIntNode* node_add_rotation(SetIntNode* node) {
    if (node_is_red(node->right) && !node_is_red(node->left))
        node = node_rotate_left(node);
    if (node_is_red(node->left) && node_is_red(node->left->left))
        node = node_rotate_right(node);
    return node;
}

static SetIntNode* node_rotate_left(SetIntNode* node) {
    SetIntNode* x = node->right;
    node->right = x->left;
    x->left = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static SetIntNode* node_rotate_right(SetIntNode* node) {
    SetIntNode* x = node->left;
    node->left = x->right;
    x->right = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

bool set_int_remove(SetInt* set, int value) {
    assert_notnull(set);
    bool deleted = false;
    if (set->_root) {
        set->_root = node_remove(set->_root, value, &deleted);
        if (set->_root)
            set->_root->_red = false;
    }
    if (deleted)
        set->_size--;
    return deleted;
}

static SetIntNode* node_remove(SetIntNode* node, int value, bool* deleted) {
    if (value < node->value) {
        if (node->left) {
            if (!node_is_red(node->left) && !node_is_red(node->left->left))
                node = node_move_red_left(node);
            node->left = node_remove(node->left, value, deleted);
        }
    } else {
        if (node_is_red(node->left))
            node = node_rotate_right(node);
        if (value == node->value && !node->right) {
            free(node);
            *deleted = true;
            return NULL;
        }
        if (node->right)
            node = node_remove_right(node, value, deleted);
    }
    return node_fixup(node);
}

static SetIntNode* node_move_red_left(SetIntNode* node) {
    node_color_flip(node);
    if (node->right && node_is_red(node->right->left)) {
        node->right = node_rotate_right(node->right);
        node = node_rotate_left(node);
        node_color_flip(node);
    }
    return node;
}

static SetIntNode* node_move_red_right(SetIntNode* node) {
    node_color_flip(node);
    if (node->left && node_is_red(node->left->left)) {
        node = node_rotate_right(node);
        node_color_flip(node);
    }
    return node;
}

static SetIntNode* node_remove_right(SetIntNode* node, int value,
                                     bool* deleted) {
    if (!node_is_red(node->right) && !node_is_red(node->right->left))
        node = node_move_red_right(node);
    if (value == node->value) {
        const SetIntNode* first = node_first(node->right);
        node->value = first->value;
        node->right = node_remove_minimum(node->right);
        *deleted = true;
    } else
        node->right = node_remove(node->right, value, deleted);
    return node;
}

static SetIntNode* node_fixup(SetIntNode* node) {
    if (node_is_red(node->right))
        node = node_rotate_left(node);
    if (node_is_red(node->left) && node_is_red(node->left->left))
        node = node_rotate_right(node);
    if (node_is_red(node->left) && node_is_red(node->right))
        node_color_flip(node);
    return node;
}

static const SetIntNode* node_first(const SetIntNode* node) {
    while (node->left)
        node = node->left;
    return node;
}

static SetIntNode* node_remove_minimum(SetIntNode* node) {
    if (!node->left) {
        free(node);
        return NULL;
    }
    if (!node_is_red(node->left) && !node_is_red(node->left->left))
        node = node_move_red_left(node);
    node->left = node_remove_minimum(node->left);
    return node_fixup(node);
}

inline SetInt set_int_copy(const SetInt* set) {
    assert_notnull(set);
    return (SetInt){._root = node_copy(set->_root), ._size = set->_size};
}

static SetIntNode* node_copy(const SetIntNode* node) {
    if (!node)
        return NULL;
    SetIntNode* copy = node_alloc(node->value);
    copy->left = node_copy(node->left);
    copy->right = node_copy(node->right);
    return copy;
}

bool set_int_equal(const SetInt* set1, const SetInt* set2) {
    assert_notnull(set1);
    assert_notnull(set2);
    if (set1->_size != set2->_size)
        return false;
    VecInt vec1 = set_int_to_vec(set1);
    VecInt vec2 = set_int_to_vec(set2);
    bool equal = vec_int_equal(&vec1, &vec2);
    vec_int_free(&vec2);
    vec_int_free(&vec1);
    return equal;
}

bool set_int_contains(const SetInt* set, int value) {
    assert_notnull(set);
    SetIntNode* node = set->_root;
    while (node) {
        if (value < node->value)
            node = node->left;
        else if (node->value < value)
            node = node->right;
        else
            return true;
    }
    return false;
}

SetInt set_int_difference(const SetInt* set1, const SetInt* set2) {
    assert_notnull(set1);
    assert_notnull(set2);
    SetInt set = set_int_alloc();
    add_to_difference(&set, set1->_root, set2);
    return set;
}

static void add_to_difference(SetInt* set, const SetIntNode* node,
                              const SetInt* set2) {
    if (node) {
        if (!set_int_contains(set2, node->value))
            set_int_add(set, node->value);
        add_to_difference(set, node->left, set2);
        add_to_difference(set, node->right, set2);
    }
}

SetInt set_int_intersection(const SetInt* set1, const SetInt* set2) {
    assert_notnull(set1);
    assert_notnull(set2);
    SetInt set = set_int_alloc();
    VecInt vec1 = set_int_to_vec(set1);
    VecInt vec2 = set_int_to_vec(set2);
    int i = 0;
    int j = 0;
    while (i < vec1._size && j < vec2._size) {
        int v1 = vec1._values[i];
        int v2 = vec2._values[j];
        if (v1 == v2) {
            set_int_add(&set, v1);
            i++;
            j++;
        } else if (v1 < v2)
            i++;
        else
            j++;
    }
    vec_int_free(&vec2);
    vec_int_free(&vec1);
    return set;
}

SetInt set_int_union(const SetInt* set1, const SetInt* set2) {
    assert_notnull(set1);
    assert_notnull(set2);
    SetInt set = set_int_copy(set1);
    set_int_unite(&set, set2);
    return set;
}

void set_int_unite(SetInt* set1, const SetInt* set2) {
    assert_notnull(set1);
    assert_notnull(set2);
    VecInt vec = set_int_to_vec(set2);
    for (int i = 0; i < vec_int_size(&vec); ++i)
        set_int_add(set1, vec_int_get(&vec, i));
    vec_int_free(&vec);
}

VecInt set_int_to_vec(const SetInt* set) {
    VecInt vec = vec_int_alloc_cap(set->_size);
    push_to_vec(&vec, set->_root);
    return vec;
}

static void push_to_vec(VecInt* vec, const SetIntNode* node) {
    if (node) {
        push_to_vec(vec, node->left);
        vec_int_push(vec, node->value);
        push_to_vec(vec, node->right);
    }
}

char* set_int_to_str(const SetInt* set) {
    VecInt vec = set_int_to_vec(set);
    char* s = vec_int_to_str(&vec);
    vec_int_free(&vec);
    return s;
}

inline int set_int_max_depth(const SetInt* set) {
    return node_max_depth(set->_root);
}

static int node_max_depth(const SetIntNode* node) {
    if (!node)
        return 0;
    int left = node_max_depth(node->left);
    int right = node_max_depth(node->right);
    return (left > right) ? (left + 1) : (right + 1);
}
