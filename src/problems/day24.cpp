#include "day24.h"
#include <algorithm>
#include <optional>
#include <stdexcept>
#include <string_view>

namespace adventofcode
{
Day24::Day24() : DaySolver(24) {
    // Data for tests
    test_data = R"(Immune System:
17 units each with 5390 hit points (weak to radiation, bludgeoning) with an attack that does 4507 fire damage at initiative 2
989 units each with 1274 hit points (immune to fire; weak to bludgeoning, slashing) with an attack that does 25 slashing damage at initiative 3

Infection:
801 units each with 4706 hit points (weak to radiation) with an attack that does 116 bludgeoning damage at initiative 1
4485 units each with 2961 hit points (immune to radiation; weak to fire, cold) with an attack that does 12 slashing damage at initiative 4)";
    test_results = { 5216L, 51L };
}

const std::vector<Day24::Group> Day24::parse_input() const {
    const auto s = input_data.cbegin();
    Army immune_system(std::string_view(s, s + input_data.find("\n\n")));
    Army infection(std::string_view(s + input_data.find("\n\n") + 2, input_data.end()));

    std::vector<Group> groups;
    groups.reserve(immune_system.groups.size() + infection.groups.size());
    for (auto& group : immune_system.groups) {
        group.id = static_cast<char>(groups.size());
        groups.push_back(group);
    }
    for (auto& group : infection.groups) {
        group.id = static_cast<char>(groups.size());
        groups.push_back(group);
    }
    return groups;
}

bool Day24::combat_finished(const std::vector<Group>& groups) const {
    unsigned char immune_systems = 0, infections = 0;
    for (const auto& group : groups) {
        if (group.immune_system)
            ++immune_systems;
        else
            ++infections;
    }
    return immune_systems == 0 || infections == 0;
}

long Day24::units_remaining(const std::vector<Group>& groups) const {
    long units = 0;
    for (const auto& group : groups)
        units += group.units;
    return units;
}

ResultType Day24::solvePart1() {
    // auto [immune_system, infection] = parse_input();
    auto groups = parse_input();

    while (!combat_finished(groups)) {
        // target selection
        std::sort(groups.begin(), groups.end(), [](const auto& g1, const auto& g2) {
            const auto g1_ep = g1.effective_power();
            const auto g2_ep = g2.effective_power();
            return g1_ep != g2_ep ? g1_ep > g2_ep : g1.initiative > g2.initiative;
        });

        for (auto& group : groups) {
            if (group.attacking.has_value() || group.units <= 0)
                throw std::runtime_error("didn't clear properly");

            char target = -1;
            long target_damage = 0;
            long target_effective_power = 0;
            unsigned char target_initiative = 0;
            for (const auto& enemy : groups) {
                auto damage = group.damage(enemy);
                if (enemy.attacked_by.has_value() || damage == 0)
                    continue;

                auto enemy_effective_power = enemy.effective_power();
                auto enemy_initiative = enemy.initiative;
                if (target_damage < damage) {
                    target = enemy.id;
                    target_damage = damage;
                    target_effective_power = enemy_effective_power;
                    target_initiative = enemy_initiative;
                } else if (target_damage == damage) {
                    if (target_effective_power < enemy_effective_power) {
                        target = enemy.id;
                        target_effective_power = enemy_effective_power;
                        target_initiative = enemy_initiative;
                    } else if (target_effective_power == enemy_effective_power) {
                        if (target_initiative < enemy_initiative) {
                            target = enemy.id;
                            target_initiative = enemy_initiative;
                        }
                    }
                }
            }

            if (target == -1)
                continue;

            group.attacking = target;
            for (auto& enemy : groups) {
                if (enemy.id == target) {
                    enemy.attacked_by = group.id;
                    break;
                }
            }
        }

        // attack
        std::sort(groups.begin(), groups.end(),
                  [](const auto& g1, const auto& g2) { return g1.initiative > g2.initiative; });

        for (const auto& group : groups) {
            if (group.units <= 0 || !group.attacking.has_value())
                continue;

            const auto target = group.attacking.value();
            for (auto& enemy : groups) {
                if (enemy.id == target) {
                    const auto damage = group.damage(enemy);
                    const auto units_killed = damage / enemy.hit_points;
                    enemy.units -= units_killed;
                    break;
                }
            }
        }

        // cleanup
        for (auto iter = groups.begin(); iter != groups.end();) {
            iter->attacking = std::nullopt;
            iter->attacked_by = std::nullopt;
            if (iter->units <= 0) {
                iter = groups.erase(iter);
            } else {
                ++iter;
            }
        }
    }

    return units_remaining(groups);
}

