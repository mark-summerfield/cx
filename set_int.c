// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "set_int.h"
#include <stdlib.h>
#include <string.h>

static set_int_node* add(set_int_node* root, int value, bool* added);
static set_int_node* node_alloc(int value);
static bool is_red(set_int_node* node);
static void color_flip(set_int_node* node);
static set_int_node* add_rotation(set_int_node* node);
static set_int_node* rotate_left(set_int_node* node);
static set_int_node* rotate_right(set_int_node* node);
static void to_vec(set_int_node* node, vec_int* out);
static void delete_node_and_children(set_int_node* node);

set_int set_int_alloc() {
    return (set_int){
        ._size = 0,
        ._root = NULL,
    };
}

void set_int_clear(set_int* me) {
    assert_notnull(me);
    delete_node_and_children(me->_root);
    me->_root = NULL;
    me->_size = 0;
}

static void delete_node_and_children(set_int_node* node) {
    if (node) {
        delete_node_and_children(node->left);
        delete_node_and_children(node->right);
        free(node);
    }
}

bool set_int_add(set_int* me, int value) {
    assert_notnull(me);
    assert_notnull(value);
    bool added = false;
    me->_root = add(me->_root, value, &added);
    me->_root->_red = false;
    if (added)
        me->_size++;
    return added;
}

static set_int_node* add(set_int_node* node, int value, bool* added) {
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

static bool is_red(set_int_node* node) {
    return node != NULL && node->_red;
}

static void color_flip(set_int_node* node) {
    node->_red = !node->_red;
    if (node->left)
        node->left->_red = !node->left->_red;
    if (node->right)
        node->right->_red = !node->right->_red;
}

static set_int_node* add_rotation(set_int_node* node) {
    if (is_red(node->right) && !is_red(node->left))
        node = rotate_left(node);
    if (is_red(node->left) && is_red(node->left->left))
        node = rotate_right(node);
    return node;
}

static set_int_node* rotate_left(set_int_node* node) {
    set_int_node* x = node->right;
    node->right = x->left;
    x->left = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static set_int_node* rotate_right(set_int_node* node) {
    set_int_node* x = node->left;
    node->left = x->right;
    x->right = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static set_int_node* node_alloc(int value) {
    set_int_node* node = malloc(sizeof(set_int_node));
    assert_alloc(node);
    node->left = node->right = NULL;
    node->value = value;
    node->_red = true;
    return node;
}

bool set_int_remove(set_int* me, int value) {
    assert_notnull(me);
    // TODO
    return false;
}

set_int set_int_copy(const set_int* me) {
    assert_notnull(me);
    set_int out = set_int_alloc();
    // TODO
    return out;
}

bool set_int_equal(const set_int* me, const set_int* them) {
    assert_notnull(me);
    assert_notnull(them);
    if (me->_size != them->_size)
        return false;
    // TODO
    return true;
}

bool set_int_contains(set_int* me, int value) {
    // TODO use iteration
    return false;
}

set_int set_int_difference(const set_int* me, const set_int* them) {
    set_int out = set_int_alloc();
    // TODO
    return out;
}

set_int set_int_symmetric_difference(const set_int* me,
                                     const set_int* them) {
    set_int out = set_int_alloc();
    // TODO
    return out;
}

set_int set_int_intersection(const set_int* me, const set_int* them) {
    set_int out = set_int_alloc();
    // TODO
    return out;
}

set_int set_int_union(const set_int* me, const set_int* them) {
    set_int out = set_int_alloc();
    // TODO
    return out;
}

void set_int_unite(set_int* me, const set_int* them) {
    // TODO
}

vec_int set_int_to_vec(const set_int* me) {
    vec_int out = vec_int_alloc_cap(me->_size);
    to_vec(me->_root, &out);
    return out;
}

static void to_vec(set_int_node* node, vec_int* out) {
    if (node) {
        to_vec(node->left, out);
        vec_int_push(out, node->value);
        to_vec(node->right, out);
    }
}
