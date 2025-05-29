// Copyright © 2024-25 Mark Summerfield. All rights reserved.
// License: GPL-3
#pragma once

// Returns true if the set is empty.
#define SET_ISEMPTY(set) ((set)->_size == 0)

// Returns the set's size.
#define SET_SIZE(set) ((set)->_size)
