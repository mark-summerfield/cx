// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "deq_int.h"
#include <assert.h>
#include <stdlib.h>

#define assert_nonempty(d) assert((d->_size) && "unexpectedly empty queue")

static void push_head(DeqInt* deq, DeqIntNode* node);
static DeqIntNode* node_alloc(int value);
static void node_free(DeqIntNode* node);

inline DeqInt deq_int_alloc() { return (DeqInt){NULL, NULL, 0}; }

inline void deq_int_free(DeqInt* deq) { deq_int_clear(deq); }

void deq_int_clear(DeqInt* deq) {
    DeqIntNode* next = NULL;
    for (DeqIntNode* node = deq->head; node; node = next) {
        next = node->next;
        node_free(node);
    }
    deq->head = deq->tail = NULL;
    deq->_size = 0;
}

inline int deq_int_first(DeqInt* deq) {
    assert_nonempty(deq);
    return deq->head->value;
}

inline int deq_int_last(DeqInt* deq) {
    assert_nonempty(deq);
    return deq->tail->value;
}

void deq_int_push(DeqInt* deq, int value) {
    DeqIntNode* node = node_alloc(value);
    if (!deq->head) // empty deque
        push_head(deq, node);
    else {
        assert(!deq->tail->next && "tail must not have a next");
        node->prev = deq->tail;
        deq->tail->next = node;
        deq->tail = node;
    }
    deq->_size++;
}

void deq_int_push_first(DeqInt* deq, int value) {
    DeqIntNode* node = node_alloc(value);
    if (!deq->head) // empty deque
        push_head(deq, node);
    else {
        assert(!deq->head->prev && "head must not have a prev");
        node->next = deq->head;
        deq->head->prev = node;
        deq->head = node;
    }
    deq->_size++;
}

static inline void push_head(DeqInt* deq, DeqIntNode* node) {
    assert(deq->_size == 0 && "headless deque must be empty");
    assert(!deq->tail && "headless deque must not have a tail");
    deq->head = deq->tail = node;
}

int deq_int_pop(DeqInt* deq) {
    assert_nonempty(deq);
    DeqIntNode* node = deq->tail;
    int value = node->value;
    if (deq->_size == 1)
        deq->head = deq->tail = NULL;
    else {
        node->prev->next = NULL;
        deq->tail = node->prev;
    }
    node_free(node);
    deq->_size--;
    return value;
}

int deq_int_pop_first(DeqInt* deq) {
    assert_nonempty(deq);
    DeqIntNode* node = deq->head;
    int value = node->value;
    if (deq->_size == 1)
        deq->head = deq->tail = NULL;
    else {
        node->next->prev = NULL;
        deq->head = node->next;
    }
    node_free(node);
    deq->_size--;
    return value;
}

static DeqIntNode* node_alloc(int value) {
    DeqIntNode* node = malloc(sizeof(DeqIntNode));
    node->prev = node->next = NULL;
    node->value = value;
    return node;
}

static inline void node_free(DeqIntNode* node) { free(node); }
