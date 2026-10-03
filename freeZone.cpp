#pragma once
#include <algorithm>
#include <vector>
#include "Cell.hpp"

// для подсчёта свободной зоны
inline int freeZoneSize(int n, const std::vector<Cell>& chosen) {
    std::vector<char> blocked(n * n, 0);

    // проверка на то, что в середине
    auto inside = [n](int r, int c) {
        return r >= 0 && r < n && c >= 0 && c < n;
    };

    for (const Cell& cell : chosen) {
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                int r = cell.row + dr;
                int c = cell.col + dc;
                if (inside(r, c)) {
                    blocked[r * n + c] = 1;
                }
            }
        }
    }

    // Для подсчёта не заблок.
    return static_cast<int>(
        std::count_if(blocked.begin(), blocked.end(),
                      [](char b) { return b == 0; }));
}