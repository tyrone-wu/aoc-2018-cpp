#pragma once

#include "../DaySolver.h"
#include <charconv>
#include <optional>
#include <ranges>
#include <string_view>

namespace adventofcode
{
class Day23 final : public DaySolver {
public:
    Day23();
    ~Day23() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    struct Coord {
        long x = 0;
        long y = 0;
        long z = 0;

        Coord() = default;

        explicit Coord(const long x, const long y, const long z) : x(x), y(y), z(z) {}
    };

    struct Nanobot {
        Coord coord;
        long radius;

        explicit Nanobot(const std::string_view& input) {
            const auto s = input.cbegin();
            const auto components = std::string_view(s + input.find('<') + 1, s + input.find('>')) |
                                    std::views::split(',') |
                                    std::views::transform([](const auto&& c) {
                                        long component;
                                        std::from_chars(c.begin(), c.end(), component);
                                        return component;
                                    }) |
                                    std::ranges::to<std::vector>();
            coord = Coord(components[0], components[1], components[2]);
            std::from_chars(s + input.find_last_of('=') + 1, input.end(), radius);
        }
    };

    [[nodiscard]] const std::vector<Nanobot> parse_input() const;

    [[nodiscard]] long man_dist(const Coord& c1, const Coord& c2) const;

    [[nodiscard]] long binary_search(const std::vector<long>& comps) const;

    [[nodiscard]] long in_range(const Coord& coord, const std::vector<Nanobot>& nanobots,
                                const std::optional<long> radius) const;
};
} // namespace adventofcode
