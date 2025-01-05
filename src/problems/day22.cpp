#include "day22.h"
#include <charconv>
#include <climits>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

namespace adventofcode
{
Day22::Day22() : DaySolver(22) {
    // Data for tests
    test_data = R"(depth: 510
target: 10,10)";
    test_results = { 114UL, 45UL };
}

const std::tuple<unsigned int, unsigned int, unsigned int> Day22::parse_input() const {
    unsigned int depth, x_target, y_target;
    std::from_chars(input_data.begin() + input_data.find(' ') + 1,
                    input_data.begin() + input_data.find('\n'), depth);
    std::from_chars(input_data.begin() + input_data.find_last_of(' ') + 1,
                    input_data.begin() + input_data.find_last_of(','), x_target);
    std::from_chars(input_data.begin() + input_data.find_last_of(',') + 1, input_data.end(),
                    y_target);

    return std::tuple(depth, x_target, y_target);
}

unsigned long Day22::geologic_index(
    const unsigned int x, const unsigned int y, const unsigned int x_target,
    const unsigned int y_target,
    const std::vector<std::vector<std::pair<unsigned long, ErosionLevel>>>& cave) const {
    if ((x == 0 && y == 0) || (x == x_target && y == y_target)) {
        return 0;
    } else if (y == 0) {
        return static_cast<unsigned long>(x) * 16807UL;
    } else if (x == 0) {
        return static_cast<unsigned long>(y) * 48271UL;
    } else {
        return static_cast<unsigned long>(cave[y][x - 1].first) *
               static_cast<unsigned long>(cave[y - 1][x].first);
    }
}

const std::pair<unsigned long, Day22::ErosionLevel>
Day22::erosion_level(const unsigned long depth, const unsigned long geologic_index) const {
    const auto erosion_level = (geologic_index + depth) % 20183;
    const auto region_type = static_cast<ErosionLevel>(erosion_level % 3);
    return std::pair(erosion_level, region_type);
}

void Day22::debug(
    const std::vector<std::vector<std::pair<unsigned long, ErosionLevel>>>& cave) const {
    auto y = 0;
    for (const auto& row : cave) {
        if (y++ == 16)
            break;

        auto x = 0;
        for (const auto& [el, rt] : row) {
            if (x++ == 16)
                break;

            char rt_char;
            if (rt == ErosionLevel::Rocky)
                rt_char = '.';
            else if (rt == ErosionLevel::Wet)
                rt_char = '=';
            else
                rt_char = '|';
            std::cout << rt_char;
        }
        std::cout << '\n';
    }
}

const std::vector<std::vector<std::pair<unsigned long, Day22::ErosionLevel>>>
Day22::generate_cave(const unsigned int depth, const unsigned int x_target,
                     const unsigned int y_target) const {
    std::vector<std::vector<std::pair<unsigned long, ErosionLevel>>> cave(
        depth, std::vector<std::pair<unsigned long, ErosionLevel>>(depth));
    for (auto y = 0U; y < depth; ++y) {
        for (auto x = 0U; x < depth; ++x) {
            const auto geo_idx = geologic_index(x, y, x_target, y_target, cave);
            cave[y][x] = erosion_level(depth, geo_idx);
        }
    }
    // debug(cave);
    return cave;
}

ResultType Day22::solvePart1() {
    const auto [depth, x_target, y_target] = parse_input();
    const auto cave = generate_cave(depth, x_target, y_target);
    unsigned long risk_level = 0;
    for (auto y = 0U; y <= y_target; ++y) {
        for (auto x = 0U; x <= x_target; ++x)
            risk_level += static_cast<unsigned long>(cave[y][x].second);
    }
    return risk_level;
}

void Day22::dfs(const std::vector<std::vector<std::pair<unsigned long, ErosionLevel>>>& cave, Me me,
                const unsigned long x_target, const unsigned long y_target,
                std::unordered_map<unsigned long, unsigned long>& cache) const {
    if (me.x == x_target && me.y == y_target)
        me.equip_tool(Tool::Torch);

    const auto key = me.key();
    if (!cache.contains(key))
        cache[key] = ULONG_MAX;

    // 1003 was my first submission, which was too high :P
    // general solution would be to use pq to get a (potentially) non-optimal solution first, then
    // use that to trim branching
    if (me.time < cache.at(key) &&
        me.time < 1003) { // me.time < cache.at(Me::key_hash(x_target, y_target, Tool::Torch))) {
        cache[key] = me.time;
        if (me.x == x_target && me.y == y_target)
            return;
    } else {
        return;
    }

    const auto erosion_from = cave[me.y][me.x].second;
    ++me.time;
    if (me.y > 0) {
        Me clone = me;
        --clone.y;
        clone.match_tool(erosion_from, cave[clone.y][clone.x].second);
        dfs(cave, clone, x_target, y_target, cache);
    }
    if (me.x + 1 < x_target + 100) { // cave.size()) {
        Me clone = me;
        ++clone.x;
        clone.match_tool(erosion_from, cave[clone.y][clone.x].second);
        dfs(cave, clone, x_target, y_target, cache);
    }
    if (me.y + 1 < y_target + 100) { // cave.size()) {
        Me clone = me;
        ++clone.y;
        clone.match_tool(erosion_from, cave[clone.y][clone.x].second);
        dfs(cave, clone, x_target, y_target, cache);
    }
    if (me.x > 0) {
        Me clone = me;
        --clone.x;
        clone.match_tool(erosion_from, cave[clone.y][clone.x].second);
        dfs(cave, clone, x_target, y_target, cache);
    }
}

// 41890842 µs
ResultType Day22::solvePart2() {
    const auto [depth, x_target, y_target] = parse_input();
    const auto cave = generate_cave(depth, x_target, y_target);

    std::unordered_map<unsigned long, unsigned long> cache = {
        { Me::key_hash(x_target, y_target, Tool::Torch), ULONG_MAX },
    };
    dfs(cave, Me(), x_target, y_target, cache);
    return cache.at(Me::key_hash(x_target, y_target, Tool::Torch));
}

// 976 low

} // namespace adventofcode
