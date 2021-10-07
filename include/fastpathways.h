#pragma once

#include <cstdint>
#include <vector>
#include <tuple>
#include <cmath>
#include <ostream>
#include <iterator>

template <class T>
auto operator<<(std::ostream &out, std::vector<T> const &xs) -> std::ostream& {
    out << "[ ";
    std::copy(std::begin(xs), std::end(xs), std::ostream_iterator<T>(out, " "));
    return out << ']';
}

auto isbelow(std::vector<int64_t> const &, std::vector<int64_t> const &) -> bool;
auto operator<=>(std::vector<int64_t> const &, std::vector<int64_t> const &) -> std::partial_ordering;
auto isbasic(std::vector<int64_t> const &) -> bool;
auto operator+(std::vector<int64_t> const &, std::vector<int64_t> const &) -> std::vector<int64_t>;
auto basic(std::vector<int64_t> const &x) -> std::vector<std::vector<int64_t>>;

auto lowerbound(int64_t) -> int64_t;
auto lowerbound(std::vector<int64_t> const &) -> int64_t;

struct BoundingSequence {};

struct BoundingSequenceA: BoundingSequence {
    static auto bounds(int64_t, int64_t) -> std::vector<int64_t>;
};

struct BoundingSequenceC: BoundingSequence {
    static auto bounds(int64_t, int64_t, int64_t) -> std::vector<int64_t>;
};

struct BoundingSequenceE: BoundingSequence {
    static auto bounds(int64_t, int64_t, int64_t) -> std::vector<int64_t>;
};

auto bounds(int64_t, int64_t) -> std::tuple<std::vector<int64_t>, std::vector<int64_t>>;

auto retain(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t) -> bool;

auto stackchildren(int64_t, std::vector<std::vector<int64_t>> &) -> void;
auto stackchildren(std::vector<int64_t> const &, std::vector<std::vector<std::vector<int64_t>>> &) -> void;

template <class T>
auto backup(std::size_t N, int &i, std::vector<std::vector<T>> &stack) -> bool {
    while (std::size(stack) > N) {
        stack.back().pop_back();
        if (stack.back().empty()) {
            stack.pop_back();
            i -= 1;
        } else {
            break;
        }
    }
    return std::size(stack) <= N;
}

template <class T>
auto backup(std::size_t N, std::vector<std::vector<T>> &stack) -> bool {
    while (std::size(stack) > N) {
        stack.back().pop_back();
        if (stack.back().empty()) {
            stack.pop_back();
        } else {
            break;
        }
    }
    return std::size(stack) <= N;
}

auto thurber(int64_t) -> int64_t;
auto thurber(std::vector<int64_t> const &) -> int64_t;

auto vectorspace(std::size_t n, int64_t max = 1) -> std::vector<std::vector<int64_t>>;
