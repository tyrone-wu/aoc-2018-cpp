#pragma once

#include "../DaySolver.h"
#include "../utils.h"
#include <array>
#include <climits>
#include <optional>
#include <queue>
#include <unordered_set>
#include <utility>

namespace adventofcode
{
class Day15 final : public DaySolver {
public:
    Day15();
    ~Day15() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    static constexpr std::array<std::pair<char, char>, 4> delta = {
        std::pair(0, -1),
        std::pair(-1, 0),
        std::pair(1, 0),
        std::pair(0, 1),
    };

    struct Unit {
        unsigned char id;
        bool elf;
        int x;
        int y;
        int hit_points = 200;
        int attack_power = 3;

        explicit Unit(unsigned char i, bool e, int x, int y) : id(i), elf(e), x(x), y(y) {}

        static bool adjacent_to_enemy(const unsigned char id, const int x, const int y,
                                      const bool elf, const std::vector<Unit>& units) {
            for (const auto& u : units) {
                if (id == u.id || elf == u.elf)
                    continue;

                for (const auto& [dx, dy] : delta) {
                    if (x + dx == u.x && y + dy == u.y && u.hit_points > 0)
                        return true;
                }
            }
            return false;
        }

        bool move(const std::vector<Unit>& units,
                  const std::unordered_set<std::pair<unsigned long, unsigned long>,
                                           utils::PairHash>& walls) {
            std::unordered_set<std::pair<int, int>, utils::PairHash> visited = { std::pair(x, y) };
            std::queue<std::tuple<int, int, int, unsigned long>> bfs;
            for (auto i = 0UL; i < delta.size(); ++i)
                bfs.push(std::tuple(x + delta[i].first, y + delta[i].second, 1, i));

            std::vector<std::tuple<int, int, unsigned long>> possible_moves;
            possible_moves.reserve(units.size());
            std::optional<int> min_steps = std::nullopt;
            while (!bfs.empty()) {
                const auto [x_c, y_c, steps, i] = bfs.front();
                bfs.pop();

                if (min_steps.has_value() && min_steps.value() < steps)
                    break;

                const auto coord = std::pair(x_c, y_c);
                if (visited.contains(coord) || walls.contains(coord))
                    continue;
                visited.insert(coord);

                bool in_unit = false;
                for (const auto& u : units) {
                    if (x_c == u.x && y_c == u.y && u.hit_points > 0) {
                        in_unit = true;
                        break;
                    }
                }
                if (in_unit)
                    continue;

                if (adjacent_to_enemy(id, x_c, y_c, elf, units) &&
                    (!min_steps.has_value() || min_steps.value() == steps)) {
                    min_steps = steps;
                    possible_moves.push_back(std::tuple(x_c, y_c, i));
                    continue;
                }

                for (const auto& [dx, dy] : delta)
                    bfs.push(std::tuple(x_c + dx, y_c + dy, steps + 1, i));
            }

            if (possible_moves.empty())
                return false;

            std::sort(possible_moves.begin(), possible_moves.end(),
                      [](const auto& m1, const auto& m2) {
                          const auto x_1 = std::get<0>(m1), y_1 = std::get<1>(m1),
                                     x_2 = std::get<0>(m2), y_2 = std::get<1>(m2);
                          return y_1 != y_2 ? y_1 < y_2 : x_1 < x_2;
                      });
            const auto& [dx, dy] = delta.at(std::get<2>(possible_moves.front()));
            x += dx;
            y += dy;
            return true;
        }

        bool try_attack(std::vector<Unit>& units) const {
            int min_health = INT_MAX;
            std::optional<unsigned char> attack = std::nullopt;
            for (auto& u : units) {
                if (id == u.id || elf == u.elf || u.hit_points <= 0)
                    continue;

                for (const auto& [dx, dy] : delta) {
                    if (x + dx == u.x && y + dy == u.y && u.hit_points < min_health) {
                        min_health = u.hit_points;
                        attack = u.id;
                    }
                }
            }

            if (attack.has_value()) {
                for (auto& u : units) {
                    if (attack.value() == u.id) {
                        u.hit_points -= attack_power;
                        return true;
                    }
                }
            }
            return false;
        }
    };

    [[nodiscard]] const std::pair<
        std::vector<Unit>,
        std::unordered_set<std::pair<unsigned long, unsigned long>, utils::PairHash>>
    parse_input() const;

    [[nodiscard]] bool combat_finished(const std::vector<Unit>& units) const;

    void display(const std::vector<Unit>& units,
                 const std::unordered_set<std::pair<unsigned long, unsigned long>, utils::PairHash>&
                     walls) const;
};
} // namespace adventofcode
