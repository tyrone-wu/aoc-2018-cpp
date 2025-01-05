#pragma once

#include "../DaySolver.h"
#include "day16.h"
#include <string_view>

namespace adventofcode
{
class Day19 final : public DaySolver {
public:
    Day19();
    ~Day19() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

    [[nodiscard]] static const std::pair<unsigned long, std::vector<Day16::Instruction>>
    parse_input(const std::string_view& input) {
        const auto s = input.cbegin();

        unsigned long ip;
        std::from_chars(s + input.find(' ') + 1, s + input.find('\n'), ip);

        const auto instructions = std::string_view(s + input.find('\n') + 1, input.end()) |
                                  std::views::split('\n') |
                                  std::views::transform([](const auto&& line) {
                                      return Day16::Instruction(std::string_view(line));
                                  }) |
                                  std::ranges::to<std::vector>();

        return std::pair(ip, instructions);
    }

private:
    void debug(const unsigned long ip, const unsigned long ip_reg,
               const Day16::Registers& regs) const;
};
} // namespace adventofcode
