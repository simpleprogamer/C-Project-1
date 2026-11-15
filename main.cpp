#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <stdexcept>
#include <fstream>
#include "cell.cpp"
#include "freeZone.cpp"
#include "randomCellGeneratorr.cpp"
#include "UtilsInput.cpp"

namespace
{
    void printStatsBlock(std::ostream& out, int n, long long m, int trials, const free_zone::Stats& stats)
    {
        out << "\n Part 1: medium and median value of the free zone \n";
        out << "n = " << n << ", m =" << m << ", amount of the experiments T = " << trials << '\n';
        out << std::fixed << std::setprecision(3);
        out << "Medium size of the free zone: " << stats.mean << '\n';
        out << "Median size of the free zone: " << stats.median << '\n';
        out << "Board size n*n = " << (static_cast<long long>(n) * n) << '\n';
    }
    struct DependantPoint
    {
        long long m;
        double ratio;
        double meanFreezone;
        double meanfreezoneratio;
    };
    std::vector<DependantPoint> studyDependencyonRatio(int n, double maxRatio, int pointsCount, int trialsperpoint)
    {
        if (maxRatio <= 0.0)
        {
            throw std::invalid_argument("studyDependencyOnRatio: Maximal Ratio must be a positive number");
        }
        if (pointsCount <= 0.0)
        {
            throw std::invalid_argument("studyDependencyOnRatio: Count of points must be a positive number");
        }
        const long long nsquared = static_cast<long long>(n) * n;
        auto averageFreezone = [n, trialsperpoint](long long m) -> double
            {
                const std::vector<long long> sizes = free_zone::collectfreezonesize(n,m,trialsperpoint);
                const double sum = static_cast<double>(std::accumulate(sizes.begin(), sizes.end(), 0LL, std::plus<long long>{}));
                return sum / static_cast<double>(sizes.size());
            };


        std::vector<DependantPoint> points;
        points.reserve(pointsCount);

        for (int i = 0; i < pointsCount; ++i)
        {
            const double ratio = maxRatio * static_cast<double>(i) / static_cast<double>(pointsCount - 1);
            const long long m = static_cast<long long>(std::llround(ratio * static_cast<double>(nsquared)));
            const double meanFree = averageFreezone(m);
            points.push_back(DependantPoint
                {
                    m, static_cast<double>(m) / static_cast<double>(nsquared), meanFree, meanFree / static_cast<double>(nsquared) });
        }


    
    const auto best = std::max_element(points.begin(), points.end(), [](const DependantPoint& a, const DependantPoint& b) {
        return std::greater<double>{}(b.meanfreezoneratio, a.meanfreezoneratio);
        });
    (void)best;

    return points;
    
    }

void printDependencyTable(std::ostream& out, int n, const std::vector<DependantPoint>& points)
{
    out << "\n Part 2: dependency of the size of the free zone from m/n^2 (n = " << n << ") \n";
    out << std::left << std::setw(10) << "m" << std::setw(14) << "m/n^2" << std::setw(20) << "medium freezone" << "medium freezone/n^2" << '\n';
    out << std::fixed << std::setprecision(4);
    for (const auto&p : points)
    {
        out << std::left << std::setw(18) << p.m << std::setw(14) << p.ratio << std::setw(20) << p.meanFreezone << p.meanfreezoneratio << '\n';
    }
    out << "\n Results: With growing m/n^2 the part of the free cells decreases monotonously"
        "(every chosen cell blocks itself and up to its 8 neighbours), and in greater m/n^2 freezone approaches to zero.\n";
}

}


int main()
{
    try
    {
        std::cout << "Task 10: random cell of the board n*n, free zone =\n";
        std::cout << "Neighbours cells are cells that can be neighbours by the side or by the diagonal.)\n\n";
        
        const int n = input_utils::readPositiveInt(std::cin, std::cout, "Write the size of the board n:");
        const long long m = input_utils::readNonNegativeLongLong(std::cin, std::cout, "Write the amount of chosen cells m:");
        const int trials = input_utils::readPositiveInt(std::cin, std::cout, "Write the amount of experiments T:");
        const std::vector<long long> sizes = free_zone::collectfreezonesize(n,m,trials);  
        const free_zone::Stats stats = free_zone::computeStats(sizes);
        printStatsBlock(std::cout, n, m, trials, stats);

        constexpr double kmaxratio = 4.0;
        constexpr int kpointscount = 21;
        constexpr int ktrialsperpoint = 30;
        const std::vector<DependantPoint> points = studyDependencyonRatio(n, kmaxratio, kpointscount, ktrialsperpoint);
        printDependencyTable(std::cout, n, points);

        const std::string outputPath = "output/results.txt";
        std::ofstream fout(outputPath);
        if(!fout)
        {
            throw std::runtime_error("Couldn't open the file to save results: " + outputPath);
        }
        printStatsBlock(fout, n, m, trials, stats);
        printDependencyTable(fout, n, points);
        fout.close();
        std::cout << "\nExperiment saved into the file:" << outputPath << '\n';
    }
    catch(const std::exception& ex)
    {
    std::cerr << "Error:" << ex.what() << "\n";
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error.\n";
        return 1;
    }
    return 0;
}

