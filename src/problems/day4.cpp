#include "day4.h"
#include <algorithm>
#include <ranges>
#include <unordered_map>
#include <variant>

namespace adventofcode
{
Day4::Day4() : DaySolver(4) {
    // Data for tests
    test_data = R"([1518-11-01 00:00] Guard #10 begins shift
[1518-11-01 00:05] falls asleep
[1518-11-01 00:25] wakes up
[1518-11-01 00:30] falls asleep
[1518-11-01 00:55] wakes up
[1518-11-01 23:58] Guard #99 begins shift
[1518-11-02 00:40] falls asleep
[1518-11-02 00:50] wakes up
[1518-11-03 00:05] Guard #10 begins shift
[1518-11-03 00:24] falls asleep
[1518-11-03 00:29] wakes up
[1518-11-04 00:02] Guard #99 begins shift
[1518-11-04 00:36] falls asleep
[1518-11-04 00:46] wakes up
[1518-11-05 00:03] Guard #99 begins shift
[1518-11-05 00:45] falls asleep
[1518-11-05 00:55] wakes up)";
    test_results = { 240UL, 4455UL };
}

const std::vector<Day4::Log> Day4::parse_input() const {
    auto lines = input_data | std::views::split('\n');
    auto logs = lines | std::views::transform([](const auto&& line) {
                    const auto line_sv = std::string_view(line.begin(), line.end());
                    return Log(line_sv);
                }) |
                std::ranges::to<std::vector>();
    std::sort(logs.begin(), logs.end(), Log::comparator());
    return logs;
}

ResultType Day4::solvePart1() {
    const auto logs = parse_input();
    bool asleep = false;
    unsigned int sleep_min = 0;
    unsigned int id = 0;
    std::unordered_map<unsigned int, std::unordered_map<unsigned int, unsigned int>> sleep_times;
    for (const auto& log : logs) {
        if (holds_alternative<unsigned int>(log.event)) {
            if (asleep) {
                for (auto m = sleep_min; m < 60; m++) {
                    if (sleep_times[id].find(m) == sleep_times[id].end())
                        sleep_times[id][m] = 0;
                    sleep_times[id][m]++;
                }
            }
            asleep = false;
            sleep_min = log.minute;
            id = get<unsigned int>(log.event);
        } else if (get<bool>(log.event)) {
            if (asleep) {
                if (sleep_times.find(id) == sleep_times.end())
                    sleep_times[id] = {};
                for (auto m = sleep_min; m < log.minute; m++) {
                    if (sleep_times[id].find(m) == sleep_times[id].end())
                        sleep_times[id][m] = 0;
                    sleep_times[id][m]++;
                }
            }
            asleep = false;
        } else {
            asleep = true;
            sleep_min = log.minute;
        }
    }
    if (asleep) {
        for (auto m = sleep_min; m < 60; m++) {
            if (sleep_times[id].find(m) == sleep_times[id].end())
                sleep_times[id][m] = 0;
            sleep_times[id][m]++;
        }
    }

    unsigned int max_sleep = 0;
    unsigned long ans = 0;
    for (const auto& [guard_id, sleep] : sleep_times) {
        unsigned int sleep_time = 0;
        unsigned int min = 0;
        unsigned int most_slept_min = 0;
        for (const auto& [m, f] : sleep) {
            sleep_time += f;
            if (most_slept_min < f) {
                most_slept_min = f;
                min = m;
            }
        }
        if (max_sleep < sleep_time) {
            max_sleep = sleep_time;
            ans = guard_id * min;
        }
    }
    return ans;
}

ResultType Day4::solvePart2() {
    const auto logs = parse_input();
    bool asleep = false;
    unsigned int sleep_min = 0;
    unsigned int id = 0;
    std::unordered_map<unsigned int, std::unordered_map<unsigned int, unsigned int>> sleep_times;
    for (const auto& log : logs) {
        if (holds_alternative<unsigned int>(log.event)) {
            if (asleep) {
                for (auto m = sleep_min; m < 60; m++) {
                    if (sleep_times[id].find(m) == sleep_times[id].end())
                        sleep_times[id][m] = 0;
                    sleep_times[id][m]++;
                }
            }
            asleep = false;
            sleep_min = log.minute;
            id = get<unsigned int>(log.event);
        } else if (get<bool>(log.event)) {
            if (asleep) {
                if (sleep_times.find(id) == sleep_times.end())
                    sleep_times[id] = {};
                for (auto m = sleep_min; m < log.minute; m++) {
                    if (sleep_times[id].find(m) == sleep_times[id].end())
                        sleep_times[id][m] = 0;
                    sleep_times[id][m]++;
                }
            }
            asleep = false;
        } else {
            asleep = true;
            sleep_min = log.minute;
        }
    }
    if (asleep) {
        for (auto m = sleep_min; m < 60; m++) {
            if (sleep_times[id].find(m) == sleep_times[id].end())
                sleep_times[id][m] = 0;
            sleep_times[id][m]++;
        }
    }

    unsigned int max_slept_min = 0;
    unsigned long ans = 0;
    for (const auto& [guard_id, sleep] : sleep_times) {
        unsigned int min = 0;
        unsigned int most_slept_min = 0;
        for (const auto& [m, f] : sleep) {
            if (most_slept_min < f) {
                most_slept_min = f;
                min = m;
            }
        }
        if (max_slept_min < most_slept_min) {
            max_slept_min = most_slept_min;
            ans = guard_id * min;
        }
    }
    return ans;
}

} // namespace adventofcode
