// Copyright © 2024 Mark Summerfield. All rights reserved.

#include "deq_str.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void push_head(DeqStr* deq, DeqStrNode* node);
static DeqStrNode* node_alloc(char* value);
static void node_free(DeqStrNode* node, bool owns);

inline DeqStr deq_str_alloc(bool owns) {
    return (DeqStr){NULL, NULL, 0, owns};
}

inline void deq_str_free(DeqStr* deq) { deq_str_clear(deq); }

void deq_str_clear(DeqStr* deq) {
    DeqStrNode* next = NULL;
    for (DeqStrNode* node = deq->head; node; node = next) {
        next = node->next;
        node_free(node, deq->_owns);
    }
    deq->head = deq->tail = NULL;
    deq->_size = 0;
}

inline const char* deq_str_first(DeqStr* deq) {
    return deq->_size ? deq->head->value : NULL;
}

inline const char* deq_str_last(DeqStr* deq) {
    return deq->_size ? deq->tail->value : NULL;
}

void deq_str_push(DeqStr* deq, char* value) {
    DeqStrNode* node = node_alloc(value);
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

void deq_str_push_first(DeqStr* deq, char* value) {
    DeqStrNode* node = node_alloc(value);
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

static inline void push_head(DeqStr* deq, DeqStrNode* node) {
    assert(deq->_size == 0 && "headless deque must be empty");
    assert(!deq->tail && "headless deque must not have a tail");
    deq->head = deq->tail = node;
}

char* deq_str_pop(DeqStr* deq) {
    if (!deq->_size)
        return NULL;
    DeqStrNode* node = deq->tail;
    char* value = node->value;
    if (deq->_size == 1)
        deq->head = deq->tail = NULL;
    else {
        node->prev->next = NULL;
        deq->tail = node->prev;
    }
    node_free(node, false); // if DeqStr owned, caller now owns
    deq->_size--;
    return value;
}

char* deq_str_pop_first(DeqStr* deq) {
    if (!deq->_size)
        return NULL;
    DeqStrNode* node = deq->head;
    char* value = node->value;
    if (deq->_size == 1)
        deq->head = deq->tail = NULL;
    else {
        node->next->prev = NULL;
        deq->head = node->next;
    }
    node_free(node, false); // if DeqStr owned, caller now owns
    deq->_size--;
    return value;
}

char* deq_str_join(DeqStr* deq, const char* sep) {
    if (!deq->_size)
        return NULL;
    const int SEP_SIZE = sep ? strlen(sep) : 0;
    int size = 0;
    for (DeqStrNode* node = deq->head; node; node = node->next)
        size += strlen(node->value);
    size += ((deq->_size - 1) * SEP_SIZE) + 1; // +1 for 0-terminator
    char* s = malloc(size);
    assert_alloc(s);
    char* p = s;
    for (DeqStrNode* node = deq->head; node; node = node->next) {
        p = stpcpy(p, node->value);
        if (node != deq->tail)
            p = stpcpy(p, sep);
    }
    return s;
}

static DeqStrNode* node_alloc(char* value) {
    DeqStrNode* node = malloc(sizeof(DeqStrNode));
    node->prev = node->next = NULL;
    node->value = value;
    return node;
}

static inline void node_free(DeqStrNode* node, bool owns) {
    if (owns)
        free(node->value);
    free(node);
}
