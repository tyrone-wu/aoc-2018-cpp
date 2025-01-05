#pragma once

#include "../DaySolver.h"
#include <ranges>
#include <unordered_map>

namespace adventofcode
{
class Day3 final : public DaySolver {
public:
    Day3();
    ~Day3() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    struct Claim {
        int id;
        std::pair<int, int> start;
        std::pair<int, int> span;

        explicit Claim(const std::ranges::subrange<const char*>& input) {
            auto tokens = input | std::views::split(' ');

            auto iter = tokens.begin();
            std::from_chars((*iter).begin() + 1, (*iter).end(), id);
            iter++;
            iter++;

            const std::string_view start_sv((*iter).begin(), (*iter).end());
            int start_x;
            int start_y;
            std::from_chars(start_sv.begin(), start_sv.begin() + start_sv.find(','), start_x);
            std::from_chars(start_sv.begin() + start_sv.find(',') + 1, start_sv.end() - 1, start_y);
            start = std::pair(start_x, start_y);
            iter++;

            int span_x;
            int span_y;
            const std::string_view span_sv((*iter).begin(), (*iter).end());
            std::from_chars(span_sv.begin(), span_sv.begin() + span_sv.find('x'), span_x);
            std::from_chars(span_sv.begin() + span_sv.find('x') + 1, span_sv.end(), span_y);
            span = std::pair(span_x, span_y);
        }

        const std::vector<std::pair<unsigned long, unsigned long>> claim_range() const {
            std::vector<std::pair<unsigned long, unsigned long>> claim_range;
            for (auto x = get<0>(start); x < get<0>(start) + get<0>(span); x++) {
                for (auto y = get<1>(start); y < get<1>(start) + get<1>(span); y++)
                    claim_range.push_back(std::pair(x, y));
            }
            return claim_range;
        }
    };

    [[nodiscard]] const std::vector<Claim> parse_input() const;

    [[nodiscard]] const std::unordered_map<unsigned long, std::vector<int>>
    solve(const std::vector<Day3::Claim>& claims) const;
};
} // namespace adventofcode
