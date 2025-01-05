#pragma once

#include "../DaySolver.h"

namespace adventofcode
{
class Day2 final : public DaySolver {
public:
    Day2();
    ~Day2() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    [[nodiscard]] const std::vector<std::ranges::subrange<const char*>>
    parse_input(const std::string_view input) const;
};
} // namespace adventofcode
