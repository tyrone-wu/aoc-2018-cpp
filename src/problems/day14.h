#pragma once

#include "../DaySolver.h"

namespace adventofcode
{
class Day14 final : public DaySolver {
public:
    Day14();
    ~Day14() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    void push_score(std::vector<unsigned char>& scores, const unsigned char score) const;

    void display(const std::vector<unsigned char>& scores) const;

    [[nodiscard]] bool matches(const std::vector<unsigned char>& scores, unsigned long& i,
                               const std::vector<unsigned char>& target) const;
};
} // namespace adventofcode
