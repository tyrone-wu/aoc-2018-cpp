#pragma once

#include "../DaySolver.h"
#include <unordered_map>

namespace adventofcode
{
class Day7 final : public DaySolver {
public:
    Day7();
    ~Day7() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    [[nodiscard]] const std::tuple<std::unordered_map<char, std::vector<char>>,
                                   std::unordered_map<char, unsigned int>, std::vector<char>>
    parse_input() const;
};
} // namespace adventofcode
