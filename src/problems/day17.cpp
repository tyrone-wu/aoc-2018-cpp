#include "../utils.h"
#include "day17.h"
#include <algorithm>
#include <charconv>
#include <climits>
#include <iostream>
#include <ranges>
#include <tuple>

namespace adventofcode
{
Day17::Day17() : DaySolver(17) {
    // Data for tests
    test_data = R"(x=495, y=2..7
y=7, x=495..501
x=501, y=3..7
x=498, y=2..4
x=506, y=1..2
x=498, y=10..13
x=504, y=10..13
y=13, x=498..504)";
    test_results = { 57UL, 29UL };
}

const std::tuple<std::unordered_set<std::pair<unsigned int, unsigned int>, utils::UIntPairHash>,
                 unsigned int, unsigned int>
Day17::parse_input() const {
    std::unordered_set<std::pair<unsigned int, unsigned int>, utils::UIntPairHash> clay;
    unsigned int min_y = UINT_MAX, max_y = 0;

    auto lines = input_data | std::views::split('\n');
    std::ranges::for_each(lines, [&clay, &min_y, &max_y](const auto& line) {
        const std::string_view line_sv(line);

        unsigned int point, range_lo, range_hi;
        std::from_chars(line_sv.begin() + line_sv.find('=') + 1,
                        line_sv.begin() + line_sv.find(','), point);
        std::from_chars(line_sv.begin() + line_sv.find_last_of('=') + 1,
                        line_sv.begin() + line_sv.find('.'), range_lo);
        std::from_chars(line_sv.begin() + line_sv.find_last_of('.') + 1, line_sv.end(), range_hi);

        unsigned int x_min, x_max, y_min, y_max;
        if (line_sv.front() == 'x') {
            x_min = point, x_max = point;
            y_min = range_lo, y_max = range_hi;
        } else {
            x_min = range_lo, x_max = range_hi;
            y_min = point, y_max = point;
        }

        for (auto x = x_min; x <= x_max; ++x) {
            for (auto y = y_min; y <= y_max; ++y) {
                clay.insert(std::pair(x, y));
                min_y = std::min(min_y, y);
                max_y = std::max(max_y, y);
            }
        }
    });
    return std::tuple(clay, min_y, max_y);
}

void Day17::display(
    const std::unordered_set<std::pair<unsigned int, unsigned int>, utils::UIntPairHash>& clay,
    const std::unordered_set<std::pair<unsigned int, unsigned int>, utils::UIntPairHash>& water,
    const unsigned int depth_from_bottom) const {
    unsigned int x_min = UINT_MAX, x_max = 0, y_min = UINT_MAX, y_max = 0;
    for (const auto& [x, y] : clay) {
        x_min = std::min(x_min, x);
        x_max = std::max(x_max, x);
        // y_min = std::min(y_min, y);
        // y_max = std::max(y_max, y);
    }
    for (const auto& [x, y] : water) {
        y_min = std::min(y_min, y);
        y_max = std::max(y_max, y);
    }
    if (y_max > depth_from_bottom) {
        y_min = std::max(y_min, y_max - depth_from_bottom);
    }

    for (auto y = y_min; y <= y_max + 1; ++y) {
        for (auto x = x_min - 1; x <= x_max + 1; ++x) {
            char s;
            const auto coord = std::pair(x, y);
            if (water.contains(coord) && clay.contains(coord)) {
                s = '~';
            } else if (water.contains(coord)) {
                s = '|';
            } else if (clay.contains(coord)) {
                s = '#';
            } else {
                s = ' ';
            }
            std::cout << s;
        }
        std::cout << '\n';
    }
    std::cout << "===\n";
}

unsigned long max_size = 0;

bool Day17::water_flow(
    std::unordered_set<std::pair<unsigned int, unsigned int>, utils::UIntPairHash>& clay,
    std::unordered_set<std::pair<unsigned int, unsigned int>, utils::UIntPairHash>& water,
    const std::pair<unsigned int, unsigned int> flow, const unsigned int& max_y) const {
    const auto [x, y] = flow;
    if (y == max_y)
        return false;

    const auto down = std::pair(x, y + 1);
    if (!clay.contains(down) && !water.contains(down)) {
        water.insert(down);
        if (!water_flow(clay, water, down, max_y))
            return false;
    } else if (!clay.contains(down) && water.contains(down)) {
        return false;
    }

    auto x_l = x;
    while (!clay.contains(std::pair(x_l - 1, y)) && clay.contains(std::pair(x_l - 1, y + 1))) {
        water.insert(std::pair(--x_l, y));
    }
    auto x_r = x;
    while (!clay.contains(std::pair(x_r + 1, y)) && clay.contains(std::pair(x_r + 1, y + 1))) {
        water.insert(std::pair(++x_r, y));
    }

    auto left_blocked = clay.contains(std::pair(x_l - 1, y));
    auto right_blocked = clay.contains(std::pair(x_r + 1, y));
    if (left_blocked && right_blocked) {
        for (auto x_b = x_l; x_b <= x_r; ++x_b)
            clay.insert(std::pair(x_b, y));
    }

    // display(clay, water, 30);

    if (!left_blocked) {
        water.insert(std::pair(--x_l, y));
        left_blocked = water_flow(clay, water, std::pair(x_l, y), max_y);
    }
    if (!right_blocked) {
        water.insert(std::pair(++x_r, y));
        right_blocked = water_flow(clay, water, std::pair(x_r, y), max_y);
    }

    return left_blocked && right_blocked;
}

ResultType Day17::solvePart1() {
    auto [clay, min_y, max_y] = parse_input();
    const auto water_src = std::pair(500, min_y);

    // std::cout << min_y << ' ' << max_y << '\n';

    std::unordered_set<std::pair<unsigned int, unsigned int>, utils::UIntPairHash> water = {
        water_src
    };
    water_flow(clay, water, water_src, max_y);

    // display(clay, water, max_y);

    return water.size();
}

ResultType Day17::solvePart2() {
    auto [clay, min_y, max_y] = parse_input();
    const auto water_src = std::pair(500, min_y);
    const auto clay_size = clay.size();

    std::unordered_set<std::pair<unsigned int, unsigned int>, utils::UIntPairHash> water = {
        water_src
    };
    water_flow(clay, water, water_src, max_y);

    return clay.size() - clay_size;
}

} // namespace adventofcode
