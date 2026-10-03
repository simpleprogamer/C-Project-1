#pragma once
#include <random>
#include <stdexcept>
#include "Cell.hpp"

//возвр n * n (opperator)
class RandomCellGenerator {
public:
    RandomCellGenerator(int n, unsigned seed)
        : engine_(seed), dist_(0, n - 1) {
        if (n <= 0) {
            throw std::invalid_argument("Розмір дошки n має бути додатним");
        }
    }

    Cell operator()() {
        int r = dist_(engine_);
        int c = dist_(engine_);
        return Cell{r, c};
    }

private:
    std::mt19937 engine_;
    std::uniform_int_distribution<int> dist_;
};