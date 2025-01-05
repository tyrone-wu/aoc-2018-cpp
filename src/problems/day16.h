#pragma once

#include "../DaySolver.h"
#include <array>
#include <charconv>
#include <climits>
#include <ranges>
#include <stdexcept>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

namespace adventofcode
{
class Day16 final : public DaySolver {
public:
    Day16();
    ~Day16() override = default;

    ResultType solvePart1() override;
    ResultType solvePart2() override;

    enum class Opcode {
        Addr,
        Addi,
        Mulr,
        Muli,
        Banr,
        Bani,
        Borr,
        Bori,
        Setr,
        Seti,
        Gtir,
        Gtri,
        Gtrr,
        Eqir,
        Eqri,
        Eqrr,
        __MAX_OPCODE,
    };

    struct Registers {
        std::array<unsigned long, 6> regs = {};

        Registers() = default;

        explicit Registers(const std::string_view& input) {
            const auto reg_vec = std::string_view(input.begin() + input.find('[') + 1,
                                                  input.begin() + input.find(']')) |
                                 std::views::split(std::string_view(", ")) |
                                 std::views::transform([](const auto& n_s) {
                                     unsigned int n;
                                     std::from_chars(n_s.begin(), n_s.end(), n);
                                     return n;
                                 }) |
                                 std::ranges::to<std::vector>();

            regs = { reg_vec[0], reg_vec[1], reg_vec[2], reg_vec[3] };
        }

        explicit Registers(const unsigned long reg_0) { regs[0] = reg_0; }

        bool operator==(const Registers& other) const {
            for (auto i = 0UL; i < 4; ++i) {
                if (regs[i] != other.regs[i])
                    return false;
            }
            return true;
        }
    };

    struct Instruction {
        unsigned char opcode;
        unsigned long a;
        unsigned long b;
        unsigned long c;

        Instruction() = default;

        explicit Instruction(const std::string_view& input) {
            const auto vec = input | std::views::split(' ') |
                             std::views::transform([](const auto&& n_s) {
                                 unsigned long n = ULONG_MAX;
                                 std::from_chars(n_s.begin(), n_s.end(), n);
                                 return n;
                             }) |
                             std::ranges::to<std::vector>();

            if (vec.at(0) != ULONG_MAX) {
                opcode = static_cast<unsigned char>(vec.at(0));
            } else {
                const std::string_view op_sv(input.begin(), input.begin() + input.find(' '));
                Opcode op;
                if (op_sv == "addr") {
                    op = Opcode::Addr;
                } else if (op_sv == "addi") {
                    op = Opcode::Addi;
                } else if (op_sv == "mulr") {
                    op = Opcode::Mulr;
                } else if (op_sv == "muli") {
                    op = Opcode::Muli;
                } else if (op_sv == "banr") {
                    op = Opcode::Banr;
                } else if (op_sv == "bani") {
                    op = Opcode::Bani;
                } else if (op_sv == "borr") {
                    op = Opcode::Borr;
                } else if (op_sv == "bori") {
                    op = Opcode::Bori;
                } else if (op_sv == "setr") {
                    op = Opcode::Setr;
                } else if (op_sv == "seti") {
                    op = Opcode::Seti;
                } else if (op_sv == "gtir") {
                    op = Opcode::Gtir;
                } else if (op_sv == "gtri") {
                    op = Opcode::Gtri;
                } else if (op_sv == "gtrr") {
                    op = Opcode::Gtrr;
                } else if (op_sv == "eqir") {
                    op = Opcode::Eqir;
                } else if (op_sv == "eqri") {
                    op = Opcode::Eqri;
                } else if (op_sv == "eqrr") {
                    op = Opcode::Eqrr;
                } else {
                    op = Opcode::__MAX_OPCODE;
                }

                opcode = static_cast<unsigned char>(op);
            }
            a = vec.at(1);
            b = vec.at(2);
            c = vec.at(3);
        }

        const Registers execute(const Opcode op, const Registers& before) const {
            unsigned long res = 0;
            switch (op) {
            case Opcode::Addr: {
                res = before.regs.at(a) + before.regs.at(b);
                break;
            }
            case Opcode::Addi: {
                res = before.regs.at(a) + b;
                break;
            }
            case Opcode::Mulr: {
                res = before.regs.at(a) * before.regs.at(b);
                break;
            }
            case Opcode::Muli: {
                res = before.regs.at(a) * b;
                break;
            }
            case Opcode::Banr: {
                res = before.regs.at(a) & before.regs.at(b);
                break;
            }
            case Opcode::Bani: {
                res = before.regs.at(a) & b;
                break;
            }
            case Opcode::Borr: {
                res = before.regs.at(a) | before.regs.at(b);
                break;
            }
            case Opcode::Bori: {
                res = before.regs.at(a) | b;
                break;
            }
            case Opcode::Setr: {
                res = before.regs.at(a);
                break;
            }
            case Opcode::Seti: {
                res = a;
                break;
            }
            case Opcode::Gtir: {
                res = a > before.regs.at(b);
                break;
            }
            case Opcode::Gtri: {
                res = before.regs.at(a) > b;
                break;
            }
            case Opcode::Gtrr: {
                res = before.regs.at(a) > before.regs.at(b);
                break;
            }
            case Opcode::Eqir: {
                res = a == before.regs.at(b);
                break;
            }
            case Opcode::Eqri: {
                res = before.regs.at(a) == b;
                break;
            }
            case Opcode::Eqrr: {
                res = before.regs.at(a) == before.regs.at(b);
                break;
            }
            default:
                throw std::runtime_error("invalid opcode");
            }

            Registers after = before;
            after.regs[c] = res;
            return after;
        }
    };

private:
    friend Opcode& operator++(Opcode& op) {
        op = static_cast<Opcode>(static_cast<int>(op) + 1);
        return op;
    }

    struct Sample {
        Registers before;
        Instruction instruction;
        Registers after;

        explicit Sample(const std::string_view& input) {
            auto lines =
                input | std::views::split('\n') |
                std::views::transform([](const auto&& line) { return std::string_view(line); }) |
                std::ranges::to<std::vector>();

            before = Registers(lines.at(0));
            instruction = Instruction(lines.at(1));
            after = Registers(lines.at(2));
        }

        const std::unordered_set<Opcode> possible_opcodes() const {
            std::unordered_set<Opcode> possible_opcodes;
            for (auto op = Opcode::Addr; op != Opcode::__MAX_OPCODE; ++op) {
                if (instruction.execute(op, before) == after)
                    possible_opcodes.insert(op);
            }
            return possible_opcodes;
        }
    };

    [[nodiscard]] const std::pair<std::vector<Sample>, std::vector<Instruction>>
    parse_input() const;

    bool opcodes_narrowed(const std::unordered_map<unsigned char, std::unordered_set<Opcode>>&
                              possible_opcodes) const {
        for (const auto& ops : possible_opcodes) {
            if (ops.second.size() != 1)
                return false;
        }
        return true;
    }
};
} // namespace adventofcode
