#include "day5.h"
#include <algorithm>
#include <cctype>
#include <climits>
#include <iterator>
#include <ranges>

namespace adventofcode
{
Day5::Day5() : DaySolver(5) {
    // Data for tests
    test_data = R"(dabAcCaCBAcCcaDA)";
    test_results = { 10UL, 4UL };
}

size_t Day5::solve(std::vector<char> polymer) const {
    size_t i = 0;
    while (i + 1 < polymer.size()) {
        if (polymer[i] != polymer[i + 1] && tolower(polymer[i]) == tolower(polymer[i + 1])) {
            const auto s = polymer.begin() + static_cast<long>(i);
            polymer.erase(s, s + 2);
            if (i > 0)
                i--;
        } else {
            i++;
        }
    }
    return polymer.size();
}

ResultType Day5::solvePart1() {
    const auto polymer = input_data | std::ranges::to<std::vector>();
    return solve(polymer);
}

ResultType Day5::solvePart2() {
    const auto polymer = input_data | std::ranges::to<std::vector>();
    size_t min_size = INT_MAX;
    for (char u = 'a'; u <= 'z'; u++) {
        std::vector<char> filtered;
        copy_if(polymer.begin(), polymer.end(), back_inserter(filtered),
                [&u](const auto& c) { return tolower(c) != u; });
        min_size = std::min(min_size, solve(filtered));
    }
    return min_size;
}

} // namespace adventofcode
