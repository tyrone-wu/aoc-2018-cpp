#include "day11.h"
#include <array>
#include <climits>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace adventofcode
{
Day11::Day11() : DaySolver(11) {
    // Data for tests
    test_data = R"(42)";
    test_results = { "21,61", "232,251,12" };
}

const std::array<std::array<int, 301>, 301> Day11::generate_grid(const unsigned long serial) const {
    std::array<std::array<int, 301>, 301> grid = {};
    for (auto y = 1UL; y < 301; y++) {
        for (auto x = 1UL; x < 301; x++) {
            const auto rack_id = x + 10;
            const auto power_level =
                static_cast<int>((((rack_id * y + serial) * rack_id) / 100) % 10) - 5;
            grid[y][x] = power_level;
        }
    }
    return grid;
}

const std::array<std::array<int, 301>, 301>
Day11::generate_sat(const std::array<std::array<int, 301>, 301>& grid) const {
    std::array<std::array<int, 301>, 301> sat = {};
    for (auto y = 1UL; y < 301; y++) {
        for (auto x = 1UL; x < 301; x++) {
            const auto up = sat[y - 1][x];
            const auto left = sat[y][x - 1];
            const auto up_left = sat[y - 1][x - 1];
            sat[y][x] = grid[y][x] + up + left - up_left;
        }
    }
    return sat;
}

const std::pair<std::string, int>
Day11::largest_total_power(const unsigned long size,
                           const std::array<std::array<int, 301>, 301>& sat) const {
    std::pair<unsigned long, unsigned long> id;
    int largest_power = INT_MIN;

    for (auto y = 1UL; y < 301 - size + 1; y++) {
        for (auto x = 1UL; x < 301 - size + 1; x++) {
            const auto down = sat[y + size - 1][x - 1];
            const auto right = sat[y - 1][x + size - 1];
            const auto down_right = sat[y + size - 1][x + size - 1];
            const auto power = sat[y - 1][x - 1] - down - right + down_right;

            if (largest_power < power) {
                id = std::pair(x, y);
                largest_power = power;
            }
        }
    }

    std::stringstream ss;
    ss << std::get<0>(id) << ',' << std::get<1>(id);
    return std::pair(ss.str(), largest_power);
}

ResultType Day11::solvePart1() {
    const auto serial = std::stoul(std::string(input_data.begin(), input_data.end()));
    const auto grid = generate_grid(serial);
    const auto sat = generate_sat(grid);
    const auto id = std::get<0>(largest_total_power(3, sat));
    return id;
}

ResultType Day11::solvePart2() {
    const auto serial = std::stoul(std::string(input_data.begin(), input_data.end()));
    const auto grid = generate_grid(serial);
    const auto sat = generate_sat(grid);

    std::string pos;
    unsigned long pos_size;
    auto largest_power = INT_MIN;
    for (auto size = 1UL; size <= 300; size++) {
        const auto [id, power] = largest_total_power(size, sat);
        if (largest_power < power) {
            pos = id;
            pos_size = size;
            largest_power = power;
        }
    }
    return pos + ',' + std::to_string(pos_size);
}

} // namespace adventofcode
