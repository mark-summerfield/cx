// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "set_str.h"
#include <stdlib.h>

static void set_str_visit_node(const set_str_node* node,
                               void (*visit)(const char*));
void set_str_delete_node(set_str_node* node);

set_str set_str_alloc() {
    return (set_str){
        ._size = 0,
        ._root = NULL,
    };
}

void set_str_clear(set_str* t) {
    assert_notnull(t);
    set_str_delete_node(t->_root);
    t->_size = 0;
}

void set_str_delete_node(set_str_node* node) {
    if (node) {
        set_str_delete_node(node->_left);
        set_str_delete_node(node->_right);
        node->_left = NULL;
        node->_right = NULL;
        free(node->_value);
        node->_value = NULL;
    }
}

bool set_str_add(set_str* t, char* value) {
    assert_notnull(t);
    assert_notnull(value);
    // TODO
    return false;
}

bool set_str_remove(set_str* t, const char* value) {
    assert_notnull(t);
    // TODO
    return false;
}

set_str set_str_copy(const set_str* t) {
    assert_notnull(t);
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

bool set_str_contains(set_str* t, const char* value) {
    // TODO
    return false;
}

void set_str_visitor(const set_str* t, void (*visit)(const char*)) {
    assert_notnull(t);
    if (t->_size)
        set_str_visit_node(t->_root, visit);
}

static void set_str_visit_node(const set_str_node* node,
                               void (*visit)(const char*)) {
    if (!node)
        return;
    set_str_visit_node(node->_left, visit);
    visit(node->_value);
    set_str_visit_node(node->_right, visit);
}

// inline const char** set_str_borrow_root(const set_str* t) { return
// t->_root; }

set_str set_str_difference(const set_str* t1, const set_str* t2) {
    set_str t = set_str_alloc();
    // TODO
    return t;
}

set_str set_str_symmetric_difference(const set_str* t1, const set_str* t2) {
    set_str t = set_str_alloc();
    // TODO
    return t;
}

set_str set_str_intersection(const set_str* t1, const set_str* t2) {
    set_str t = set_str_alloc();
    // TODO
    return t;
}

set_str set_str_union(const set_str* t1, const set_str* t2) {
    set_str t = set_str_alloc();
    // TODO
    return t;
}

void set_str_unite(set_str* t1, const set_str* t2) {
    // TODO
}
