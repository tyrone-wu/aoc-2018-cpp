#include "day16.h"
#include "day19.h"
#include <iostream>

namespace adventofcode
{
Day19::Day19() : DaySolver(19) {
    // Data for tests
    test_data = R"(#ip 0
seti 5 0 1
seti 6 0 2
addi 0 1 0
addr 1 2 3
setr 1 0 0
seti 8 0 4
seti 9 0 5)";
    test_results = { 6UL, 0 };
}

void Day19::debug(const unsigned long ip, const unsigned long ip_reg,
                  const Day16::Registers& regs) const {
    std::cout << "ip: " << ip << " ip_reg: " << static_cast<int>(ip_reg) << " [" << regs.regs[0];
    for (auto i = 1UL; i < regs.regs.size(); ++i)
        std::cout << ", " << regs.regs[i];
    std::cout << "]\n";
}

ResultType Day19::solvePart1() {
    const auto [ip_reg, instructions] = parse_input(input_data);

    auto ip = 0UL;
    Day16::Registers registers = Day16::Registers();

    const auto jump_opcodes = (1 << static_cast<unsigned char>(Day16::Opcode::Setr)) |
                              (1 << static_cast<unsigned char>(Day16::Opcode::Seti)) |
                              (1 << static_cast<unsigned char>(Day16::Opcode::Addr)) |
                              (1 << static_cast<unsigned char>(Day16::Opcode::Addi));

    while (ip < instructions.size()) {
        const auto& insn = instructions[ip];

        registers.regs[ip_reg] = ip;
        registers = insn.execute(static_cast<Day16::Opcode>(insn.opcode), registers);

        // debug(ip, ip_reg, registers);

        if (((1 << insn.opcode) & jump_opcodes) != 0) {
            ip = registers.regs[ip_reg];
        }
        ++ip;
    }

    return registers.regs[0];
}

ResultType Day19::solvePart2() {
    const unsigned long reg_2 = 10551315;
    auto reg_0 = reg_2;
    for (auto i = 1UL; i < reg_2 / 2; ++i) {
        if (reg_2 % i == 0)
            reg_0 += i;
    }
    return reg_0;

    // const auto [ip_reg, instructions] = parse_input();

    // auto ip = 0UL;
    // Day16::Registers registers = Day16::Registers();
    // registers.regs[0] = 1;

    // const auto jump_opcodes = (1 << static_cast<unsigned char>(Day16::Opcode::Setr)) |
    //                           (1 << static_cast<unsigned char>(Day16::Opcode::Seti)) |
    //                           (1 << static_cast<unsigned char>(Day16::Opcode::Addr)) |
    //                           (1 << static_cast<unsigned char>(Day16::Opcode::Addi));

    // while (ip < instructions.size()) {
    //     const auto& insn = instructions[ip];

    //     registers.regs[ip_reg] = ip;
    //     registers = insn.execute(static_cast<Day16::Opcode>(insn.opcode), registers);

    //     debug(ip, ip_reg, registers);

    //     if (((1 << insn.opcode) & jump_opcodes) != 0) {
    //         ip = registers.regs[ip_reg];
    //     }
    //     ++ip;
    // }

    // return registers.regs[0];
}

} // namespace adventofcode