ResultType Day24::solvePart2() {
    const auto groups_original = parse_input();

    for (auto boost = 87L;; ++boost) {
        // std::cout << boost << '\n';
        auto groups = groups_original;
        for (auto& group : groups)
            group.boost(boost);

        bool stalemate = false;
        while (!combat_finished(groups)) {
            // target selection
            std::sort(groups.begin(), groups.end(), [](const auto& g1, const auto& g2) {
                const auto g1_ep = g1.effective_power();
                const auto g2_ep = g2.effective_power();
                return g1_ep != g2_ep ? g1_ep > g2_ep : g1.initiative > g2.initiative;
            });

            for (auto& group : groups) {
                if (group.attacking.has_value() || group.units <= 0)
                    throw std::runtime_error("didn't clear properly");

                char target = -1;
                long target_damage = 0;
                long target_effective_power = 0;
                unsigned char target_initiative = 0;
                for (const auto& enemy : groups) {
                    auto damage = group.damage(enemy);
                    if (enemy.attacked_by.has_value() || damage == 0)
                        continue;

                    auto enemy_effective_power = enemy.effective_power();
                    auto enemy_initiative = enemy.initiative;
                    if (target_damage < damage) {
                        target = enemy.id;
                        target_damage = damage;
                        target_effective_power = enemy_effective_power;
                        target_initiative = enemy_initiative;
                    } else if (target_damage == damage) {
                        if (target_effective_power < enemy_effective_power) {
                            target = enemy.id;
                            target_effective_power = enemy_effective_power;
                            target_initiative = enemy_initiative;
                        } else if (target_effective_power == enemy_effective_power) {
                            if (target_initiative < enemy_initiative) {
                                target = enemy.id;
                                target_initiative = enemy_initiative;
                            }
                        }
                    }
                }

                if (target == -1)
                    continue;

                group.attacking = target;
                for (auto& enemy : groups) {
                    if (enemy.id == target) {
                        enemy.attacked_by = group.id;
                        break;
                    }
                }
            }

            auto can_attack = 0;
            for (const auto& group : groups) {
                if (group.attacking.has_value())
                    ++can_attack;
            }
            if (can_attack == 0) {
                stalemate = true;
                break;
            }

            // attack
            std::sort(groups.begin(), groups.end(),
                      [](const auto& g1, const auto& g2) { return g1.initiative > g2.initiative; });

            for (const auto& group : groups) {
                if (group.units <= 0 || !group.attacking.has_value())
                    continue;

                const auto target = group.attacking.value();
                for (auto& enemy : groups) {
                    if (enemy.id == target) {
                        const auto damage = group.damage(enemy);
                        const auto units_killed = damage / enemy.hit_points;
                        enemy.units -= units_killed;
                        break;
                    }
                }
            }

            // cleanup
            for (auto iter = groups.begin(); iter != groups.end();) {
                iter->attacking = std::nullopt;
                iter->attacked_by = std::nullopt;
                if (iter->units <= 0) {
                    iter = groups.erase(iter);
                } else {
                    ++iter;
                }
            }

            // for (const auto& group : groups)
            //     group.debug(false);
            // std::cout << "====\n";
        }
        if (stalemate)
            continue;

        unsigned char immune_systems = 0;
        for (const auto& group : groups) {
            if (group.immune_system)
                ++immune_systems;
        }
        if (immune_systems > 0)
            return units_remaining(groups);
    }

    return -1L;
}

} // namespace adventofcode
