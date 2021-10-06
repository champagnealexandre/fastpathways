#include <fastpathways.h>
#include <algorithm>

static auto nu(int64_t x) -> int64_t {
    return __builtin_popcount(x);
}

static auto lambda(int64_t x) -> int64_t {
    return std::floor(std::log2(x));
}

auto lowerbound(int64_t x) -> int64_t {
    if (x <= 327678 || nu(x) <= 16) {
        return lambda(x) + std::ceil(std::log2(nu(x)));
    } else {
        return std::ceil(std::log2(x) + std::log2(nu(x)) - 2.13);
    }
}

auto BoundingSequenceA::bounds(int64_t n, int64_t lb) -> std::vector<int64_t> {
    auto seq = std::vector<int64_t>(lb + 1);
    seq.at(lb) = n;
    for (auto i = 1; i <= lb; ++i) {
        seq.at(lb - i) = std::ceil(seq.at(lb - i + 1) / 2.0);
    }
    return seq;
}

static auto twopower(int64_t n) -> std::tuple<int64_t, bool> {
    auto t = 0;
    while (n % 2 == 0) {
        n >>= 1;
        t += 1;
    }

    auto i = 1;
    auto z = (1 << i) + 1;
    while (z < n && n % z != 0) {
        i += 1;
        z = (1 << i) + 1;
    }

    return { t, n % z == 0 };
}

auto BoundingSequenceC::bounds(int64_t n, int64_t t, int64_t lb) -> std::vector<int64_t> {
    auto seq = std::vector<int64_t>(lb + 1);
    for (auto i = 0; i <= lb - t - 2; ++i) {
        seq.at(i) = std::ceil(1.0 * n / (3 * (1 << (lb - i - 2))));
    }
    for (auto i = std::max(int64_t{0}, lb - t - 1); i <= lb; ++i) {
        seq.at(i) = std::ceil(1.0 * n / (1 << (lb - i)));
    }
    return seq;
}

auto BoundingSequenceE::bounds(int64_t n, int64_t t, int64_t lb) -> std::vector<int64_t> {
    auto seq = std::vector<int64_t>(lb + 1);
    for (auto i = 0; i <= lb - t - 2; ++i) {
        seq.at(i) = std::ceil(1.0 * n / ((1 << t) * ((1 << (lb - t - i - 1)) + 1)));
    }
    for (auto i = std::max(int64_t{0}, lb - t - 1); i <= lb; ++i) {
        seq.at(i) = std::ceil(1.0 * n / (1 << (lb - i)));
    }
    return seq;
}

auto bounds(int64_t n, int64_t lb) -> std::tuple<std::vector<int64_t>, std::vector<int64_t>> {
    int64_t t;
    bool is_div_2n_1;
    std::tie(t, is_div_2n_1) = twopower(n);

    std::vector<int64_t> vertical;
    if (is_div_2n_1) {
        vertical = BoundingSequenceC::bounds(n, t, lb);
    } else {
        vertical = BoundingSequenceE::bounds(n, t, lb);
    }

    std::vector<int64_t> slant;
    if (n % 5 != 0) {
        slant = BoundingSequenceC::bounds(n, t, lb);
    } else {
        slant = BoundingSequenceA::bounds(n, lb);
    }

    return { vertical, slant };
}

static auto retain(int64_t bound, int64_t a) -> bool {
    return a >= bound;
}

static auto retain(int64_t bound, int64_t aprev, int64_t a) -> bool {
    return a + aprev >= bound;
}

static auto retain(int64_t n, int64_t lb, int64_t i, int64_t a) -> bool {
    return n != (1 << (lb - i + 1)) * a;
}

auto retain(int64_t n, int64_t lb, int64_t v, int64_t s, int64_t i, int64_t aprev, int64_t a) -> bool {
    return retain(v, a) && retain(n, lb, i, a) && retain(s, aprev, a);
}

auto stackchildren(int64_t n, std::vector<std::vector<int64_t>> &stack) -> void {
    auto const a = stack.back().back();
    auto const N = std::size(stack);
    auto segment = std::vector<int64_t>{};
    segment.reserve(N);
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = i; j < N; ++j) {
            auto const anext = stack.at(i).back() + stack.at(j).back();
            if (a < anext && anext <= n) {
                segment.push_back(anext);
            }
        }
    }
    std::sort(std::begin(segment), std::end(segment));
    auto const last = std::unique(std::begin(segment), std::end(segment));
    segment.erase(last, std::end(segment));
    stack.push_back(segment);
}

auto backup(int &i, std::vector<std::vector<int64_t>> &stack) -> bool {
    while (std::size(stack) > 2) {
        stack.back().pop_back();
        if (stack.back().empty()) {
            stack.pop_back();
            i -= 1;
        } else {
            break;
        }
    }
    return std::size(stack) <= 2;
}

auto thurber(int64_t n) -> int64_t {
    if (n < 1) {
        throw std::domain_error{"no chains defined for integers less than 1"};
    } else if (n == 1) {
        return 0;
    } else if (n == 2) {
        return 1;
    }

    auto stack = std::vector<std::vector<int64_t>>{ {1}, {2} };
    auto lb = lowerbound(n);

    auto loop = 1;

    std::vector<int64_t> vertical, slant;
    while (true) {
        auto i = 1;
        std::tie(vertical, slant) = bounds(n, lb);
        while (true) {
            if (i < lb) {
                auto const aprev = stack.at(i-1).back();
                auto a = stack.at(i).back();
                if (retain(n, lb, vertical.at(i), slant.at(i + 1), i, aprev, a)) {
                    stackchildren(n, stack);
                    i += 1;
                    a = stack.at(i).back();
                    if (a == n) {
                        return i;
                    }
                } else if (backup(i, stack)) {
                    loop += 1;
                    break;
                }
            } else if (backup(i, stack)) {
                loop += 1;
                break;
            }
            loop += 1;
        }
        lb += 1;
    }
}
