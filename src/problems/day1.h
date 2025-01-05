#pragma once

#include "../DaySolver.h"

namespace adventofcode
{
class Day1 final : public DaySolver {
public:
    Day1();
    ~Day1() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    [[nodiscard]] const std::vector<int> parse_input(const std::string_view input) const;
};
} // namespace adventofcode
