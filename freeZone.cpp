#pragma once
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <vector>
#include "cell.cpp"
#include "randomCellGeneratorr.cpp" 

namespace free_zone {
    struct Stats {
        double mean = 0.0;
        double median = 0.0;
    };


    inline void freeZoneSize(std::vector<std::vector<bool>>& blocked, const Cell& c) {
        const int n = static_cast<int>(blocked.size());
        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                const int r = c.row + dr;
                const int col = c.col + dc;
                if (r >= 0 && r < n && col >= 0 && col < n) {
                    blocked[r][col] = true;
                }
            }
        }
    }
        inline long long runSingleTrial(RandomCellGenerator & gen, int n, long long m)
        {
            if (n <= 0) throw std::invalid_argument("runSingleTrial: n must be a positive number");
            if (m < 0) throw std::invalid_argument("runSingleTrial: m shouldnt be a negative number");
            std::vector<std::vector<bool>> blocked(n, std::vector<bool>(n, false));
            for (long long i = 0; i < m; ++i) {
                const Cell c = gen();
                freeZoneSize(blocked ,c);
            };
            long long freeCount =  0;
            for(const auto& row: blocked)
            {
                freeCount += std::count(row.begin(), row.end(), false);
            }
            return freeCount;

        }
        inline std::vector<long long> collectfreezonesize(int n, long long m, int trials)
        {
            if(trials <= 0)
            {
                throw std::invalid_argument(" collectFreeZoneSizes: trials must be positive number");
            }
            RandomCellGenerator gen(n);
            std::vector<long long> results;
            results.reserve(trials);

            for (int t =0; t < trials; ++t)
            {
                results.push_back(runSingleTrial(gen, n, m));
            }
            return results;
        }
        inline Stats computeStats(std::vector<long long> values) 
        {
            if(values.empty())
            {
                throw std::invalid_argument("computeStats: empty size of data");
            }
            const double sum = static_cast<double>(std::accumulate(values.begin(), values.end(), 0LL, std::plus<long long>{}));
            Stats s;
            s.mean = sum / static_cast<double>(values.size());

            std::sort(values.begin(), values.end(), [](long long a, long long b)
                {
                    return a < b;
                });
            const std::size_t mid = values.size() / 2;
            if(values.size() % 2 == 0)
            {
                s.median = (values[mid - 1] + values[mid]) / 2.0;
            }
            else 
            {
                s.median = static_cast<double>(values[mid]);
            }
            return s;
        }

}
