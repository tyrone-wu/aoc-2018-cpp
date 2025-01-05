#pragma once

#include "../DaySolver.h"
#include "../utils.h"
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace adventofcode
{
class Day20 final : public DaySolver {
public:
    Day20();
    ~Day20() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    static constexpr std::pair<int, int> delta(const char mv) {
        switch (mv) {
        case 'N':
            return std::pair(0, -1);
        case 'E':
            return std::pair(1, 0);
        case 'S':
            return std::pair(0, 1);
        case 'W':
            return std::pair(-1, 0);
        default:
            throw std::out_of_range("invalid move");
        }
    }

    struct Position {
        int x;
        int y;
        unsigned long steps;

        explicit Position(int x, int y, unsigned long s) : x(x), y(y), steps(s) {};

        void move(const char mv) {
            const auto [dx, dy] = delta(mv);
            x += dx;
            y += dy;
            ++steps;
        }
    };

    [[nodiscard]] const std::unordered_map<std::pair<int, int>, unsigned long, utils::IntPairHash>
    walk_regex() const;
};
} // namespace adventofcode
