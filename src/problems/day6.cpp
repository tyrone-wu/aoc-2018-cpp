#include "../utils.h"
#include "day6.h"
#include <algorithm>
#include <charconv>
#include <cstdint>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

namespace adventofcode
{
Day6::Day6() : DaySolver(6) {
    // Data for tests
    test_data = R"(1, 1
1, 6
8, 3
3, 4
5, 5
8, 9)";
    test_results = { 17, 16 };
}

const std::vector<std::tuple<int, int, int>> Day6::parse_input() const {
    auto lines = input_data | std::views::split('\n');
    int i = 0;
    return lines | std::views::transform([&i](const auto&& line) {
               int x;
               int y;
               const auto line_sv = std::string_view(line.begin(), line.end());
               std::from_chars(line_sv.begin(), line_sv.begin() + line_sv.find(','), x);
               std::from_chars(line_sv.begin() + line_sv.find(' ') + 1, line_sv.end(), y);
               return std::tuple(i++, x, y);
           }) |
           std::ranges::to<std::vector>();
}

ResultType Day6::solvePart1() {
    const auto coords = parse_input();
    int max_x = 0;
    int max_y = 0;
    for (const auto& coord : coords) {
        const auto x = get<1>(coord), y = get<2>(coord);
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
    }

    const int pad = 5;
    std::unordered_map<std::pair<int, int>, std::vector<int>, utils::PairHash> grid;
    for (int y = -pad; y <= max_y + pad; y++) {
        for (int x = -pad; x <= max_x + pad; x++) {
            int min_dist = INT32_MAX;
            std::vector<int> ids;
            for (const auto& coord : coords) {
                const int dist = abs(get<1>(coord) - x) + abs(get<2>(coord) - y);
                if (min_dist >= dist) {
                    if (min_dist > dist) {
                        min_dist = dist;
                        ids.clear();
                    }
                    ids.push_back(get<0>(coord));
                }
            }
            grid[std::pair(x, y)] = ids;
        }
    }

    std::unordered_set<int> inf_ids;
    for (int x = -pad; x <= max_x + pad; x++) {
        if (grid[std::pair(x, -pad)].size() == 1)
            inf_ids.insert(grid[std::pair(x, -pad)][0]);
        if (grid[std::pair(x, max_y + pad)].size() == 1)
            inf_ids.insert(grid[std::pair(x, max_y + pad)][0]);
    }
    for (int y = -pad; y <= max_y + pad; y++) {
        if (grid[std::pair(-pad, y)].size() == 1)
            inf_ids.insert(grid[std::pair(-pad, y)][0]);
        if (grid[std::pair(max_x + pad, y)].size() == 1)
            inf_ids.insert(grid[std::pair(max_x + pad, y)][0]);
    }

    int max_area = 0;
    std::unordered_map<int, int> areas;
    for (const auto& [_, ids] : grid) {
        if (ids.size() > 1)
            continue;
        if (inf_ids.find(ids[0]) != inf_ids.end())
            continue;
        if (areas.find(ids[0]) == areas.end())
            areas[ids[0]] = 0;
        max_area = std::max(max_area, ++areas[ids[0]]);
    }
    return max_area;
}

ResultType Day6::solvePart2() {
    const int range = 10000;
    const auto coords = parse_input();
    int max_x = 0;
    int max_y = 0;
    for (const auto& coord : coords) {
        const auto x = get<1>(coord), y = get<2>(coord);
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
    }

    int area = 0;
    for (int y = 0; y <= max_y; y++) {
        for (int x = 0; x <= max_x; x++) {
            int dist = 0;
            for (const auto& coord : coords)
                dist += abs(get<1>(coord) - x) + abs(get<2>(coord) - y);
            if (dist < range)
                area++;
        }
    }
    return area;
}

} // namespace adventofcode
