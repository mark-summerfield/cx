// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "set_str.h"
#include <stdlib.h>
#include <string.h>

typedef struct {
    set_str_node* node;
    bool flag;
} node_and_flag;

static set_str_node* node_alloc(char* value);
static bool is_red(set_str_node* node);
static void color_flip(set_str_node* node);
static set_str_node* add_rotation(set_str_node* node);
static set_str_node* rotate_left(set_str_node* node);
static set_str_node* rotate_right(set_str_node* node);
void set_str_delete_node_and_children(set_str_node* node);

set_str set_str_alloc() {
    return (set_str){
        ._size = 0,
        ._root = NULL,
    };
}

void set_str_clear(set_str* me) {
    assert_notnull(me);
    set_str_delete_node_and_children(me->_root);
    me->_root = NULL;
    me->_size = 0;
}

void set_str_delete_node_and_children(set_str_node* node) {
    if (node) {
        set_str_delete_node_and_children(node->left);
        set_str_delete_node_and_children(node->right);
        free(node->value);
        free(node);
    }
}

bool set_str_add(set_str* me, char* value) {
    return false; // TODO
}

static bool is_red(set_str_node* node) {
    return node != NULL && node->_red;
}

static void color_flip(set_str_node* node) {
    node->_red = !node->_red;
    if (node->left)
        node->left->_red = !node->left->_red;
    if (node->right)
        node->right->_red = !node->right->_red;
}

static set_str_node* add_rotation(set_str_node* node) {
    if (is_red(node->right) && !is_red(node->left))
        node = rotate_left(node);
    if (is_red(node->left) && is_red(node->left->left))
        node = rotate_right(node);
    return node;
}

static set_str_node* rotate_left(set_str_node* node) {
    set_str_node* x = node->right;
    node->right = x->left;
    x->left = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static set_str_node* rotate_right(set_str_node* node) {
    set_str_node* x = node->left;
    node->left = x->right;
    x->right = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static set_str_node* node_alloc(char* value) {
    set_str_node* node = malloc(sizeof(set_str_node));
    assert_alloc(node);
    node->left = node->right = NULL;
    node->value = value;
    node->_red = true;
    return node;
}

bool set_str_remove(set_str* me, const char* value) {
    assert_notnull(me);
    // TODO
    return false;
}

set_str set_str_copy(const set_str* me) {
    assert_notnull(me);
    set_str out = set_str_alloc();
    // TODO
    return out;
}

bool set_str_equal(const set_str* me, const set_str* them) {
    assert_notnull(me);
    assert_notnull(them);
    if (me->_size != them->_size)
        return false;
    // TODO
    return true;
}

bool set_str_contains(set_str* me, const char* value) {
    // TODO use iteration
    return false;
}

set_str set_str_difference(const set_str* me, const set_str* them) {
    set_str out = set_str_alloc();
    // TODO
    return out;
}

set_str set_str_symmetric_difference(const set_str* me,
                                     const set_str* them) {
    set_str out = set_str_alloc();
    // TODO
    return out;
}

set_str set_str_intersection(const set_str* me, const set_str* them) {
    set_str out = set_str_alloc();
    // TODO
    return out;
}

set_str set_str_union(const set_str* me, const set_str* them) {
    set_str me = set_str_alloc();
    // TODO
    return out;
}

void set_str_unite(set_str* me, const set_str* them) {
    // TODO
}

inline const set_str_node* set_str_get_root(const set_str* me) {
    return me->_root;
}
