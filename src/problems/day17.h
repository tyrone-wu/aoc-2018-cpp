#pragma once

#include "../DaySolver.h"
#include "../utils.h"
#include <unordered_set>

namespace adventofcode
{
class Day17 final : public DaySolver {
public:
    Day17();
    ~Day17() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    [[nodiscard]] const std::tuple<
        std::unordered_set<std::pair<unsigned int, unsigned int>, utils::UIntPairHash>,
        unsigned int, unsigned int>
    parse_input() const;

    [[nodiscard]] const std::vector<unsigned int> parse_range(const std::string_view& input) const;

    bool water_flow(
        std::unordered_set<std::pair<unsigned int, unsigned int>, utils::UIntPairHash>& clay,
        std::unordered_set<std::pair<unsigned int, unsigned int>, utils::UIntPairHash>& water,
        const std::pair<unsigned int, unsigned int> flow, const unsigned int& max_y) const;

    void display(
        const std::unordered_set<std::pair<unsigned int, unsigned int>, utils::UIntPairHash>& clay,
        const std::unordered_set<std::pair<unsigned int, unsigned int>, utils::UIntPairHash>& water,
        const unsigned int depth_from_bottom) const;
};
} // namespace adventofcode
