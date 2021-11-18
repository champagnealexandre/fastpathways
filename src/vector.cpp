#include <algorithm>
#include <fastpathways.h>
#include <map>

auto sum(std::vector<int64_t> const &xs) -> int64_t {
    auto s = int64_t{};
    for (auto const &x : xs) {
        s += x;
    }
    return s;
}

static auto isbelow(std::vector<int64_t> const &v, std::vector<int64_t> const &w) -> bool {
    if (v == w || std::size(v) != std::size(w)) {
        return false;
    }
    for (std::size_t i = 0; i < std::size(v); ++i) {
        if (v.at(i) > w.at(i)) {
            return false;
        }
    }
    return true;
}

auto operator<(std::vector<int64_t> const &v, std::vector<int64_t> const &w) -> bool {
    if (std::size(v) < std::size(w)) {
        return true;
    }
    for (size_t i = 0; i < std::size(v); ++i) {
        if (v.at(i) < w.at(i)) {
            return true;
        } else if (v.at(i) > w.at(i)) {
            return false;
        }
    }
    return false;
}

static auto isbasic(std::vector<int64_t> const &xs) -> bool {
    auto s = int64_t{};
    for (auto const &x : xs) {
        s += x;
        if (x < 0) {
            return false;
        }
    }
    return s == 1;
}

static auto operator+(std::vector<int64_t> const &v, std::vector<int64_t> const &w) -> std::vector<int64_t> {
    if (std::size(v) != std::size(w)) {
        throw std::invalid_argument{"vectors must have the same size"};
    }
    auto x = std::vector<int64_t>(std::size(v));
    for (std::size_t i = 0; i < std::size(v); ++i) {
        x.at(i) = v.at(i) + w.at(i);
    }
    return x;
}

static auto basic(std::vector<int64_t> const &xs) -> std::vector<std::vector<int64_t>> {
    auto const N = std::size(xs);
    auto bs = std::vector<std::vector<int64_t>>{};
    bs.reserve(N);
    for (std::size_t i = 0; i < N; ++i) {
        auto b = std::vector<int64_t>(N);
        b.at(i) = 1;
        bs.push_back(b);
    }
    return bs;
}

auto lowerbound(std::vector<int64_t> const &xs) -> int64_t {
    return std::get<0>(thurber(sum(xs)));
}

auto divides(int64_t n, int64_t m) -> int64_t {
    if (m == 0) {
        return n;
    } else {
        return static_cast<int64_t>(std::floor(n / m));
    }
}

auto divides(std::vector<int64_t> const &n, std::vector<int64_t> const &m) -> int64_t {
    int64_t d = divides(n.at(0), m.at(0));
    for (std::size_t i = 1; i < std::min(std::size(n), std::size(m)); ++i) {
        d = std::min(d, divides(n.at(i), m.at(i)));
    }
    return d;
}

auto ord(std::vector<int64_t> const &x, std::vector<int64_t> const &v, std::vector<int64_t> const &w) -> bool {
    auto const a = sum(v);
    auto const b = sum(w);
    return a < b || (a == b && divides(x, v) < divides(x, w));
}

static auto ord(std::vector<int64_t> const &x) -> std::function<int(std::vector<int64_t> const&, std::vector<int64_t> const&)> {
    return [&x](std::vector<int64_t> const &v, std::vector<int64_t> const &w) {
        return ord(x, v, w);
    };
}

static auto stackchildren(std::vector<int64_t> const &x, std::vector<std::vector<std::vector<int64_t>>> &stack) -> void {
    auto const a = stack.back().back();
    auto const N = std::size(stack);
    auto segment = std::vector<std::vector<int64_t>>{};
    segment.reserve(N);
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = i; j < N; ++j) {
            auto const anext = stack.at(i).back() + stack.at(j).back();
            if (!isbelow(anext, a) && (anext != a) && (isbelow(anext, x) || anext == x)) {
                segment.push_back(anext);
            }
        }
    }
    std::stable_sort(std::begin(segment), std::end(segment), ord(x));
    auto const last = std::unique(std::begin(segment), std::end(segment));
    segment.erase(last, std::end(segment));
    stack.push_back(segment);
}

auto reduce(std::vector<int64_t> x) -> std::tuple<std::vector<int64_t>, int64_t> {
    auto counter = std::map<int64_t,int64_t>{};
    for (auto const &e : x) {
        if (counter.count(e)) {
            counter[e] += 1;
        } else {
            counter[e] = 1;
        }
    }
    auto t = 0;
    std::vector<int64_t> keys;
    for (auto const &[k,v] : counter) {
        if (k > 1) {
            t += v - 1;
            keys.push_back(k);
        } else if (k == 1) {
            t += v;
        }
    }
    std::sort(std::rbegin(keys), std::rend(keys));
    if (std::size(keys) == 0) {
        t -= 1;
    }
    return { std::move(keys), t };
}

auto thurber(std::vector<int64_t> x) -> int64_t {
    if (std::size(x) == 0) {
        return 0;
    } else if (sum(x) == 0) {
        throw std::domain_error{"vector is not in the space"};
    } else if (std::size(x) == 1) {
        return std::get<0>(thurber(x.at(0)));
    } else if (isbasic(x)) {
        return 0;
    }

    auto const reduced = reduce(x);
    if (std::get<1>(reduced) != 0) {
        return thurber(std::get<0>(reduced)) + std::get<1>(reduced);
    } else {
        std::sort(std::rbegin(x), std::rend(x));
    }

    auto stack = std::vector<std::vector<std::vector<int64_t>>>{};
    stack.reserve(std::size(x));
    for (auto &&e: basic(x)) {
        stack.push_back({e});
    }
    auto lb = lowerbound(x);
    auto const N = std::size(stack);

    auto loop = 1;

    std::vector<int64_t> vertical, slant;
    while (true) {
        auto i = 0;
        std::tie(vertical, slant) = bounds(sum(x), lb);
        if (std::size(stack) == N) {
            stackchildren(x, stack);
            i += 1;
            for (auto const &a : stack.at(N + i - 1)) {
                if (a == x) {
                    return i;
                }
            }
        }
        while (true) {
            if (i < lb) {
                auto const aprev = stack.at(N + i - 2).back();
                auto a = stack.at(N + i - 1).back();
                if (retain(sum(x), lb, vertical.at(i), 0, i, sum(aprev), sum(a))) {
                    stackchildren(x, stack);
                    i += 1;
                    a = stack.at(N + i - 1).back();
                    if (a == x) {
                        return i;
                    }
                } else if (backup(N, i, stack)) {
                    loop += 1;
                    break;
                }
            } else if (backup(N, i, stack)) {
                loop += 1;
                break;
            }
            loop += 1;
        }
        lb += 1;
    }
}

auto vectorspace(std::size_t n, int64_t max) -> std::vector<std::vector<int64_t>> {
    auto space = std::vector<std::vector<int64_t>>{};
    space.reserve(1 << n);
    auto x = std::vector<int64_t>(n);
    x.at(0) = 1;
    space.push_back(x);
    while (true) {
        std::size_t i = 0;
        while (i < std::size(x) && x.at(i) == max) {
            x.at(i++) = 0;
        }
        if (i == std::size(x)) {
            break;
        }
        x.at(i) += 1;
        space.push_back(x);
    }
    return space;
}
