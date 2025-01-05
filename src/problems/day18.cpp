#include "day18.h"
#include <algorithm>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <unordered_map>

namespace adventofcode
{
Day18::Day18() : DaySolver(18) {
    // Data for tests
    test_data = R"(.#.#...|#.
.....#|##|
.|..|...#.
..|#.....#
#.#|||#|#|
...#.||...
.|....|...
||...#|.#|
|.||||..|.
...#.|..|.)";
    test_results = { 1147, 0 };
}

const std::vector<std::vector<char>> Day18::parse_input() const {
    std::vector<std::vector<char>> map = { std::vector(input_data.find('\n') + 2, '.') };
    auto lines = input_data | std::views::split('\n');
    std::ranges::for_each(lines, [&map](const auto& line) {
        std::vector<char> row = { '.' };
        row.insert(row.end(), line.begin(), line.end());
        row.push_back('.');
        map.push_back(row);
    });
    map.push_back(std::vector(input_data.find('\n') + 2, '.'));
    return map;
}

void Day18::display(const std::vector<std::vector<char>>& map) const {
    for (const auto& row : map) {
        for (const auto& acre : row)
            std::cout << acre;
        std::cout << '\n';
    }
    std::cout << "===\n";
}

const std::pair<unsigned char, unsigned char>
Day18::adjacent(const std::vector<std::vector<char>>& map, const unsigned long x_acre,
                const unsigned long y_acre) const {
    unsigned char trees = 0, lumberyards = 0;
    for (auto y = y_acre - 1; y <= y_acre + 1; ++y) {
        for (auto x = x_acre - 1; x <= x_acre + 1; ++x) {
            if (x == x_acre && y == y_acre)
                continue;

            switch (map[y][x]) {
            case '|': {
                ++trees;
                break;
            }
            case '#': {
                ++lumberyards;
                break;
            }
            default:
            }
        }
    }
    return std::pair(trees, lumberyards);
}

const std::string Day18::map_hash(const std::vector<std::vector<char>>& map) const {
    std::string hash;
    hash.reserve(map.size() * map[0].size());

    for (const auto& row : map) {
        for (const auto& acre : row)
            hash.push_back(acre);
    }

    return hash;
}

unsigned long Day18::simulate(const unsigned long minutes) const {
    auto map = parse_input();
    auto next = map;
    std::unordered_map<std::string, unsigned long> seen;

    for (auto min = 0UL; min < minutes; ++min) {
        for (auto y = 1UL; y < map.size() - 1; ++y) {
            for (auto x = 1UL; x < map[0].size() - 1; ++x) {
                const auto [trees, lumberyards] = adjacent(map, x, y);
                switch (map[y][x]) {
                case '.': {
                    if (trees >= 3)
                        next[y][x] = '|';
                    break;
                }
                case '|': {
                    if (lumberyards >= 3)
                        next[y][x] = '#';
                    break;
                }
                case '#': {
                    if (trees == 0 || lumberyards == 0)
                        next[y][x] = '.';
                    break;
                }
                default:
                    throw std::runtime_error("invalid acre");
                }
            }
        }

        for (auto y = 1UL; y < map.size() - 1; ++y) {
            for (auto x = 1UL; x < map[0].size() - 1; ++x)
                map[y][x] = next[y][x];
        }

        const auto hash = map_hash(map);
        if (seen.contains(hash)) {
            const auto min_cycle = min - seen[hash];
            min = ((minutes - min) / min_cycle) * min_cycle + min;
        } else {
            seen[hash] = min;
        }

        // display(map);
    }

    unsigned long trees = 0, lumberyards = 0;
    for (const auto& row : map) {
        for (const auto& acre : row) {
            if (acre == '|')
                ++trees;
            else if (acre == '#')
                ++lumberyards;
        }
    }
    return trees * lumberyards;
}

ResultType Day18::solvePart1() { return simulate(10); }

ResultType Day18::solvePart2() { return simulate(1000000000); }

} // namespace adventofcode
