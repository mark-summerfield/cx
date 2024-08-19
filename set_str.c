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
static set_str_node* rotateleft(set_str_node* node);
static set_str_node* rotateright(set_str_node* node);
static void set_str_visit_node(const set_str_node* node,
                               void (*visit)(const char*));
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
    assert_notnull(me);
    assert_notnull(value);
    node_and_flag nodeflag = add(me->_root, value);
    me->_root = nodeflag.node;
    me->_root->_red = false;
    if (nodeflag.flag)
        me->_size++;
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
    if (is_red(root->left) && is_red(root->right))
        color_flip(root);
    int cmp = strcmp(value, root->value);
    if (cmp < 0) {
        nodeflag = add(root->left, value);
        root->left = nodeflag.node;
    } else if (cmp > 0) {
        nodeflag = add(root->right, value);
        root->right = nodeflag.node;
    }
    nodeflag.node = add_rotation(root);
    return nodeflag;
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
        node = rotateleft(node);
    if (is_red(node->left) && is_red(node->left->left))
        node = rotateright(node);
    return node;
}

static set_str_node* rotateleft(set_str_node* node) {
    set_str_node* x = node->right;
    node->right = x->left;
    x->left = node;
    x->_red = node->_red;
    node->_red = true;
    return x;
}

static set_str_node* rotateright(set_str_node* node) {
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

void set_str_visit_all(const set_str* me,
                       void (*visit)(const set_str_visit_data*)) {
    assert_notnull(me);
    if (me->_size) { // TODO iterative algorithm
        set_str_visit_data data = {0, me->_size, NULL};
        // At each iteration: before: data.value = node->value;
        //  -iterate-
        // At each iteration: avter: data.i++;
        //
        // set_str_visit_node(me->_root, visit); // TODO delete
    }
}

static void set_str_visit_node(const set_str_node* node,
                               void (*visit)(const set_str_visit_data*)) {
    if (!node)
        return;
    set_str_visit_node(node->left, visit);
    visit(data); // TODO
    set_str_visit_node(node->right, visit);
}

set_str set_str_difference(const set_str* me, const set_str* them) {
    set_str out = set_str_alloc();
    // TODO
    return out;
}

set_str set_str_symmetric_difference(const set_str* me, const set_str* them) {
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
