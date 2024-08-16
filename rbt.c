// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "rbt.h"
#include <stdlib.h>

static void rbt_apply(const _rbt_node* node, void (*fn)(const void*));

rbt rbt_alloc(rbt_alloc_args args) {
    assert(args.cmp && "must provide a cmp function");
    assert(args.cpy && "must provide a cpy function");
    assert(args.destroy && "must provide a destroy function");
    return (rbt){._size = 0,
                 ._root = NULL,
                 ._cmp = args.cmp,
                 ._cpy = args.cpy,
                 ._destroy = args.destroy};
}

void rbt_free(rbt* t) {
    assert_notnull(t);
    rbt_clear(t);
    // TODO
}

void rbt_clear(rbt* t) {
    assert_notnull(t);
    // TODO
    t->_size = 0;
}

void rbt_insert(rbt* t, void* value) {
    assert_notnull(t);
    assert_notnull(value);
    // TODO
}

inline bool rbt_remove(rbt* t, void* value) {
    assert_notnull(t);
    // TODO
    return false;
}

rbt rbt_copy(const rbt* t) {
    assert_notnull(t);
#pragma GCC diagnostic ignored "-Woverride-init"
#pragma GCC diagnostic push
    rbt vc = rbt_alloc((rbt_alloc_args){
        .cmp = t->_cmp, .cpy = t->_cpy, .destroy = t->_destroy});
#pragma GCC diagnostic pop
    // TODO
    return vc;
}

bool rbt_equal(const rbt* v1, const rbt* v2) {
    assert_notnull(v1);
    assert_notnull(v2);
    if (v1->_size != v2->_size || v1->_cmp != v2->_cmp ||
        v1->_cpy != v2->_cpy || v1->_destroy != v2->_destroy)
        return false;
    // TODO
    return true;
}

bool rbt_find(const rbt* t, const void* value, void* tree_value) {
    assert_notnull(t);
    assert_notnull(value);
    // TODO tree_value may be NULL in which case not assigned to
    return false;
}

inline bool rbt_contains(rbt* t, const void* value) {
    return rbt_find(t, value, NULL);
}

inline void rbt_iterate(const rbt* t, void (*fn)(const void*)) {
    assert_notnull(t);
    if (t->_size)
        rbt_apply(t->_root, fn);
}

static void rbt_apply(const _rbt_node* node, void (*fn)(const void*)) {
    if (!node)
        return;
    rbt_apply(node->_left, fn);
    fn(node->_value);
    rbt_apply(node->_right, fn);
}
