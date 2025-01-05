#pragma once

#include "../DaySolver.h"
#include <array>
#include <string_view>

namespace adventofcode
{
class Day12 final : public DaySolver {
public:
    Day12();
    ~Day12() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    struct Pot {
        bool plant;
        long id;

        explicit Pot(const bool p, const long i) : plant(p), id(i) {}
    };

    struct Mutation {
        std::array<bool, 5> match = {};
        bool spread;

        explicit Mutation(const std::string_view& input) {
            const auto s = input.cbegin();
            auto i = 0UL;
            for (auto c = s; c != s + 5; c++)
                match[i++] = *c == '#';
            spread = *(s + input.find('>') + 2) == '#';
        }

        bool matches(const std::vector<Pot>& state, const unsigned long c) const {
            for (auto i = 0UL; i < 5; i++) {
                if (match[i] != state[c - 2 + i].plant)
                    return false;
            }
            return true;
        }
    };

    [[nodiscard]] const std::pair<std::vector<Pot>, std::vector<Mutation>> parse_input() const;

    void pad_end(std::vector<Pot>& state) const;

    void pad_front(std::vector<Pot>& state, const std::vector<Pot>& next) const;

    void display(const std::vector<Pot>& state) const;
};
} // namespace adventofcode
