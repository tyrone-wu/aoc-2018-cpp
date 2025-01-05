#pragma once

#include "../DaySolver.h"
#include <charconv>
#include <tuple>
#include <variant>

namespace adventofcode
{
class Day4 final : public DaySolver {
public:
    Day4();
    ~Day4() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    struct Log {
        unsigned int year, month, day, hour, minute;
        std::variant<unsigned int, bool> event;

        explicit Log(const std::string_view& input) {
            const auto s = input.begin();
            std::from_chars(s + 1, s + input.find('-'), year);
            std::from_chars(s + input.find('-') + 1, s + input.find_last_of('-'), month);
            std::from_chars(s + input.find_last_of('-') + 1, s + input.find(' '), day);
            std::from_chars(s + input.find(' ') + 1, s + input.find(':'), hour);
            std::from_chars(s + input.find(':') + 1, s + input.find(']'), minute);
            if (std::string_view(s + input.find(']') + 2, input.end()) == "falls asleep") {
                event = false;
            } else if (std::string_view(s + input.find(']') + 2, input.end()) == "wakes up") {
                event = true;
            } else {
                unsigned int guard;
                std::from_chars(s + input.find('#') + 1, s + input.find('b') - 1, guard);
                event = guard;
            }
        }

        static inline auto comparator() {
            return [](const Log& l1, const Log& l2) {
                return std::tie(l1.year, l1.month, l1.day, l1.hour, l1.minute) <
                       std::tie(l2.year, l2.month, l2.day, l2.hour, l2.minute);
            };
        }
    };

    [[nodiscard]] const std::vector<Log> parse_input() const;
};
} // namespace adventofcode
