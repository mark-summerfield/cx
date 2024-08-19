// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "set_str.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    set_str_node* node;
    bool flag;
} node_and_flag;

static node_and_flag add(set_str_node* root, char* value);
static set_str_node* node_alloc(char* value);
static bool is_red(set_str_node* node);
static void color_flip(set_str_node* node);
static set_str_node* add_rotation(set_str_node* node);
static set_str_node* rotate_left(set_str_node* node);
static set_str_node* rotate_right(set_str_node* node);
static void set_str_visit_node(const set_str_node* node,
                               void (*visit)(const char*));
void set_str_delete_node_and_children(set_str_node* node);

set_str set_str_alloc() {
    return (set_str){
        ._size = 0,
        ._root = NULL,
    };
}

void set_str_clear(set_str* v) {
    assert_notnull(v);
    set_str_delete_node_and_children(v->_root);
    v->_root = NULL;
    v->_size = 0;
}

void set_str_delete_node_and_children(set_str_node* node) {
    if (node) {
        set_str_delete_node_and_children(node->_left);
        set_str_delete_node_and_children(node->_right);
        free(node->value);
        free(node);
    }
}

bool set_str_add(set_str* v, char* value) {
    assert_notnull(v);
    assert_notnull(value);
    node_and_flag nodeflag = add(v->_root, value);
    nodeflag.node->_red = false;
    if (nodeflag.flag)
        v->_size++;
    return nodeflag.flag;
}

static node_and_flag add(set_str_node* root, char* value) {
    node_and_flag nodeflag = {root, false};
    nodeflag.flag = false;
    if (!root) { // If element was in the tree it would go here
        nodeflag.node = node_alloc(value);
        nodeflag.flag = true;
        return nodeflag;
    }
    if (is_red(root->_left) && is_red(root->_right))
        color_flip(root);
    int cmp = strcmp(value, root->value);
    if (cmp < 0) {
        nodeflag = add(root->_left, value);
        root->_left = nodeflag.node;
    } else if (cmp > 0) {
        nodeflag = add(root->_right, value);
        root->_right = nodeflag.node;
    }
    nodeflag.node = add_rotation(root);
    return nodeflag;
}

static bool is_red(set_str_node* node) {
    return node != NULL && node->_red;
}

static void color_flip(set_str_node* node) {
    node->_red = !node->_red;
    if (node->_left)
        node->_left->_red = !node->_left->_red;
    if (node->_right)
        node->_right->_red = !node->_right->_red;
}

static set_str_node* add_rotation(set_str_node* node) {
    if (is_red(node->_right) && !is_red(node->_left))
        node = rotate_left(node);
    if (is_red(node->_left) && is_red(node->_left->_left))
        node = rotate_right(node);
    return node;
}

static set_str_node* rotate_left(set_str_node* node) {
    set_str_node* x = node->_right;
    node->_right = x->_left;
    x->_left = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static set_str_node* rotate_right(set_str_node* node) {
    set_str_node* x = node->_left;
    node->_left = x->_right;
    x->_right = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static set_str_node* node_alloc(char* value) {
    set_str_node* node = malloc(sizeof(set_str_node));
    assert_alloc(node);
    node->_left = node->_right = NULL;
    node->value = value;
    node->_red = true;
    return node;
}

bool set_str_remove(set_str* v, const char* value) {
    assert_notnull(v);
    // TODO
    return false;
}

set_str set_str_copy(const set_str* v) {
    assert_notnull(v);
    set_str tc = set_str_alloc();
    // TODO
    return tc;
}

bool set_str_equal(const set_str* v1, const set_str* v2) {
    assert_notnull(v1);
    assert_notnull(v2);
    if (v1->_size != v2->_size)
        return false;
    // TODO
    return true;
}

bool set_str_contains(set_str* v, const char* value) {
    // TODO use iteration
    return false;
}

void set_str_visit_all(const set_str* v,
                       void (*visit)(const set_str_visit_data*)) {
    assert_notnull(v);
    if (v->_size) { // TODO iterative algorithm
        set_str_visit_data data = {0, v->_size, NULL};
        // At each iteration: before: data.value = node->value;
        //  -iterate-
        // At each iteration: avter: data.i++;
        //
        // set_str_visit_node(v->_root, visit); // TODO delete
    }
}

static void set_str_visit_node(const set_str_node* node,
                               void (*visit)(const set_str_visit_data*)) {
    if (!node)
        return;
    set_str_visit_node(node->_left, visit);
    visit(data); // TODO
    set_str_visit_node(node->_right, visit);
}

set_str set_str_difference(const set_str* v1, const set_str* v2) {
    set_str v = set_str_alloc();
    // TODO
    return v;
}

set_str set_str_symmetric_difference(const set_str* v1, const set_str* v2) {
    set_str v = set_str_alloc();
    // TODO
    return v;
}

set_str set_str_intersection(const set_str* v1, const set_str* v2) {
    set_str v = set_str_alloc();
    // TODO
    return v;
}

set_str set_str_union(const set_str* v1, const set_str* v2) {
    set_str v = set_str_alloc();
    // TODO
    return v;
}

void set_str_unite(set_str* v1, const set_str* v2) {
    // TODO
}

const set_str_node* set_str_root(set_str* v) {
    return NULL; // TODO
}

inline const char* set_str_tostring(const set_str* v) {
    return set_str_tostring_range(v, 0, v->_size - 1);
}

const char* set_str_tostring_range(const set_str* v, int begin, int end) {
    return NULL; // TODO
}
