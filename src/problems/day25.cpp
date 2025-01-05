#include "day25.h"
#include <algorithm>
#include <array>
#include <charconv>
#include <cstdlib>
#include <queue>
#include <ranges>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace adventofcode
{
Day25::Day25() : DaySolver(25) {
    // Data for tests
    test_data = R"(0,0,0,0
3,0,0,0
0,3,0,0
0,0,3,0
0,0,0,3
0,0,0,6
9,0,0,0
12,0,0,0)";
    test_results = { 2, 0 };
}

const std::vector<std::array<short, 4>> Day25::parse_input() const {
    return input_data | std::views::split('\n') | std::views::transform([](const auto&& line) {
               const std::string_view point_sv(line);
               std::array<short, 4> point = {};
               auto nums = point_sv | std::views::split(',');
               unsigned long i = 0;
               std::ranges::for_each(nums, [&point, &i](const auto& n) {
                   std::from_chars(n.begin(), n.end(), point[i++]);
               });
               return point;
           }) |
           std::ranges::to<std::vector>();
}

ResultType Day25::solvePart1() {
    const auto points = parse_input();

    std::unordered_map<unsigned long, std::unordered_set<unsigned long>> connected;
    for (unsigned long i = 0; i < points.size(); ++i) {
        for (unsigned long j = 0; j < points.size(); ++j) {
            if (i == j)
                continue;

            if (!connected.contains(i))
                connected[i] = {};

            short man_dist = 0;
            for (unsigned long k = 0; k < 4; ++k)
                man_dist += std::abs(points[i][k] - points[j][k]);
            if (man_dist <= 3)
                connected[i].insert(j);
        }
    }

    int constellations = 0;
    std::unordered_set<unsigned long> seen;
    std::queue<unsigned long> buffer;
    for (unsigned long i = 0; i < points.size(); ++i) {
        if (seen.contains(i))
            continue;

        ++constellations;
        buffer.push(i);
        while (!buffer.empty()) {
            const auto idx = buffer.front();
            buffer.pop();

            for (const auto& to : connected.at(idx)) {
                if (!seen.contains(to)) {
                    buffer.push(to);
                    seen.insert(to);
                }
            }
        }
    }
    return constellations;
}

ResultType Day25::solvePart2() { return 0; }

} // namespace adventofcode
