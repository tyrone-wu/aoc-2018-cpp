#include "day1.h"
#include <algorithm>
#include <ranges>
#include <set>

namespace adventofcode
{
Day1::Day1() : DaySolver(1) {
    // Data for tests
    test_data = R"(+1
-2
+3
+1
)";
    test_results = { 3L, 2L };
}

ResultType Day1::solvePart1() {
    long frequency = 0;
    auto lines = std::views::split(input_data, '\n');
    std::ranges::for_each(lines, [&frequency](const std::ranges::subrange<const char*> line) {
        if (line.empty())
            return;
        const std::string magnitude_str(line.begin() + 1, line.end());
        int magnitude = std::stoi(magnitude_str);
        switch (line.front()) {
        case '+':
            frequency += magnitude;
            break;
        case '-':
            frequency -= magnitude;
            break;
        default:
            std::abort();
        }
    });
    return frequency;
}

const std::vector<int> Day1::parse_input(const std::string_view input) const {
    auto lines = std::views::split(input, '\n');
    std::vector<int> changes;
    std::ranges::for_each(lines, [&changes](const std::ranges::subrange<const char*> line) {
        if (line.empty())
            return;
        const std::string line_str(line.begin() + 1, line.end());
        int change = std::stoi(line_str);
        changes.push_back(change * (line.front() == '+' ? 1 : -1));
    });
    return changes;
}

ResultType Day1::solvePart2() {
    long frequency = 0;
    const std::vector<int> changes = parse_input(input_data);
    std::set<long> seen = {};
    size_t i = 0;
    while (!seen.contains(frequency)) {
        seen.insert(frequency);
        frequency += changes[i];
        i = (i + 1) % changes.size();
    }
    return frequency;
}

} // namespace adventofcode
