#pragma once

#include <cmath>
#include <cstdint>
#include <iterator>
#include <ostream>
#include <string>
#include <tuple>
#include <vector>

template <class T>
auto operator<<(std::ostream &out, std::vector<T> const &xs) -> std::ostream& {
    out << "[ ";
    std::copy(std::begin(xs), std::end(xs), std::ostream_iterator<T>(out, " "));
    return out << ']';
}

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

auto lowerbound(std::string const &) -> int64_t;
auto lowerbound(std::vector<int64_t> const &) -> int64_t;
auto lowerbound(int64_t) -> int64_t;

auto retain(int64_t, int64_t, int64_t, int64_t, int64_t, int64_t, int64_t) -> bool;

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
auto dropsegment(std::size_t N, int &i, std::vector<std::vector<T>> &stack) -> bool {
    if (std::size(stack) > N) {
        stack.pop_back();
        i -= 1;
        return backup(N, i, stack);
    }
    return false;
}

auto ord(std::vector<int64_t> const&, std::vector<int64_t> const&, std::vector<int64_t> const&) -> bool;
auto ord(std::string const&, std::string const&, std::string const&) -> bool;

auto thurber(int64_t) -> std::tuple<int64_t, std::vector<int64_t>>;
auto thurber(std::vector<int64_t>) -> int64_t;
auto thurber(std::string) -> int64_t;

auto vectorspace(std::size_t n, int64_t max = 1) -> std::vector<std::vector<int64_t>>;
auto stringspace(std::size_t n, std::string = "ab") -> std::vector<std::string>;

auto divides(int64_t, int64_t) -> int64_t;
auto divides(std::vector<int64_t> const &, std::vector<int64_t> const&) -> int64_t;
auto divides(std::string const &, std::string const&) -> int64_t;

auto sum(std::vector<int64_t> const &) -> int64_t;
