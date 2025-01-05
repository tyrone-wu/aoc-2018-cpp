#include "day14.h"
#include <iostream>
#include <ranges>
#include <string>

namespace adventofcode
{
Day14::Day14() : DaySolver(14) {
    // Data for tests
    test_data = R"(2018)";
    test_data2 = R"(59414)";
    test_results = { "5941429882", 2018UL };
}

void Day14::push_score(std::vector<unsigned char>& scores, const unsigned char score) const {
    if (score >= 10)
        push_score(scores, score / 10);
    scores.push_back(score % 10);
}

void Day14::display(const std::vector<unsigned char>& scores) const {
    for (const auto& s : scores)
        std::cout << static_cast<int>(s) << ' ';
    std::cout << '\n';
}

ResultType Day14::solvePart1() {
    const unsigned long cycles = std::stoul(std::string(input_data));

    unsigned long i = 0;
    unsigned long j = 1;
    std::vector<unsigned char> scores = { 3, 7 };
    while (scores.size() < cycles + 10) {
        const unsigned char new_score = scores.at(i) + scores.at(j);
        push_score(scores, new_score);
        i = (i + scores.at(i) + 1) % scores.size();
        j = (j + scores.at(j) + 1) % scores.size();
        // display(scores);
    }

    std::string improved_score;
    for (auto s = cycles; s < cycles + 10; ++s)
        improved_score.push_back(scores.at(s) + '0');
    return improved_score;
}

bool Day14::matches(const std::vector<unsigned char>& scores, unsigned long& i,
                    const std::vector<unsigned char>& target) const {
    while (i + target.size() <= scores.size()) {
        bool matches = true;
        for (auto j = i; j < i + target.size(); ++j) {
            if (scores.at(j) != target.at(j - i)) {
                matches = false;
                break;
            }
        }

        if (matches)
            return true;
        ++i;
    }
    return false;
}

ResultType Day14::solvePart2() {
    const std::vector<unsigned char> target =
        input_data |
        std::views::transform([](const auto& c) { return static_cast<unsigned char>(c - '0'); }) |
        std::ranges::to<std::vector>();

    unsigned long num_recipes = 0;
    unsigned long i = 0;
    unsigned long j = 1;
    std::vector<unsigned char> scores = { 3, 7 };
    while (!matches(scores, num_recipes, target)) {
        const unsigned char new_score = scores.at(i) + scores.at(j);
        push_score(scores, new_score);
        i = (i + scores.at(i) + 1) % scores.size();
        j = (j + scores.at(j) + 1) % scores.size();
    }

    return num_recipes;
}

} // namespace adventofcode
