// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "deq_int.h"
#include <assert.h>
#include <stdlib.h>

static void int_push_head(DeqInt* deq, DeqIntNode* node);
static DeqIntNode* int_node_alloc(int value);

inline DeqInt deq_int_alloc() { return (DeqInt){NULL, NULL, 0}; }

void deq_int_free(DeqInt* deq) { deq_int_clear(deq); }

void deq_int_clear(DeqInt* deq) {
    DeqIntNode* next = NULL;
    for (DeqIntNode* node = deq->head; node; node = next) {
        next = node->next;
        free(node);
    }
    deq->head = deq->tail = NULL;
    deq->_size = 0;
}

void deq_int_push(DeqInt* deq, int value) {
    DeqIntNode* node = int_node_alloc(value);
    if (!deq->head) // empty list
        int_push_head(deq, node);
    else {
        assert(!deq->tail->next && "tail must not have a next");
        node->prev = deq->tail;
        deq->tail = node;
    }
    deq->_size++;
}

void deq_int_push_first(DeqInt* deq, int value) {
    DeqIntNode* node = int_node_alloc(value);
    if (!deq->head) // empty list
        int_push_head(deq, node);
    else {
        assert(!deq->head->prev && "head must not have a prev");
        node->next = deq->head;
        deq->head = node;
    }
    deq->_size++;
}

static void int_push_head(DeqInt* deq, DeqIntNode* node) {
    assert(deq->_size == 0 && "headless list must be empty");
    assert(!deq->tail && "headless list must not have a tail");
    deq->head = node;
    deq->tail = node;
}

int deq_int_pop(DeqInt* deq) {
    assert(deq->_size && "can't pop empty list");
    DeqIntNode* node = deq->tail;
    deq->tail = deq->tail->prev;
    int value = node->value;
    free(node);
    deq->_size--;
    return value;
}

int deq_int_pop_first(DeqInt* deq) {
    assert(deq->_size && "can't pop empty list");
    DeqIntNode* node = deq->head;
    deq->head = deq->head->next;
    int value = node->value;
    free(node);
    deq->_size--;
    return value;
}

static DeqIntNode* int_node_alloc(int value) {
    DeqIntNode* node = malloc(sizeof(DeqIntNode));
    node->prev = node->next = NULL;
    node->value = value;
    return node;
}
