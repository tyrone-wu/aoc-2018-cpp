#pragma once

#include "../DaySolver.h"
#include <array>

namespace adventofcode
{
class Day11 final : public DaySolver {
public:
    Day11();
    ~Day11() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    [[nodiscard]] const std::array<std::array<int, 301>, 301>
    generate_grid(const unsigned long serial) const;

    [[nodiscard]] const std::array<std::array<int, 301>, 301>
    generate_sat(const std::array<std::array<int, 301>, 301>& grid) const;

    [[nodiscard]] const std::pair<std::string, int>
    largest_total_power(const unsigned long size,
                        const std::array<std::array<int, 301>, 301>& sat) const;
};
} // namespace adventofcode
