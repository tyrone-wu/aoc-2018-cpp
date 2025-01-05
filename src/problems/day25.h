#pragma once

#include "../DaySolver.h"
#include <array>

namespace adventofcode
{
class Day25 final : public DaySolver {
public:
    Day25();
    ~Day25() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    [[nodiscard]] const std::vector<std::array<short, 4>> parse_input() const;
};
} // namespace adventofcode
