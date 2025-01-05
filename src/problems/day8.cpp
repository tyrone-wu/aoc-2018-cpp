#include "day8.h"
#include <charconv>
#include <ranges>

namespace adventofcode
{
Day8::Day8() : DaySolver(8) {
    // Data for tests
    test_data = R"(2 3 0 3 10 11 12 1 1 0 1 99 2 1 1 2)";
    test_results = { 138, 66 };
}

const std::vector<unsigned char> Day8::parse_input() const {
    auto nums = input_data | std::views::split(' ');
    return nums | std::views::transform([](const auto&& r) {
               unsigned char n;
               std::from_chars(r.begin(), r.end(), n);
               return n;
           }) |
           std::ranges::to<std::vector>();
}

int Day8::dfs(std::vector<unsigned char>::const_iterator& iter) const {
    int sum = 0;
    const auto child_nodes = *iter++;
    const auto num_metadata = *iter++;
    for (auto i = 0; i < child_nodes; i++)
        sum += dfs(iter);
    for (auto i = 0; i < num_metadata; i++, iter++)
        sum += *iter;
    return sum;
}

ResultType Day8::solvePart1() {
    const auto license = parse_input();
    auto iter = license.begin();
    return dfs(iter);
}

int Day8::dfs_p2(std::vector<unsigned char>::const_iterator& iter) const {
    const auto child_nodes = *iter++;
    const auto num_metadata = *iter++;
    int value = 0;
    if (child_nodes == 0) {
        for (auto i = 0; i < num_metadata; i++, iter++)
            value += *iter;
    } else {
        std::vector<int> child_metadata;
        for (auto i = 0; i < child_nodes; i++)
            child_metadata.push_back(dfs_p2(iter));

        for (auto i = 0; i < num_metadata; i++, iter++) {
            if (*iter <= child_metadata.size())
                value += child_metadata[*iter - 1];
        }
    }
    return value;
}

ResultType Day8::solvePart2() {
    const auto license = parse_input();
    auto iter = license.begin();
    return dfs_p2(iter);
}

} // namespace adventofcode
