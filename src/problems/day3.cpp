#include "day3.h"
#include <algorithm>
#include <iterator>
#include <ranges>
#include <unordered_map>
#include <unordered_set>

namespace adventofcode
{
Day3::Day3() : DaySolver(3) {
    // Data for tests
    test_data = R"(#1 @ 1,3: 4x4
#2 @ 3,1: 4x4
#3 @ 5,5: 2x2)";
    test_results = { 4UL, 3 };
}

const std::vector<Day3::Claim> Day3::parse_input() const {
    auto lines = input_data | std::views::split('\n');
    return lines | std::views::transform([](const auto&& line) { return Claim(line); }) |
           std::ranges::to<std::vector>();
}

const std::unordered_map<unsigned long, std::vector<int>>
Day3::solve(const std::vector<Day3::Claim>& claims) const {
    std::unordered_map<unsigned long, std::vector<int>> fabric;
    for (const auto& claim : claims) {
        for (const auto& c : claim.claim_range()) {
            unsigned long coord = (get<0>(c) << 32UL) | get<1>(c);
            if (fabric.find(coord) == fabric.end())
                fabric[coord] = {};
            fabric[coord].push_back(claim.id);
        }
    }
    return fabric;
}

ResultType Day3::solvePart1() {
    const auto claims = parse_input();
    const auto fabric = solve(claims);
    unsigned long total_overlaps = 0;
    for (const auto& [_, overlaps] : fabric) {
        if (overlaps.size() > 1)
            total_overlaps++;
    }
    return total_overlaps;
}

ResultType Day3::solvePart2() {
    const auto claims = parse_input();
    const auto fabric = solve(claims);
    std::unordered_set<int> claim_ids;
    transform(claims.begin(), claims.end(), inserter(claim_ids, claim_ids.end()),
              [](const auto& claim) { return claim.id; });

    for (const auto& [_, overlaps] : fabric) {
        if (overlaps.size() > 1) {
            for (const auto& id : overlaps)
                claim_ids.erase(id);
        }
    }
    return *claim_ids.begin();
}

} // namespace adventofcode
