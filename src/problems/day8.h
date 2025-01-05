#pragma once

#include "../DaySolver.h"

namespace adventofcode
{
class Day8 final : public DaySolver {
public:
    Day8();
    ~Day8() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    [[nodiscard]] const std::vector<unsigned char> parse_input() const;

    [[nodiscard]] int dfs(std::vector<unsigned char>::const_iterator& iter) const;

    [[nodiscard]] int dfs_p2(std::vector<unsigned char>::const_iterator& iter) const;
};
} // namespace adventofcode
