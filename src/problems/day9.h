#pragma once

#include "../DaySolver.h"

namespace adventofcode
{
class Day9 final : public DaySolver {
public:
    Day9();
    ~Day9() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    [[nodiscard]] const std::pair<unsigned int, unsigned int> parse_input() const;

    [[nodiscard]] unsigned long solve(const unsigned int players,
                                      const unsigned int last_marble_worth) const;
};
} // namespace adventofcode
