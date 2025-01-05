#include "day12.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>

namespace adventofcode
{
Day12::Day12() : DaySolver(12) {
    // Data for tests
    test_data = R"(initial state: #..#.#..##......###...###

...## => #
..#.. => #
.#... => #
.#.#. => #
.#.## => #
.##.. => #
.#### => #
#.#.# => #
#.### => #
##.#. => #
##.## => #
###.. => #
###.# => #
####. => #)";
    test_results = { 325, 0 };
}

void Day12::pad_front(std::vector<Pot>& state, const std::vector<Pot>& base) const {
    auto f_iter = std::find_if(base.begin(), base.end(), [](const auto& p) { return p.plant; });
    for (auto i = 0; i < 4; i++) {
        if (f_iter == base.begin())
            state.push_back(Pot(false, f_iter->id - 4 + i));
        else
            f_iter--;
    }
}

void Day12::pad_end(std::vector<Pot>& state) const {
    auto b_iter = std::find_if(state.rbegin(), state.rend(), [](const auto& p) { return p.plant; });
    for (auto i = 0; i < 4; i++) {
        if (b_iter == state.rbegin())
            state.push_back(Pot(false, b_iter->id + 1));
        b_iter--;
    }
}

const std::pair<std::vector<Day12::Pot>, std::vector<Day12::Mutation>> Day12::parse_input() const {
    const auto s = input_data.cbegin();

    std::vector<Pot> state;
    pad_front(state, { Pot(true, 0) });
    long id = 0;
    for (auto iter = s + input_data.find(':') + 2; iter != s + input_data.find('\n'); iter++)
        state.push_back(Pot(*iter == '#', id++));
    pad_end(state);

    const auto mutations =
        std::string_view(s + input_data.find("\n\n") + 2, input_data.end()) |
        std::views::split('\n') |
        std::views::transform([](const auto&& line) { return Mutation(std::string_view(line)); }) |
        std::ranges::to<std::vector>();

    return std::pair(state, mutations);
}

ResultType Day12::solvePart1() {
    const auto input = parse_input();
    auto state = std::get<0>(input);
    const auto mutations = std::get<1>(input);

    std::vector<Pot> next_state;
    for (auto g = 0; g < 20; g++) {
        for (auto c = 2UL; c < state.size() - 2; c++) {
            bool spread = false;
            for (auto mut = mutations.begin(); mut != mutations.end(); mut++) {
                if (mut->matches(state, c)) {
                    spread = mut->spread;
                    break;
                }
            }
            next_state.push_back(Pot(spread, state[c].id));
        }

        state.clear();
        pad_front(state, next_state);
        state.insert(state.end(), next_state.begin(), next_state.end());
        pad_end(state);
        next_state.clear();
    }

    long sum = 0;
    for (auto iter = state.begin(); iter != state.end(); iter++)
        sum += iter->plant ? iter->id : 0;
    return sum;
}

void Day12::display(const std::vector<Pot>& state) const {
    for (auto p = state.begin(); p != state.end(); p++)
        std::cout << (p->plant ? '#' : '.');
    std::cout << std::endl;
}

ResultType Day12::solvePart2() {
    const auto input = parse_input();
    auto state = std::get<0>(input);
    const auto mutations = std::get<1>(input);

    long prev_sum = 0;
    long prev_diff = 0;
    std::vector<Pot> next_state;
    for (auto g = 0L; g < 143; g++) {
        for (auto c = 2UL; c < state.size() - 2; c++) {
            bool spread = false;
            for (auto mut = mutations.begin(); mut != mutations.end(); mut++) {
                if (mut->matches(state, c)) {
                    spread = mut->spread;
                    break;
                }
            }
            next_state.push_back(Pot(spread, state[c].id));
        }

        state.clear();
        pad_front(state, next_state);
        state.insert(state.end(), next_state.begin(), next_state.end());
        pad_end(state);
        next_state.clear();

        // display(state);

        long sum = 0;
        for (auto iter = state.begin(); iter != state.end(); iter++)
            sum += iter->plant ? iter->id : 0;
        // std::cout << sum << std::endl;
        const auto diff = sum - prev_sum;
        // if (prev_diff != diff) {
        //     std::cout << g + 1 << ": " << "sum: " << sum << ", diff: " << diff << std::endl;
        // }
        prev_sum = sum;
        prev_diff = diff;
    }
    return (50000000000 - 143) * prev_diff + prev_sum;
}

} // namespace adventofcode
