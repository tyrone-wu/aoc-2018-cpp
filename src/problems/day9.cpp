#include "day9.h"
#include <charconv>
#include <cstdio>
#include <iterator>
#include <list>
#include <ranges>

namespace adventofcode
{
Day9::Day9() : DaySolver(9) {
    // Data for tests
    test_data = R"(30 players; last marble is worth 5807 points)";
    test_results = { 37305UL, 0 };
}

const std::pair<unsigned int, unsigned int> Day9::parse_input() const {
    const auto tokens = input_data | std::views::split(' ') | std::ranges::to<std::vector>();
    unsigned int players, last_marble_worth;
    std::from_chars(tokens[0].begin(), tokens[0].end(), players);
    std::from_chars(tokens[6].begin(), tokens[6].end(), last_marble_worth);
    return std::pair(players, last_marble_worth);
}

unsigned long Day9::solve(const unsigned int players, const unsigned int last_marble_worth) const {
    std::vector<unsigned long> scores;
    scores.reserve(players);
    for (auto i = 0U; i < players; i++)
        scores.push_back(0);

    std::list<unsigned long> marbles = { 0 };
    auto iter = marbles.begin();
    int i = 0;

    for (unsigned long marble = 1U, player = 0U; marble <= last_marble_worth;
         marble++, player = (player + 1) % players) {
        const auto marbles_size = static_cast<int>(marbles.size());
        const auto prev_i = i;
        if (marble % 23 != 0) {
            i = (i + 1) % marbles_size + 1;
            if (i >= prev_i) {
                std::advance(iter, i - prev_i);
            } else {
                iter = marbles.begin();
                std::advance(iter, i);
            }

            iter = marbles.insert(iter, marble);
        } else {
            i = (i + marbles_size - 7) % marbles_size;
            if (i <= prev_i) {
                std::advance(iter, i - prev_i);
            } else {
                iter = marbles.end();
                std::advance(--iter, i - marbles_size);
            }

            scores[player] += marble + *iter;
            iter = marbles.erase(iter);
        }
    }

    return *std::max_element(scores.begin(), scores.end());
}

ResultType Day9::solvePart1() {
    const auto [players, last_marble_worth] = parse_input();
    return solve(players, last_marble_worth);
}

ResultType Day9::solvePart2() {
    const auto [players, last_marble_worth] = parse_input();
    return solve(players, last_marble_worth * 100);
}

} // namespace adventofcode
