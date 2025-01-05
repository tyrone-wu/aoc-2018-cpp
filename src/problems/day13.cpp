#include "day13.h"
#include <algorithm>
#include <iostream>
#include <ostream>
#include <ranges>
#include <string>
#include <unordered_set>

namespace adventofcode
{
Day13::Day13() : DaySolver(13) {
    // Data for tests
    test_data = R"(/->-\        
|   |  /----\ 
| /-+--+-\  | 
| | |  | v  | 
\-+-/  \-+--/ 
  \------/    )";
    test_data2 = R"(/>-<\   
|   |   
| /<+-\ 
| | | v 
\>+</ | 
  |   ^ 
  \<->/ )";
    test_results = { "7,3", "6,4" };
}

const std::pair<std::vector<std::vector<char>>, std::vector<Day13::Cart>>
Day13::parse_input() const {
    auto tracks = input_data | std::views::split('\n') |
                  std::views::transform(
                      [](const auto&& line) { return std::vector(line.begin(), line.end()); }) |
                  std::ranges::to<std::vector>();

    std::vector<Cart> carts;
    for (auto y = 0UL; y < tracks.size(); y++) {
        for (auto x = 0UL; x < tracks[0].size(); x++) {
            switch (tracks[y][x]) {
            case '^':
                carts.push_back(Cart(carts.size(), x, y, Direction::Up));
                tracks[y][x] = '|';
                break;
            case 'v':
                carts.push_back(Cart(carts.size(), x, y, Direction::Down));
                tracks[y][x] = '|';
                break;
            case '<':
                carts.push_back(Cart(carts.size(), x, y, Direction::Left));
                tracks[y][x] = '-';
                break;
            case '>':
                carts.push_back(Cart(carts.size(), x, y, Direction::Right));
                tracks[y][x] = '-';
                break;
            default:
            }
        }
    }

    return std::pair(tracks, carts);
}

ResultType Day13::solvePart1() {
    const auto input = parse_input();
    const auto tracks = std::get<0>(input);
    auto carts = std::get<1>(input);

    while (true) {
        std::sort(carts.begin(), carts.end(), [](const auto& c1, const auto& c2) {
            return c1.y != c2.y ? c1.y < c2.y : c1.x < c2.x;
        });

        for (auto& cart : carts) {
            cart.move(tracks);

            for (auto& other : carts) {
                if (cart.id == other.id)
                    continue;

                if (cart.x == other.x && cart.y == other.y)
                    return std::to_string(cart.x) + "," + std::to_string(cart.y);
            }
        }
    }

    return "None";
}

void Day13::display(const std::vector<std::vector<char>>& tracks,
                    const std::vector<Cart>& carts) const {
    for (auto y = 0UL; y < tracks.size(); ++y) {
        for (auto x = 0UL; x < tracks[0].size(); ++x) {
            bool has_cart = false;
            for (const auto& c : carts) {
                if (c.x == x && c.y == y) {
                    has_cart = true;
                    break;
                }
            }
            if (has_cart)
                std::cout << '#';
            else
                std::cout << tracks[y][x];
        }
        std::cout << std::endl;
    }
}

ResultType Day13::solvePart2() {
    const auto input = parse_input();
    const auto tracks = std::get<0>(input);
    auto carts = std::get<1>(input);

    // display(tracks, carts);
    // std::cout << std::endl;

    while (true) {
        std::sort(carts.begin(), carts.end(), [](const auto& c1, const auto& c2) {
            return c1.y != c2.y ? c1.y < c2.y : c1.x < c2.x;
        });

        std::unordered_set<unsigned long> crashed_ids;
        for (auto& cart : carts) {
            if (crashed_ids.contains(cart.id))
                continue;

            cart.move(tracks);
            for (auto& other : carts) {
                if (cart.id == other.id || crashed_ids.contains(other.id))
                    continue;

                if (cart.x == other.x && cart.y == other.y) {
                    crashed_ids.insert(cart.id);
                    crashed_ids.insert(other.id);
                }
            }
        }

        for (auto iter = carts.begin(); iter != carts.end();) {
            if (crashed_ids.contains(iter->id))
                iter = carts.erase(iter);
            else
                ++iter;
        }

        // display(tracks, carts);
        // std::cout << std::endl;

        if (carts.size() == 1)
            return std::to_string(carts.front().x) + "," + std::to_string(carts.front().y);
    }

    return "None";
}

} // namespace adventofcode
