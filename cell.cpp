#pragma once
#include <compare>

// Клетка доски (ряд колона)
struct Cell {
    int row = 0;
    int col = 0;

    // cначала ряд, потом колона
    auto operator<=>(const Cell&) const = default;
};