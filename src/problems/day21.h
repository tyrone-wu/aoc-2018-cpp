#pragma once

#include "../DaySolver.h"
#include "day16.h"
#include <optional>

namespace adventofcode
{
class Day21 final : public DaySolver {
public:
    Day21();
    ~Day21() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    // unsigned long run_program(Day16::Registers registers, const unsigned long ip_reg,
    //                           const std::vector<Day16::Instruction>& instructions) const;

    [[nodiscard]] const std::optional<unsigned long> solve(const bool p2) const;

    void debug(const Day16::Registers& regs) const;
};
} // namespace adventofcode
