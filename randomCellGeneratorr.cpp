#pragma once
#include <random>
#include <stdexcept>
#include "cell.cpp"
#include <string>

//возвр n * n (opperator)
class RandomCellGenerator {
public:

    explicit RandomCellGenerator(int n)
        : n_(checkSize(n)),
        engine_(std::random_device{}()),
        dist_(0, n - 1) {
    }


    Cell operator()() {
        return Cell{ dist_(engine_), dist_(engine_) };
    }

    int boardSize() const noexcept { return n_; }

private:
    static int checkSize(int n) {
        if (n <= 0) {
            throw std::invalid_argument(
                "RandomCellGenerator: n size of the board must be a positive number" +
                std::to_string(n));
        }
        return n;
    }

    int n_;
    std::mt19937 engine_;
    std::uniform_int_distribution<int> dist_;
};
