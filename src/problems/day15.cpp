#include "day15.h"
#include <algorithm>
#include <climits>
#include <iostream>
#include <ranges>
#include <vector>

namespace adventofcode
{
Day15::Day15() : DaySolver(15) {
    // Data for tests
    test_data = R"(#####
#GG##
#.###
#..E#
#.#G#
#.E##
#####)";

    // #######
    // #.G...#
    // #...EG#
    // #.#.#G#
    // #..G#E#
    // #.....#
    // #######

    // #######
    // #G..#E#
    // #E#E.E#
    // #G.##.#
    // #...#E#
    // #...E.#
    // #######

    // #######
    // #E..EG#
    // #.#G.E#
    // #E.##E#
    // #G..#.#
    // #..E#.#
    // #######

    // #######
    // #E.G#.#
    // #.#G..#
    // #G.#.G#
    // #G..#.#
    // #...E.#
    // #######

    // #######
    // #.E...#
    // #.#..G#
    // #.###.#
    // #E#G#G#
    // #...#G#
    // #######

    // #########
    // #G......#
    // #.E.#...#
    // #..##..G#
    // #...##..#
    // #...#...#
    // #.G...G.#
    // #.....G.#
    // #########

    test_data2 = R"(####### 
#.G...# 
#...EG# 
#.#.#G#
#..G#E#
#.....# 
#######)";

    // #######
    // #.G...#
    // #...EG#
    // #.#.#G#
    // #..G#E#
    // #.....#
    // #######

    test_results = { 18740, 4988 };
}

const std::pair<std::vector<Day15::Unit>,
                std::unordered_set<std::pair<unsigned long, unsigned long>, utils::PairHash>>
Day15::parse_input() const {
    const auto map = input_data | std::views::split('\n') |
                     std::views::transform(
                         [](const auto&& line) { return std::vector(line.begin(), line.end()); }) |
                     std::ranges::to<std::vector>();

    std::vector<Day15::Unit> units;
    std::unordered_set<std::pair<unsigned long, unsigned long>, utils::PairHash> walls;
    for (auto y = 0UL; y < map.size(); ++y) {
        for (auto x = 0UL; x < map[0].size(); ++x) {
            switch (map[y][x]) {
            case 'E':
            case 'G': {
                units.push_back(Unit(static_cast<unsigned char>(units.size()), map[y][x] == 'E',
                                     static_cast<int>(x), static_cast<int>(y)));
                break;
            }
            case '#': {
                walls.insert(std::pair(x, y));
                break;
            }
            default:
            }
        }
    }

    return std::pair(units, walls);
}

bool Day15::combat_finished(const std::vector<Unit>& units) const {
    unsigned char elves = 0, goblins = 0;
    for (const auto& u : units) {
        if (u.elf)
            ++elves;
        else
            ++goblins;
    }
    return elves == 0 || goblins == 0;
}

void Day15::display(const std::vector<Unit>& units,
                    const std::unordered_set<std::pair<unsigned long, unsigned long>,
                                             utils::PairHash>& walls) const {
    auto max_x = 0UL, max_y = 0UL;
    for (const auto& [x, y] : walls) {
        max_x = std::max(max_x, x);
        max_y = std::max(max_y, y);
    }

    for (auto y = 0UL; y <= max_y; ++y) {
        for (auto x = 0UL; x <= max_x; ++x) {
            char space = '.';
            if (walls.contains(std::pair(x, y))) {
                space = '#';
            } else {
                for (const auto& u : units) {
                    if (x == static_cast<unsigned long>(u.x) &&
                        y == static_cast<unsigned long>(u.y)) {
                        space = u.elf ? 'E' : 'G';
                        break;
                    }
                }
            }
            std::cout << space;
        }
        std::cout << '\n';
    }
    std::cout << "===\n";
}

ResultType Day15::solvePart1() {
    const auto input = parse_input();
    auto units = std::get<0>(input);
    const auto walls = std::get<1>(input);

    // display(units, walls);

    auto round = 0;
    while (true) {
        std::sort(units.begin(), units.end(), [](const auto& u1, const auto& u2) {
            return u1.y != u2.y ? u1.y < u2.y : u1.x < u2.x;
        });

        bool last_unit_attacked = false;
        for (auto& u : units) {
            if (u.hit_points <= 0)
                continue;

            if (!Unit::adjacent_to_enemy(u.id, u.x, u.y, u.elf, units))
                u.move(units, walls);

            last_unit_attacked = u.try_attack(units);
        }

        for (auto iter = units.begin(); iter != units.end();) {
            if (iter->hit_points <= 0)
                iter = units.erase(iter);
            else
                ++iter;
        }

        if (combat_finished(units)) {
            if (last_unit_attacked)
                ++round;

            // std::cout << "round: " << round << '\n';
            // display(units, walls);

            break;
        }
        ++round;

        // std::cout << "round: " << round << '\n';
        // display(units, walls);
    }

    long remaining_hp = 0;
    for (const auto& u : units)
        remaining_hp += u.hit_points;
    return remaining_hp * round;
}

ResultType Day15::solvePart2() {
    const auto input = parse_input();
    const auto original_units = std::get<0>(input);
    const auto walls = std::get<1>(input);

    for (auto ap = 4;; ++ap) {
        auto units = original_units;
        for (auto& u : units) {
            if (u.elf)
                u.attack_power = ap;
        }

        bool rip_elf = false;
        auto round = 0;
        while (true) {
            std::sort(units.begin(), units.end(), [](const auto& u1, const auto& u2) {
                return u1.y != u2.y ? u1.y < u2.y : u1.x < u2.x;
            });

            bool last_unit_attacked = false;
            for (auto& u : units) {
                if (u.hit_points <= 0)
                    continue;

                if (!Unit::adjacent_to_enemy(u.id, u.x, u.y, u.elf, units))
                    u.move(units, walls);

                last_unit_attacked = u.try_attack(units);
            }

            for (auto iter = units.begin(); iter != units.end();) {
                if (iter->hit_points <= 0) {
                    if (iter->elf) {
                        rip_elf = true;
                        break;
                    }
                    iter = units.erase(iter);
                } else {
                    ++iter;
                }
            }

            if (rip_elf)
                break;

            if (combat_finished(units)) {
                if (last_unit_attacked)
                    ++round;

                // std::cout << "round: " << round << '\n';
                // display(units, walls);

                break;
            }
            ++round;

            // std::cout << "round: " << round << '\n';
            // display(units, walls);
        }

        if (!rip_elf) {
            long remaining_hp = 0;
            for (const auto& u : units)
                remaining_hp += u.hit_points;
            return remaining_hp * round;
        }
    }
    return 0;
}

} // namespace adventofcode
