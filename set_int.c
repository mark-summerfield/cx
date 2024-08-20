// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "set_int.h"
#include <stdlib.h>
#include <string.h>

static SetIntNode* add(SetIntNode* node, int value, bool* added);
static SetIntNode* node_alloc(int value);
static bool is_red(SetIntNode* node);
static void color_flip(SetIntNode* node);
static SetIntNode* add_rotation(SetIntNode* node);
static SetIntNode* rotate_left(SetIntNode* node);
static SetIntNode* rotate_right(SetIntNode* node);
static void to_vec(SetIntNode* node, VecInt* vec);
static void delete_node_and_children(SetIntNode* node);
static int max_depth(SetIntNode* node);

SetInt set_int_alloc() {
    return (SetInt){
        ._size = 0,
        ._root = NULL,
    };
}

inline void set_int_free(SetInt* set) { set_int_clear(set); }

void set_int_clear(SetInt* set) {
    assert_notnull(set);
    delete_node_and_children(set->_root);
    set->_root = NULL;
    set->_size = 0;
}

static void delete_node_and_children(SetIntNode* node) {
    if (node) {
        delete_node_and_children(node->left);
        delete_node_and_children(node->right);
        free(node);
    }
}

bool set_int_add(SetInt* set, int value) {
    assert_notnull(set);
    bool added = false;
    set->_root = add(set->_root, value, &added);
    set->_root->_red = false;
    if (added)
        set->_size++;
    return added;
}

static SetIntNode* add(SetIntNode* node, int value, bool* added) {
    if (!node) { // If value was in the tree it would go here
        *added = true;
        return node_alloc(value);
    }
    if (is_red(node->left) && is_red(node->right))
        color_flip(node);
    if (value < node->value)
        node->left = add(node->left, value, added);
    else if (value > node->value)
        node->right = add(node->right, value, added);
    return add_rotation(node);
}

static bool is_red(SetIntNode* node) { return node != NULL && node->_red; }

static void color_flip(SetIntNode* node) {
    node->_red = !node->_red;
    if (node->left)
        node->left->_red = !node->left->_red;
    if (node->right)
        node->right->_red = !node->right->_red;
}

static SetIntNode* add_rotation(SetIntNode* node) {
    if (is_red(node->right) && !is_red(node->left))
        node = rotate_left(node);
    if (is_red(node->left) && is_red(node->left->left))
        node = rotate_right(node);
    return node;
}

static SetIntNode* rotate_left(SetIntNode* node) {
    SetIntNode* x = node->right;
    node->right = x->left;
    x->left = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static SetIntNode* rotate_right(SetIntNode* node) {
    SetIntNode* x = node->left;
    node->left = x->right;
    x->right = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static SetIntNode* node_alloc(int value) {
    SetIntNode* node = malloc(sizeof(SetIntNode));
    assert_alloc(node);
    node->left = node->right = NULL;
    node->value = value;
    node->_red = true;
    return node;
}

bool set_int_remove(SetInt* set, int value) {
    assert_notnull(set);
    // TODO
    return false;
}

SetInt set_int_copy(const SetInt* set) {
    assert_notnull(set);
    SetInt out = set_int_alloc();
    // TODO
    return out;
}

bool set_int_equal(const SetInt* set1, const SetInt* set2) {
    assert_notnull(set1);
    assert_notnull(set2);
    if (set1->_size != set2->_size)
        return false;
    // TODO
    return true;
}

bool set_int_contains(SetInt* set, int value) {
    bool found = false;
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
    SetInt set = set_int_alloc();
    // TODO
    return set;
}

SetInt set_int_symmetric_difference(const SetInt* set1,
                                    const SetInt* set2) {
    SetInt set = set_int_alloc();
    // TODO
    return set;
}

SetInt set_int_intersection(const SetInt* set1, const SetInt* set2) {
    SetInt set = set_int_alloc();
    // TODO
    return set;
}

SetInt set_int_union(const SetInt* set1, const SetInt* set2) {
    SetInt set = set_int_alloc();
    // TODO
    return set;
}

void set_int_unite(SetInt* set1, const SetInt* set2) {
    // TODO
}

VecInt set_int_to_vec(const SetInt* set) {
    VecInt vec = vec_int_alloc_cap(set->_size);
    to_vec(set->_root, &vec);
    return vec;
}

static void to_vec(SetIntNode* node, VecInt* vec) {
    if (node) {
        to_vec(node->left, vec);
        vec_int_push(vec, node->value);
        to_vec(node->right, vec);
    }
}

inline int set_int_max_depth(const SetInt* set) {
    return max_depth(set->_root);
}

static int max_depth(SetIntNode* node) {
    if (!node)
        return 0;
    int left = max_depth(node->left);
    int right = max_depth(node->right);
    return (left > right) ? (left + 1) : (right + 1);
}
