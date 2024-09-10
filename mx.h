// Copyright © 2024 Mark Summerfield. All rights reserved.
#pragma once

#include <stdbool.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

bool is_zeroish(double a);

bool is_equalish(double a, double b);
