#include "day16.h"
#include <ranges>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace adventofcode
{
Day16::Day16() : DaySolver(16) {
    // Data for tests
    test_data = R"(Before: [3, 2, 1, 1]
9 2 1 2
After:  [3, 2, 2, 1])";
    test_results = { 1, 0 };
}

const std::pair<std::vector<Day16::Sample>, std::vector<Day16::Instruction>>
Day16::parse_input() const {
    const auto samples = input_data | std::views::split(std::string_view("\n\nB")) |
                         std::views::transform([](const auto& block) {
                             return Sample(std::string_view(block.begin(), block.end()));
                         }) |
                         std::ranges::to<std::vector>();

    const auto program =
        std::string_view(input_data.begin() + input_data.find(std::string_view("\n\n\n\n")) + 4) |
        std::views::split('\n') | std::views::transform([](const auto&& line) {
            return Instruction(std::string_view(line));
        }) |
        std::ranges::to<std::vector>();

    return std::pair(samples, program);
}

ResultType Day16::solvePart1() {
    const auto samples = parse_input().first;

    int ambiguous_samples = 0;
    for (const auto& sample : samples) {
        if (sample.possible_opcodes().size() >= 3)
            ++ambiguous_samples;
    }
    return ambiguous_samples;
}

ResultType Day16::solvePart2() {
    const auto [samples, program] = parse_input();

    std::unordered_set<Opcode> opcodes;
    for (auto op = Opcode::Addr; op != Opcode::__MAX_OPCODE; ++op)
        opcodes.insert(op);

    std::unordered_map<unsigned char, std::unordered_set<Opcode>> possible_opcodes;
    for (unsigned char op = 0; op < static_cast<unsigned char>(Opcode::__MAX_OPCODE); ++op)
        possible_opcodes[op] = std::unordered_set(opcodes);

    for (const auto& sample : samples) {
        const auto sample_opcodes = sample.possible_opcodes();

        const auto op = sample.instruction.opcode;
        auto& ops = possible_opcodes[op];
        for (auto opcode = ops.begin(); opcode != ops.end();) {
            if (sample_opcodes.contains(*opcode))
                ++opcode;
            else
                opcode = ops.erase(opcode);
        }
    }

    while (!opcodes_narrowed(possible_opcodes)) {
        for (const auto& opcode_entry : possible_opcodes) {
            if (opcode_entry.second.size() != 1)
                continue;

            const auto mapped_op = *(opcode_entry.second.cbegin());
            for (auto& [op, op_possible] : possible_opcodes) {
                if (opcode_entry.first == op)
                    continue;

                op_possible.erase(mapped_op);
            }
        }
    }

    // for (const auto& op : possible_opcodes) {
    //     std::cout << (int) op.first << ": ";
    //     for (const auto& p : op.second)
    //         std::cout << static_cast<int>(p) << " ";
    //     std::cout << '\n';
    // }

    Registers registers = Registers();
    for (const auto& insn : program) {
        const auto opcode = *(possible_opcodes.at(insn.opcode).cbegin());
        registers = insn.execute(opcode, registers);
    }
    return registers.regs.at(0);
}

} // namespace adventofcode
