// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"
#include <stdbool.h>

typedef struct DeqStrNode DeqStrNode;

// A doubly linked list of char* values.
typedef struct DeqStr {
    DeqStrNode* head;
    DeqStrNode* tail;
    int _size;
    bool _owns;
} DeqStr;

typedef struct DeqStrNode {
    DeqStrNode* prev;
    DeqStrNode* next;
    char* value;
} DeqStrNode;

// Returns a new empty DeqStr.
DeqStr deq_str_alloc(bool owns);

// Frees a DeqStr and all its values. It can't be used after this.
void deq_str_free(DeqStr* deq);

// Clears a DeqStr and all its values.
void deq_str_clear(DeqStr* deq);

// Returns true if the DeqStr is owning.
#define deq_str_owns(deq) ((deq)->_owns)

// Returns true if the DeqStr is empty.
#define deq_str_isempty(deq) ((deq)->_size == 0)

// Returns the DeqStr's size.
#define deq_str_size(deq) ((deq)->_size)

// Returns the DeqStr's first value. The DeqStr must not be empty.
const char* deq_str_first(DeqStr* deq);

// Returns the DeqStr's last value. The DeqStr must not be empty.
const char* deq_str_last(DeqStr* deq);

// Adds a new node with the given value at the end of the DeqStr.
// If owns, DeqStr takes ownership of the value (e.g., if char* then use
// strdup()).
void deq_str_push(DeqStr* deq, char* value);

// Adds a new node with the given value at the start of the DeqStr.
// If owns, DeqStr takes ownership of the value (e.g., if char* then use
// strdup()).
void deq_str_push_first(DeqStr* deq, char* value);

// Removes and frees the DeqStr's last node, returning its value.
// The caller owns the value if the DeqStr is owning.
// The DeqStr must not be empty.
char* deq_str_pop(DeqStr* deq);

// Removes and frees the DeqStr's first node, returning its value.
// The caller owns the value if the DeqStr is owning.
// The DeqStr must not be empty.
char* deq_str_pop_first(DeqStr* deq);

// To iterate:
//  for (DeqStrNode* node = deq->head; node; node = node->next)
//      char* value = p->value;
