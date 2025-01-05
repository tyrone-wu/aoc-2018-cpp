#pragma once

#include "../DaySolver.h"
#include <algorithm>
#include <climits>
#include <iostream>
#include <ranges>
#include <set>
#include <sstream>
#include <string_view>

namespace adventofcode
{
class Day10 final : public DaySolver {
public:
    Day10();
    ~Day10() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    struct Point {
        long x_p;
        long y_p;
        long x_v;
        long y_v;

        explicit Point(const std::ranges::subrange<const char*>& input) {
            const std::string_view sv(input.begin(), input.end());
            const auto s = sv.begin();

            std::stringstream(std::string(s + sv.find('<') + 1, s + sv.find(','))) >> x_p;
            std::stringstream(std::string(s + sv.find(',') + 1, s + sv.find('>'))) >> y_p;
            std::stringstream(
                std::string(s + sv.find_last_of('<') + 1, s + sv.find_last_of(','))) >>
                x_v;
            std::stringstream(
                std::string(s + sv.find_last_of(',') + 1, s + sv.find_last_of('>'))) >>
                y_v;
        }

        explicit Point(long x, long y) : x_p(x), y_p(y) {}

        inline bool operator<(const Point& other) const {
            return x_p != other.x_p ? x_p < other.x_p : y_p < other.y_p;
        }

        void step() {
            x_p += x_v;
            y_p += y_v;
        }
    };

    struct Points {
        std::vector<Point> points;

        explicit Points(const std::string_view& input) {
            auto lines = input | std::views::split('\n');
            std::ranges::for_each(lines, [&](const auto& line) { points.push_back(Point(line)); });
        }

        void step() {
            for (auto&& p : points)
                p.step();
        }

        bool print_message(const bool print, const long max_height) {
            long min_x = LONG_MAX, max_x = LONG_MIN, min_y = LONG_MAX, max_y = LONG_MIN;
            for (const auto& p : points) {
                min_x = std::min(min_x, p.x_p);
                max_x = std::max(max_x, p.x_p);
                min_y = std::min(min_y, p.y_p);
                max_y = std::max(max_y, p.y_p);
            }
            if (max_y - min_y > max_height - 1)
                return false;

            if (print) {
                const std::set<Point> points_set(points.begin(), points.end());
                for (auto y = min_y; y <= max_y; y++) {
                    for (auto x = min_x; x <= max_x; x++)
                        std::cout << (points_set.contains(Point(x, y)) ? '#' : '.');
                    std::cout << '\n';
                }
                std::cout << "===" << std::endl;
            }
            return true;
        }
    };
};
} // namespace adventofcode
