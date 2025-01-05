#include "day23.h"
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <ranges>
#include <string_view>

namespace adventofcode
{
Day23::Day23() : DaySolver(23) {
    // Data for tests
    test_data = R"(pos=<0,0,0>, r=4
pos=<1,0,0>, r=1
pos=<4,0,0>, r=3
pos=<0,2,0>, r=1
pos=<0,5,0>, r=3
pos=<0,0,3>, r=1
pos=<1,1,1>, r=1
pos=<1,1,2>, r=1
pos=<1,3,1>, r=1)";
    test_data2 = R"(pos=<10,12,12>, r=2
pos=<12,14,12>, r=2
pos=<16,12,12>, r=4
pos=<14,14,14>, r=6
pos=<50,50,50>, r=200
pos=<10,10,10>, r=5)";
    test_results = { 7, 36 };
}

const std::vector<Day23::Nanobot> Day23::parse_input() const {
    return input_data | std::views::split('\n') | std::views::transform([](const auto&& line) {
               return Nanobot(std::string_view(line.begin(), line.end()));
           }) |
           std::ranges::to<std::vector>();
}

long Day23::man_dist(const Coord& c1, const Coord& c2) const {
    return std::abs(c1.x - c2.x) + std::abs(c1.y - c2.y) + std::abs(c1.z - c2.z);
}

long Day23::in_range(const Coord& coord, const std::vector<Nanobot>& nanobots,
                     const std::optional<long> radius) const {
    int in_range = 0;
    for (const auto& nanobot : nanobots) {
        const auto r = radius.has_value() ? radius.value() : nanobot.radius;
        if (man_dist(coord, nanobot.coord) <= r)
            ++in_range;
    }
    return in_range;
}

ResultType Day23::solvePart1() {
    const auto nanobots = parse_input();

    unsigned long n_i;
    long largest_radius = 0;
    for (auto i = 0UL; i < nanobots.size(); ++i) {
        if (largest_radius < nanobots[i].radius) {
            largest_radius = nanobots[i].radius;
            n_i = i;
        }
    }

    return in_range(nanobots[n_i].coord, nanobots, nanobots[n_i].radius);
}

long Day23::binary_search(const std::vector<long>& comps) const {
    long left = *std::min_element(comps.begin(), comps.end());
    long right = *std::max_element(comps.begin(), comps.end());
    // std::cout << left << ' ' << right << '\n';

    while (left < right) {
        auto mid = (left + right) / 2;

        unsigned char nanobots_left = 0, nanobots_right = 0;
        for (const auto& c : comps) {
            if (c < left || c > right)
                continue;

            if (c <= mid)
                ++nanobots_left;
            else
                ++nanobots_right;
        }

        if (nanobots_left <= nanobots_right)
            left = mid + 1;
        else
            right = mid;
    }
    return left;
}

ResultType Day23::solvePart2() {
    const auto nanobots = parse_input();

    std::vector<long> x_comps, y_comps, z_comps;
    x_comps.reserve(nanobots.size());
    y_comps.reserve(nanobots.size());
    z_comps.reserve(nanobots.size());

    for (const auto& nanobot : nanobots) {
        x_comps.push_back(nanobot.coord.x);
        y_comps.push_back(nanobot.coord.y);
        z_comps.push_back(nanobot.coord.z);
    }
    // const auto x_s = binary_search(x_comps);
    // const auto y_s = binary_search(y_comps);
    // const auto z_s = binary_search(z_comps);
    // std::cout << x_s << '\n';
    // std::cout << y_s << '\n';
    // std::cout << z_s << '\n';

    const long inc = 1;
    const long lo = 100;
    const long hi = 100;

    long sum = LONG_MAX;
    long largest_in_range = 0;
    for (auto x = 59110509 - lo; x <= 59110509 + hi; x += inc) {
        for (auto y = 46561255 - lo; y <= 46561255 + hi; y += inc) {
            for (auto z = 36801764 - lo; z <= 36801764 + hi; z += inc) {
                const auto bots_in_range = in_range(Coord(x, y, z), nanobots, std::nullopt);
                if (largest_in_range < bots_in_range ||
                    (largest_in_range == bots_in_range && (x + y + z) < sum)) {
                    largest_in_range = bots_in_range;
                    sum = x + y + z;
                    std::cout << "in range: " << largest_in_range << " x: " << x << " y: " << y
                              << " z: " << z << " man: " << sum << '\n';
                }
            }
        }
    }

    // 962
    // x: 59110509
    // y: 46561255
    // z: 36801764
    // 142473498

    return 0;
}

} // namespace adventofcode
