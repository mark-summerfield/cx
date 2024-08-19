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
        free(node->value);
        node->value = NULL;
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
    // TODO use iteration
    return false;
}

void set_str_visit_all(const set_str* t,
                       void (*visit)(const set_str_visit_data*)) {
    assert_notnull(t);
    if (t->_size) { // TODO iterative algorithm
        set_str_visit_data data = {0, t->_size, NULL};
        // At each iteration: before: data.value = node->value;
        //  -iterate-
        // At each iteration: avter: data.i++;
        //
        // set_str_visit_node(t->_root, visit); // TODO delete
    }
}

/* TODO delete
static void set_str_visit_node(const set_str_node* node,
                               void (*visit)(const char*)) {
    if (!node)
        return;
    set_str_visit_node(node->_left, visit);
    visit(node->value);
    set_str_visit_node(node->_right, visit);
}
*/

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

const set_str_node* set_str_root(set_str* t) {
    return NULL; // TODO
}

inline const char* set_str_tostring(const set_str* t) {
    return set_str_tostring_range(t, 0, t->_size - 1);
}

const char* set_str_tostring_range(const set_str* t, cx_size begin,
                                   cx_size end) {
    return NULL; // TODO
}
