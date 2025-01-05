#pragma once

#include "../DaySolver.h"

namespace adventofcode
{
class Day18 final : public DaySolver {
public:
    Day18();
    ~Day18() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    [[nodiscard]] const std::vector<std::vector<char>> parse_input() const;

    void display(const std::vector<std::vector<char>>& map) const;

    [[nodiscard]] const std::pair<unsigned char, unsigned char>
    adjacent(const std::vector<std::vector<char>>& map, const unsigned long x_acre,
             const unsigned long y_acre) const;

    [[nodiscard]] unsigned long simulate(const unsigned long minutes) const;

    [[nodiscard]] const std::string map_hash(const std::vector<std::vector<char>>& map) const;
};
} // namespace adventofcode
