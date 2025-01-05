#include "day2.h"
#include <algorithm>
#include <cstdio>
#include <ranges>

namespace adventofcode
{
Day2::Day2() : DaySolver(2) {
    // Data for tests
    test_data = R"(abcdef
bababc
abbcde
abcccd
aabcdd
abcdee
ababab
)";
    test_data2 = R"(fghij
fguij
)";
    test_results = { 12UL, "fgij" };
}

ResultType Day2::solvePart1() {
    auto lines = std::views::split(input_data, '\n');
    unsigned long two = 0;
    unsigned long three = 0;
    std::ranges::for_each(lines, [&two, &three](const std::ranges::subrange<const char*> line) {
        if (line.empty())
            return;

        int freq[26] = {};
        for (const char& c : line)
            freq[c - 'a']++;

        bool hasTwo = false;
        bool hasThree = false;
        for (const int& f : freq) {
            if (f == 2)
                hasTwo = true;
            if (f == 3)
                hasThree = true;
            if (hasTwo && hasThree)
                break;
        }
        if (hasTwo)
            two++;
        if (hasThree)
            three++;
    });
    return two * three;
}

const std::vector<std::ranges::subrange<const char*>>
Day2::parse_input(const std::string_view input) const {
    auto lines = std::views::split(input, '\n');
    return lines | std::views::transform([](const auto line) { return line; }) |
           std::ranges::to<std::vector>();
}

ResultType Day2::solvePart2() {
    const auto boxes = parse_input(input_data);
    for (const auto&& [i, id_i] : std::views::enumerate(boxes)) {
        for (size_t j = static_cast<size_t>(i) + 1; j < boxes.size(); j++) {
            const auto& id_j = boxes[j];
            std::string common;
            for (long k = 0; k < static_cast<long>(id_i.size()); k++) {
                if (id_i[k] == id_j[k])
                    common.push_back(id_i[k]);
            }
            if (common.size() + 1 == id_i.size())
                return common;
        }
    }
    return "None";
}

} // namespace adventofcode
