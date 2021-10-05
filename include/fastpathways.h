#pragma once

#include <cstdint>
#include <vector>
#include <tuple>
#include <cmath>
#include <ostream>
#include <iterator>

auto lowerbound(int64_t x) -> int64_t;

struct BoundingSequence {};

struct BoundingSequenceA: BoundingSequence {
    static auto bounds(int64_t n, int64_t lb) -> std::vector<int64_t>;
};

struct BoundingSequenceC: BoundingSequence {
    static auto bounds(int64_t n, int64_t lb) -> std::vector<int64_t>;
};

auto bounds(int64_t n, int64_t lb) -> std::tuple<std::vector<int64_t>, std::vector<int64_t>>;

template <class T>
auto operator<<(std::ostream &out, std::vector<T> const &xs) -> std::ostream& {
    out << "[ ";
    std::copy(std::begin(xs), std::end(xs), std::ostream_iterator<T>(out, " "));
    return out << ']';
}

auto retain(int64_t n, int64_t lb, int64_t v, int64_t s, int64_t i, int64_t aprev, int64_t a) -> bool;

auto stackchildren(int64_t n, std::vector<std::vector<int64_t>> &stack) -> void;

auto backup(std::vector<std::vector<int64_t>> &stack) -> bool;

auto thurber(int64_t n) -> int64_t;
