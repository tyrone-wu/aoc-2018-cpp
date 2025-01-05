#include "day19.h"
#include "day21.h"
#include <climits>
#include <iostream>
#include <optional>
#include <unordered_set>

namespace adventofcode
{
Day21::Day21() : DaySolver(21) {
    // Data for tests
    test_data = R"()";
    test_results = { 0, 0 };
}

static constexpr auto jump_opcodes = (1 << static_cast<unsigned char>(Day16::Opcode::Setr)) |
                                     (1 << static_cast<unsigned char>(Day16::Opcode::Seti)) |
                                     (1 << static_cast<unsigned char>(Day16::Opcode::Addr)) |
                                     (1 << static_cast<unsigned char>(Day16::Opcode::Addi));

void Day21::debug(const Day16::Registers& regs) const {
    std::cout << " [" << regs.regs[0];
    for (auto i = 1UL; i < regs.regs.size(); ++i)
        std::cout << ", " << regs.regs[i];
    std::cout << "]\n";
}

// unsigned long Day21::run_program(Day16::Registers registers, const unsigned long ip_reg,
//                                  const std::vector<Day16::Instruction>& instructions) const {
//     auto instructions_executed = 0UL;
//     auto ip = 0UL;
//     while (ip < instructions.size()) {
//         const auto& insn = instructions[ip];
//         registers.regs[ip_reg] = ip;
//         registers = insn.execute(static_cast<Day16::Opcode>(insn.opcode), registers);
//         if (((1 << insn.opcode) & jump_opcodes) != 0) {
//             ip = registers.regs[ip_reg];
//         }
//         ++ip;
//         ++instructions_executed;

//         // if (instructions_executed >= 100000)
//         //     break;

//         if (ip == 29)
//             debug(registers);
//     }
//     return instructions_executed;
// }

const std::optional<unsigned long> Day21::solve(const bool p2) const {
    const auto [ip_reg, instructions] = Day19::parse_input(input_data);

    std::unordered_set<unsigned long> seen;

    unsigned long prev_reg_5 = 0;

    Day16::Registers registers;
    auto ip = 0UL;
    while (ip < instructions.size()) {
        const auto& insn = instructions[ip];
        registers.regs[ip_reg] = ip;
        registers = insn.execute(static_cast<Day16::Opcode>(insn.opcode), registers);
        if (((1 << insn.opcode) & jump_opcodes) != 0) {
            ip = registers.regs[ip_reg];
        }
        ++ip;

        if (ip == 28) {
            // debug(registers);
            const auto reg_5 = registers.regs[5];
            if (!p2)
                return reg_5;

            if (seen.contains(reg_5))
                return prev_reg_5;
            seen.insert(reg_5);
            prev_reg_5 = reg_5;
        }
    }

    return std::nullopt;
}

ResultType Day21::solvePart1() {
    // const auto [ip_reg, instructions] = Day19::parse_input(input_data);
    // unsigned long min_executed = ULONG_MAX;
    // for (auto reg_0 = 0UL; reg_0 < 1000000; ++reg_0) {
    //     min_executed =
    //         std::min(min_executed, run_program(Day16::Registers(reg_0), ip_reg, instructions));
    //     if (min_executed < 100000)
    //         std::cout << min_executed << '\n';
    // }

    return solve(false).value();
}

ResultType Day21::solvePart2() {
    // 101912603 µs
    return solve(true).value();
}

} // namespace adventofcode
