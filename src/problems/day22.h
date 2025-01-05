#pragma once

#include "../DaySolver.h"
#include <tuple>
#include <unordered_map>
#include <utility>

namespace adventofcode
{
class Day22 final : public DaySolver {
public:
    Day22();
    ~Day22() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

private:
    [[nodiscard]] const std::tuple<unsigned int, unsigned int, unsigned int> parse_input() const;

    enum class ErosionLevel {
        Rocky = 0,
        Wet,
        Narrow,
    };

    [[nodiscard]] unsigned long geologic_index(
        const unsigned int x, const unsigned int y, const unsigned int x_target,
        const unsigned int y_target,
        const std::vector<std::vector<std::pair<unsigned long, ErosionLevel>>>& cave) const;

    [[nodiscard]] const std::pair<unsigned long, ErosionLevel>
    erosion_level(const unsigned long depth, const unsigned long geologic_index) const;

    void debug(const std::vector<std::vector<std::pair<unsigned long, ErosionLevel>>>& cave) const;

    const std::vector<std::vector<std::pair<unsigned long, ErosionLevel>>>
    generate_cave(const unsigned int depth, const unsigned int x_target,
                  const unsigned int y_target) const;

    enum class Tool {
        Torch = 0,
        ClimbingGear,
        Neither,
    };

    struct Me {
        unsigned long x = 0;
        unsigned long y = 0;
        Tool equipped = Tool::Torch;
        unsigned long time = 0;

        Me() = default;

        unsigned long key() const { return key_hash(x, y, equipped); }

        static unsigned long key_hash(const unsigned long x, const unsigned long y, Tool tool) {
            return (static_cast<unsigned long>(tool) << 32) | (x << 16) | y;
        }

        void equip_tool(const Tool tool) {
            if (equipped == tool)
                return;

            equipped = tool;
            time += 7;
        }

        void match_tool(const ErosionLevel from, const ErosionLevel to) {
            if (from == to)
                return;

            if (from == ErosionLevel::Rocky) {
                this->equip_tool(to == ErosionLevel::Wet ? Tool::ClimbingGear : Tool::Torch);
            } else if (from == ErosionLevel::Wet) {
                this->equip_tool(to == ErosionLevel::Rocky ? Tool::ClimbingGear : Tool::Neither);
            } else {
                this->equip_tool(to == ErosionLevel::Rocky ? Tool::Torch : Tool::Neither);
            }

            // switch (cave[y][x].second) {
            // case ErosionLevel::Rocky: {
            //     clone_1.equip_tool(Tool::ClimbingGear);
            //     clone_2.equip_tool(Tool::Torch);
            //     break;
            // }
            // case ErosionLevel::Wet: {
            //     clone_1.equip_tool(Tool::ClimbingGear);
            //     clone_2.equip_tool(Tool::Neither);
            //     break;
            // }
            // case ErosionLevel::Narrow: {
            //     clone_1.equip_tool(Tool::Torch);
            //     clone_2.equip_tool(Tool::Neither);
            //     break;
            // }
            // }
            // return std::pair(clone_1, clone_2);
        }
    };

    void dfs(const std::vector<std::vector<std::pair<unsigned long, ErosionLevel>>>& cave, Me me,
             const unsigned long x_target, const unsigned long y_target,
             std::unordered_map<unsigned long, unsigned long>& cache) const;
};
} // namespace adventofcode
