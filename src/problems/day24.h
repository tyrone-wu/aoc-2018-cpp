#pragma once

#include "../DaySolver.h"
#include <algorithm>
#include <charconv>
#include <iostream>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <vector>

namespace adventofcode
{
class Day24 final : public DaySolver {
public:
    Day24();
    ~Day24() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    struct Group {
        char id = -1;
        bool immune_system;
        long units;
        long hit_points;
        long attack_damage;
        std::string attack_type;
        unsigned char initiative;
        std::vector<std::string> weaknesses;
        std::vector<std::string> immunities;
        std::optional<unsigned char> attacking;
        std::optional<unsigned char> attacked_by;

        explicit Group(const bool is, const std::string_view& input) {
            immune_system = is;
            attacking = std::nullopt;
            attacked_by = std::nullopt;

            const auto tokens =
                input | std::views::split(' ') |
                std::views::transform([](const auto&& token) { return std::string_view(token); }) |
                std::ranges::to<std::vector>();

            std::from_chars(tokens[0].begin(), tokens[0].end(), units);
            std::from_chars(tokens[4].begin(), tokens[4].end(), hit_points);
            const auto dmg = std::find(tokens.begin(), tokens.end(), "does") + 1;
            std::from_chars(dmg->begin(), dmg->end(), attack_damage);
            attack_type = std::string(*(std::find(tokens.begin(), tokens.end(), "damage") - 1));
            std::from_chars(tokens.back().begin(), tokens.back().end(), initiative);

            const auto open_paren = input.find('(');
            if (open_paren != std::string_view::npos) {
                const std::string_view parenth(input.cbegin() + open_paren,
                                               input.cbegin() + input.find(')'));
                const auto s = parenth.cbegin();
                const auto semicolon = parenth.find(';');

                const auto weak_to = parenth.find("weak to ");
                if (weak_to != std::string_view::npos) {
                    weaknesses = std::string_view(s + weak_to + 8,
                                                  s + ((semicolon != std::string_view::npos &&
                                                        semicolon > weak_to)
                                                           ? semicolon
                                                           : parenth.size())) |
                                 std::views::split(std::string_view(", ")) |
                                 std::views::transform([](const auto&& w) {
                                     return std::string(w.begin(), w.end());
                                 }) |
                                 std::ranges::to<std::vector>();
                }

                const auto immune_to = parenth.find("immune to ");
                if (immune_to != std::string_view::npos) {
                    immunities = std::string_view(s + immune_to + 10,
                                                  s + ((semicolon != std::string_view::npos &&
                                                        semicolon > immune_to)
                                                           ? semicolon
                                                           : parenth.size())) |
                                 std::views::split(std::string_view(", ")) |
                                 std::views::transform([](const auto&& w) {
                                     return std::string(w.begin(), w.end());
                                 }) |
                                 std::ranges::to<std::vector>();
                }
            }
        }

        void debug(const bool verbose) const {
            std::cout << (immune_system ? "immune sy = " : "infection = ") << "units: " << units
                      << " hp: " << hit_points << '\n';
            if (verbose) {
                std::cout << "  - dmg: " << attack_damage << " type: " << attack_type
                          << " init: " << static_cast<int>(initiative) << '\n';

                std::cout << "  - weak: " << (weaknesses.empty() ? "none" : "");
                for (const auto& w : weaknesses)
                    std::cout << w << ' ';
                std::cout << '\n';

                std::cout << "  - immune: " << (immunities.empty() ? "none" : "");
                for (const auto& im : immunities)
                    std::cout << im << ' ';
                std::cout << '\n';
            }
        }

        long effective_power() const { return units * attack_damage; }

        bool weak_to(const std::string& type) const {
            return std::find(weaknesses.begin(), weaknesses.end(), type) != weaknesses.end();
        }

        bool immune_to(const std::string& type) const {
            return std::find(immunities.begin(), immunities.end(), type) != immunities.end();
        }

        long damage(const Group& enemy) const {
            if (units <= 0 || enemy.units <= 0)
                throw std::runtime_error("no units to attack");

            if (immune_system == enemy.immune_system || enemy.immune_to(attack_type))
                return 0;

            auto damage = effective_power();
            if (enemy.weak_to(attack_type))
                damage *= 2;

            return damage;
        }

        long total_hp() const { return units * hit_points; }

        void boost(const long boost) {
            if (immune_system)
                attack_damage += boost;
        }
    };

    struct Army {
        std::vector<Group> groups;

        explicit Army(const std::string_view& input) {
            const bool immune_system = *(input.cbegin() + 1) == 'm';
            groups = std::string_view(input.cbegin() + input.find('\n') + 1, input.end()) |
                     std::views::split('\n') |
                     std::views::transform([immune_system](const auto&& line) {
                         return Group(immune_system, std::string_view(line));
                     }) |
                     std::ranges::to<std::vector>();
        }
    };

    [[nodiscard]] const std::vector<Group> parse_input() const;

    [[nodiscard]] bool combat_finished(const std::vector<Group>& groups) const;

    [[nodiscard]] long units_remaining(const std::vector<Group>& groups) const;
};
} // namespace adventofcode
