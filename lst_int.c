// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "lst_int.h"
#include <assert.h>
#include <stdlib.h>

static void int_push_head(LstInt* lst, LstIntNode* node);
static LstIntNode* int_node_alloc(int value);
static void int_node_free(LstIntNode* node);

inline LstInt lst_int_alloc() { return (LstInt){NULL, NULL, 0}; }

void lst_int_free(LstInt* lst) { lst_int_clear(lst); }

void lst_int_clear(LstInt* lst) {
    for (LstIntNode* node = lst->head; node; node = node->next)
        int_node_free(node);
    lst->head = lst->tail = NULL;
    lst->_size = 0;
}

void lst_int_push(LstInt* lst, int value) {
    LstIntNode* node = int_node_alloc(value);
    if (!lst->head) // empty list
        int_push_head(lst, node);
    else {
        assert(!lst->tail->next && "tail must not have a next");
        node->prev = lst->tail;
        lst->tail = node;
    }
    lst->_size++;
}

void lst_int_push_first(LstInt* lst, int value) {
    LstIntNode* node = int_node_alloc(value);
    if (!lst->head) // empty list
        int_push_head(lst, node);
    else {
        assert(!lst->head->prev && "head must not have a prev");
        node->next = lst->head;
        lst->head = node;
    }
    lst->_size++;
}

static void int_push_head(LstInt* lst, LstIntNode* node) {
    assert(lst->_size == 0 && "headless list must be empty");
    assert(!lst->tail && "headless list must not have a tail");
    lst->head = node;
    lst->tail = node;
}

int lst_int_pop(LstInt* lst) { return 0; }

int lst_int_pop_first(LstInt* lst) { return 0; }

static LstIntNode* int_node_alloc(int value) {
    LstIntNode* node = malloc(sizeof(LstIntNode));
    node->prev = node->next = NULL;
    node->value = value;
    return node;
}

static inline void int_node_free(LstIntNode* node) { free(node); }
