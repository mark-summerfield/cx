// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include "cx.h"

typedef struct LstIntNode LstIntNode;

// A doubly linked list of int values.
typedef struct {
    LstIntNode* head;
    LstIntNode* tail;
    int _size;
} LstInt;

typedef struct LstIntNode {
    LstIntNode* prev;
    LstIntNode* next;
    int value;
} LstIntNode;

// Returns a new empty LstInt.
LstInt lst_int_alloc();

// Frees a LstInt and all its values. It can't be used after this.
void lst_int_free(LstInt* lst);

// Clears a LstInt and all its values.
void lst_int_clear(LstInt* lst);

// Returns true if the LstInt is empty.
#define lst_isempty(lst) ((lst)->_size == 0)

// Returns the LstInt's size.
#define lst_size(lst) ((lst)->_size)

// Adds a new node with the given value at the end of the LstInt.
void lst_int_push(LstInt* lst, int value);

// Adds a new node with the given value at the start of the LstInt.
void lst_int_push_first(LstInt* lst, int value);

// Removes and frees the LstInt's last node, returning its value.
int lst_int_pop(LstInt* lst);

// Removes and frees the LstInt's first node, returning its value.
int lst_int_pop_first(LstInt* lst);

// To iterate:
//  for (LstIntNode* node = lst->head; node; node = node->next)
//      int value = p->value;
