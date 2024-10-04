// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"

typedef struct DeqIntNode DeqIntNode;

// A doubly linked list of int values.
//
// To iterate:
// ```
// for (DeqIntNode* node = deq->head; node; node = node->next)
//     int value = p->value;
// ```
typedef struct DeqInt {
    DeqIntNode* head;
    DeqIntNode* tail;
    int _size;
} DeqInt;

typedef struct DeqIntNode {
    DeqIntNode* prev;
    DeqIntNode* next;
    int value;
} DeqIntNode;

// Returns a new empty DeqInt.
DeqInt deq_int_alloc();

// Frees a DeqInt and all its values. It can't be used after this.
void deq_int_free(DeqInt* deq);

// Clears a DeqInt and all its values.
void deq_int_clear(DeqInt* deq);

// Returns the DeqInt's first value. The DeqInt must not be empty.
int deq_int_first(DeqInt* deq);

// Returns the DeqInt's last value. The DeqInt must not be empty.
int deq_int_last(DeqInt* deq);

// Adds a new node with the given value at the end of the DeqInt.
void deq_int_push(DeqInt* deq, int value);

// Adds a new node with the given value at the start of the DeqInt.
void deq_int_push_first(DeqInt* deq, int value);

// Removes and frees the DeqInt's last node, returning its value.
// The DeqInt must not be empty.
int deq_int_pop(DeqInt* deq);

// Removes and frees the DeqInt's first node, returning its value.
// The DeqInt must not be empty.
int deq_int_pop_first(DeqInt* deq);
