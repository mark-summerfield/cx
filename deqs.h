// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3
#pragma once

// Returns true if the deq is empty.
#define DEQ_ISEMPTY(deq) ((deq)->_size == 0)

// Returns the deq's size.
#define DEQ_SIZE(deq) ((deq)->_size)
