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

// Frees a DeqStr and frees all its values if owned. It can't be used
// after this.
void deq_str_free(DeqStr* deq);

// Clears a DeqStr and frees all its values if owned.
void deq_str_clear(DeqStr* deq);

// Returns true if the DeqStr is owning.
#define deq_str_owns(deq) ((deq)->_owns)

// Returns the DeqStr's first value or NULL if empty.
const char* deq_str_first(DeqStr* deq);

// Returns the DeqStr's last value or NULL if empty.
const char* deq_str_last(DeqStr* deq);

// Adds a new node with the given value at the end of the DeqStr.
// If owns, DeqStr takes ownership of the value (e.g., if char* then use
// strdup()).
void deq_str_push(DeqStr* deq, char* value);

// Adds a new node with the given value at the start of the DeqStr.
// If owns, DeqStr takes ownership of the value (e.g., if char* then use
// strdup()).
void deq_str_push_first(DeqStr* deq, char* value);

// Removes and frees the DeqStr's last node, returning its value or NULL
// if empty.
// The caller owns the value if the DeqStr is owning.
char* deq_str_pop(DeqStr* deq);

// Removes and frees the DeqStr's first node, returning its value or
// NULL if empty.
// The caller owns the value if the DeqStr is owning.
char* deq_str_pop_first(DeqStr* deq);

// Returns a char* of all the deq's strings joined together with sep, or
// NULL if deq is empty. If sep is NULL they are joined with no separator.
// The returned char* value is now owned by the caller.
char* deq_str_join(DeqStr* deq, const char* sep);

// To iterate:
//  for (DeqStrNode* node = deq->head; node; node = node->next)
//      const char* value = p->value;
