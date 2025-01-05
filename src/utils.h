#pragma once

#include <functional>
#include <utility>

namespace adventofcode::utils
{
struct PairHash {
    template <typename T1, typename T2> std::size_t operator()(const std::pair<T1, T2>& p) const {
        return std::hash<T1>()(p.first) ^ std::hash<T2>()(p.second);
    }
};

struct UIntPairHash {
    std::size_t operator()(const std::pair<unsigned int, unsigned int>& p) const {
        return (static_cast<unsigned long>(p.first) << 32) | static_cast<unsigned long>(p.second);
    }
};

struct IntPairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return (static_cast<unsigned long>(p.first) << 32) | static_cast<unsigned long>(p.second);
    }
};
} // namespace adventofcode::utils
