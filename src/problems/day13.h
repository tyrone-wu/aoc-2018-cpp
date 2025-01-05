#pragma once

#include "../DaySolver.h"

namespace adventofcode
{
class Day13 final : public DaySolver {
public:
    Day13();
    ~Day13() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    enum class Direction {
        Up = 0,
        Right,
        Down,
        Left,
        __MAX_DIR,
    };

    friend Direction& operator++(Direction& dir) {
        dir = static_cast<Direction>((static_cast<int>(dir) + 1) %
                                     static_cast<int>(Direction::__MAX_DIR));
        return dir;
    }

    friend Direction& operator--(Direction& dir) {
        const auto max = static_cast<int>(Direction::__MAX_DIR);
        dir = static_cast<Direction>((static_cast<int>(dir) - 1 + max) % max);
        return dir;
    }

    enum class Turn {
        Left = 0,
        Straight,
        Right,
        __MAX_TURN,
    };

    friend Turn& operator++(Turn& turn) {
        turn = static_cast<Turn>((static_cast<int>(turn) + 1) % static_cast<int>(Turn::__MAX_TURN));
        return turn;
    }

    struct Cart {
        unsigned long id;
        unsigned long x;
        unsigned long y;
        Direction dir;
        Turn turn_intersection;

        explicit Cart(unsigned long i, unsigned long x, unsigned long y, Direction d)
            : id(i), x(x), y(y), dir(d), turn_intersection(Turn::Left) {}

        void move(const std::vector<std::vector<char>>& tracks) {
            switch (dir) {
            case Direction::Up:
                --y;
                break;
            case Direction::Down:
                ++y;
                break;
            case Direction::Left:
                --x;
                break;
            case Direction::Right:
                ++x;
                break;
            default:
            }

            const auto track = tracks[y][x];
            if (track == '/' || track == '\\') {
                switch (dir) {
                case Direction::Up:
                    dir = track == '/' ? Direction::Right : Direction::Left;
                    break;
                case Direction::Down:
                    dir = track == '/' ? Direction::Left : Direction::Right;
                    break;
                case Direction::Left:
                    dir = track == '/' ? Direction::Down : Direction::Up;
                    break;
                case Direction::Right:
                    dir = track == '/' ? Direction::Up : Direction::Down;
                    break;
                default:
                }
            } else if (track == '+') {
                if (turn_intersection == Turn::Left) {
                    --dir;
                } else if (turn_intersection == Turn::Right) {
                    ++dir;
                }
                ++turn_intersection;
            }
        }
    };

    [[nodiscard]] const std::pair<std::vector<std::vector<char>>, std::vector<Cart>>
    parse_input() const;

    void display(const std::vector<std::vector<char>>& tracks,
                 const std::vector<Cart>& carts) const;
};
} // namespace adventofcode
