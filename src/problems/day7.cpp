#include "day7.h"
#include <algorithm>
#include <climits>
#include <functional>
#include <queue>
#include <ranges>
#include <unordered_set>
#include <vector>

namespace adventofcode
{
Day7::Day7() : DaySolver(7) {
    // Data for tests
    test_data = R"(Step C must be finished before step A can begin.
Step C must be finished before step F can begin.
Step A must be finished before step B can begin.
Step A must be finished before step D can begin.
Step B must be finished before step E can begin.
Step D must be finished before step E can begin.
Step F must be finished before step E can begin.)";
    test_results = { "CABDFE", 15 };
}

const std::tuple<std::unordered_map<char, std::vector<char>>,
                 std::unordered_map<char, unsigned int>, std::vector<char>>
Day7::parse_input() const {
    std::unordered_map<char, std::vector<char>> steps;
    std::unordered_map<char, unsigned int> depends;
    auto lines = input_data | std::views::split('\n');
    std::ranges::for_each(lines, [&steps, &depends](const auto& line) {
        const char from = line[5], to = line[36];
        steps[from].push_back(to);
        depends[to]++;
    });

    std::vector<char> start;
    for (const auto& [s, e] : steps) {
        static_cast<void>(e);
        if (depends.find(s) == depends.end())
            start.push_back(s);
    }

    return std::tuple(steps, depends, start);
}

ResultType Day7::solvePart1() {
    const auto input = parse_input();
    const auto steps = std::get<0>(input);
    auto depends = std::get<1>(input);
    const auto start = std::get<2>(input);

    std::string order;
    std::priority_queue<char, std::vector<char>, std::greater<char>> ready(start.begin(),
                                                                           start.end());
    std::unordered_set<char> seen;
    while (!ready.empty()) {
        const char from = ready.top();
        ready.pop();
        order.push_back(from);

        const auto next = steps.find(from);
        if (next == steps.end())
            continue;
        for (const auto& to : next->second) {
            if (seen.contains(to))
                continue;

            if (--depends[to] == 0) {
                ready.push(to);
                seen.insert(to);
            }
        }
    }
    return order;
}

ResultType Day7::solvePart2() {
    const auto input = parse_input();
    const auto steps = std::get<0>(input);
    auto depends = std::get<1>(input);
    const auto start = std::get<2>(input);

    const unsigned int max_workers = 5;
    const unsigned int step_duration = 60;
    int time = 0;
    std::vector<std::pair<char, unsigned int>> workers;

    std::string order;
    std::priority_queue<char, std::vector<char>, std::greater<char>> ready(start.begin(),
                                                                           start.end());
    std::unordered_set<char> seen;
    while (!workers.empty() || !ready.empty()) {
        while (workers.size() < max_workers && !ready.empty()) {
            const char from = ready.top();
            ready.pop();
            workers.push_back(
                std::pair(from, step_duration + static_cast<unsigned int>(from) - 'A' + 1));
        }

        for (auto&& w : workers) {
            w.second--;
        }

        if (!workers.empty()) {
            std::vector<char> finished;
            for (auto iter = workers.begin(); iter != workers.end();) {
                const char s = std::get<0>(*iter);
                const bool drop = std::get<1>(*iter) == 0;
                if (drop) {
                    finished.push_back(s);

                    const auto next = steps.find(s);
                    if (next != steps.end()) {
                        for (const auto& to : next->second) {
                            if (seen.contains(to))
                                continue;

                            if (--depends[to] == 0) {
                                ready.push(to);
                                seen.insert(to);
                            }
                        }
                    }

                    workers.erase(iter);
                } else {
                    iter++;
                }
            }

            std::sort(finished.begin(), finished.end());
            order.insert(order.end(), finished.begin(), finished.end());
        }
        time++;
    }

    // std::cout << "=== " << order << std::endl;
    return time;
}

} // namespace adventofcode
