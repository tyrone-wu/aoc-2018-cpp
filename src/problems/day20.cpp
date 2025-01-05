#include "../utils.h"
#include "day20.h"
#include <algorithm>
#include <climits>
#include <stack>
#include <unordered_map>
#include <utility>

namespace adventofcode
{
Day20::Day20() : DaySolver(20) {
    // Data for tests
    test_data = R"(^WSSEESWWWNW(S|NENNEEEENN(ESSSSW(NWSW|SSEN)|WSWWN(E|WWS(E|SS))))$)";
    test_results = { 31UL, 0 };
}

const std::unordered_map<std::pair<int, int>, unsigned long, utils::IntPairHash>
Day20::walk_regex() const {
    const std::vector<char> regex(input_data.begin() + 1, input_data.end() - 1);

    std::unordered_map<std::pair<int, int>, unsigned long, utils::IntPairHash> dists;
    auto pos = Position(0, 0, 0);
    std::stack<Position> stack;
    for (const auto& mv : regex) {
        switch (mv) {
        case 'N':
        case 'E':
        case 'S':
        case 'W': {
            pos.move(mv);
            const auto coord = std::pair(pos.x, pos.y);
            if (!dists.contains(coord))
                dists[coord] = pos.steps;
            dists[coord] = std::min(dists[coord], pos.steps);
            pos.steps = dists[coord];
            break;
        }
        case '(': {
            stack.push(pos);
            break;
        }
        case '|': {
            pos = stack.top();
            break;
        }
        case ')': {
            pos = stack.top();
            stack.pop();
            break;
        }
        default:
            throw std::runtime_error("invalid move");
        }
    }

    return dists;
}

ResultType Day20::solvePart1() {
    const auto dists = walk_regex();
    unsigned long doors = 0;
    for (const auto& [c, d] : dists)
        doors = std::max(doors, d);
    return doors;
}

ResultType Day20::solvePart2() {
    const auto dists = walk_regex();
    unsigned long doors = 0;
    for (const auto& [c, d] : dists) {
        if (d >= 1000)
            ++doors;
    }
    return doors;
}

} // namespace adventofcode
