// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "set_int.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    set_int_node* node;
    bool flag;
} node_and_flag;

static node_and_flag add(set_int_node* root, int value);
static set_int_node* node_alloc(int value);
static bool is_red(set_int_node* node);
static void color_flip(set_int_node* node);
static set_int_node* add_rotation(set_int_node* node);
static set_int_node* rotate_left(set_int_node* node);
static set_int_node* rotate_right(set_int_node* node);
static void to_vec(set_int_node* node, vec_int* v);
static void delete_node_and_children(set_int_node* node);

set_int set_int_alloc() {
    return (set_int){
        ._size = 0,
        ._root = NULL,
    };
}

void set_int_clear(set_int* e) {
    assert_notnull(e);
    delete_node_and_children(e->_root);
    e->_root = NULL;
    e->_size = 0;
}

static void delete_node_and_children(set_int_node* node) {
    if (node) {
        delete_node_and_children(node->_left);
        delete_node_and_children(node->_right);
        free(node);
    }
}

bool set_int_add(set_int* e, int value) {
    assert_notnull(e);
    assert_notnull(value);
    node_and_flag nodeflag = add(e->_root, value);
    nodeflag.node->_red = false;
    if (nodeflag.flag)
        e->_size++;
    return nodeflag.flag;
}

static node_and_flag add(set_int_node* root, int value) {
    node_and_flag nodeflag = {root, false};
    nodeflag.flag = false;
    if (!root) { // If element was in the tree it would go here
        nodeflag.node = node_alloc(value);
        nodeflag.flag = true;
        return nodeflag;
    }
    if (is_red(root->_left) && is_red(root->_right))
        color_flip(root);
    if (value < root->value) {
        nodeflag = add(root->_left, value);
        root->_left = nodeflag.node;
    } else if (value > root->value) {
        nodeflag = add(root->_right, value);
        root->_right = nodeflag.node;
    }
    nodeflag.node = add_rotation(root);
    return nodeflag;
}

static bool is_red(set_int_node* node) {
    return node != NULL && node->_red;
}

static void color_flip(set_int_node* node) {
    node->_red = !node->_red;
    if (node->_left)
        node->_left->_red = !node->_left->_red;
    if (node->_right)
        node->_right->_red = !node->_right->_red;
}

static set_int_node* add_rotation(set_int_node* node) {
    if (is_red(node->_right) && !is_red(node->_left))
        node = rotate_left(node);
    if (is_red(node->_left) && is_red(node->_left->_left))
        node = rotate_right(node);
    return node;
}

static set_int_node* rotate_left(set_int_node* node) {
    set_int_node* x = node->_right;
    node->_right = x->_left;
    x->_left = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static set_int_node* rotate_right(set_int_node* node) {
    set_int_node* x = node->_left;
    node->_left = x->_right;
    x->_right = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static set_int_node* node_alloc(int value) {
    set_int_node* node = malloc(sizeof(set_int_node));
    assert_alloc(node);
    node->_left = node->_right = NULL;
    node->value = value;
    node->_red = true;
    return node;
}

bool set_int_remove(set_int* e, int value) {
    assert_notnull(e);
    // TODO
    return false;
}

set_int set_int_copy(const set_int* e) {
    assert_notnull(e);
    set_int tc = set_int_alloc();
    // TODO
    return tc;
}

bool set_int_equal(const set_int* e1, const set_int* e2) {
    assert_notnull(e1);
    assert_notnull(e2);
    if (e1->_size != e2->_size)
        return false;
    // TODO
    return true;
}

bool set_int_contains(set_int* e, int value) {
    // TODO use iteration
    return false;
}

set_int set_int_difference(const set_int* e1, const set_int* e2) {
    set_int e = set_int_alloc();
    // TODO
    return e;
}

set_int set_int_symmetric_difference(const set_int* e1, const set_int* e2) {
    set_int e = set_int_alloc();
    // TODO
    return e;
}

set_int set_int_intersection(const set_int* e1, const set_int* e2) {
    set_int e = set_int_alloc();
    // TODO
    return e;
}

set_int set_int_union(const set_int* e1, const set_int* e2) {
    set_int e = set_int_alloc();
    // TODO
    return e;
}

void set_int_unite(set_int* e1, const set_int* e2) {
    // TODO
}

vec_int set_int_to_vec(const set_int* e) {
    vec_int v = vec_int_alloc_cap(e->_size);
    to_vec(e->_root, &v);
    return v;
}

static void to_vec(set_int_node* node, vec_int* v) {
    if (node) {
        to_vec(node->_left, v);
        vec_int_push(v, node->value);
        to_vec(node->_right, v);
    }
}
