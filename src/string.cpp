#include <algorithm>
#include <fastpathways.h>
#include <map>
#include <set>
#include <functional>

static auto isbelow(std::string const &v, std::string const &w) -> bool {
    if (v == w) {
        return false;
    }
    return w.find(v) != std::string::npos;
}

static auto isbasic(std::string const &x) -> bool {
    return std::size(x) == 1;
}

static auto basic(std::string const &x) -> std::vector<std::string> {
    std::set<char> chars(std::begin(x), std::end(x));
    std::vector<std::string> basic(std::size(chars));
    auto i = 0;
    for (auto const &c: chars) {
        basic[i++] = std::string(1, c);
    }
    return basic;
}

static auto formula(std::string const &x) -> std::vector<int64_t> {
    std::map<char, int64_t> counter = {};
    for (auto const &c : x) {
        if (counter.count(c)) {
            counter[c] += 1;
        } else {
            counter[c] = 1;
        }
    }
    auto formula = std::vector<int64_t>(std::size(counter));
    auto i = 0;
    for (auto const &[k,v] : counter) {
        formula[i++] = v;
    }
    return formula;
}

static auto numbasic(std::string const &x) -> std::size_t {
    return std::size(std::set<char>(std::begin(x), std::end(x)));
}

auto lowerbound(std::string const &x) -> int64_t {
    return thurber(formula(x));
}

auto divides(std::string const &x, std::string const &y) -> int64_t {
    if (std::size(y) > std::size(x)) {
        return 0;
    } else if (y == x) {
        return 1;
    }
    int64_t div = 0;
    for (std::size_t n = 0; n < std::size(x) - std::size(y); ++n) {
        int64_t count = 0;
        for (std::size_t pos = n; (pos = x.find(y, pos)) != std::string::npos; pos += std::size(y), ++count);
        div = std::max(div, count);
    }
    return div;
}

auto ord(std::string const &x, std::string const &v, std::string const &w) -> bool {
    return std::size(v) < std::size(w) || (std::size(v) == std::size(w) && divides(x, v) < divides(x, w));
}

static auto ord(std::string const &x) -> std::function<int(std::string const&, std::string const&)> {
    return [&x](std::string const &v, std::string const &w) {
        return ord(x, v, w);
    };
}

static auto stackchildren(std::string const &x, std::vector<std::vector<std::string>> &stack) -> void {
    auto const a = stack.back().back();
    auto const N = std::size(stack);
    auto segment = std::vector<std::string>{};
    segment.reserve(N);
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = i; j < N; ++j) {
            auto anext = stack.at(i).back() + stack.at(j).back();
            auto keep = true;
            for (std::size_t k = 0; k < N; ++k) {
                if (isbelow(anext, stack.at(k).back()) || (anext == a)) {
                    keep = false;
                    break;
                }
            }
            if (keep && (isbelow(anext, x) || anext == x)) {
                segment.push_back(anext);
            }
            anext = stack.at(j).back() + stack.at(i).back();
            keep = true;
            for (std::size_t k = 0; k < N; ++k) {
                if (isbelow(anext, stack.at(k).back()) || (anext == a)) {
                    keep = false;
                    break;
                }
            }
            if (keep && (isbelow(anext, x) || anext == x)) {
                segment.push_back(anext);
            }
        }
    }
    std::stable_sort(std::begin(segment), std::end(segment), ord(x));
    auto const last = std::unique(std::begin(segment), std::end(segment));
    segment.erase(last, std::end(segment));
    stack.push_back(segment);
}

auto thurber(std::string x) -> int64_t {
    if (std::size(x) == 0) {
        return 0;
    } else if (isbasic(x)) {
        return 0;
    }

    auto stack = std::vector<std::vector<std::string>>{};
    stack.reserve(numbasic(x));
    for (auto &&e: basic(x)) {
        stack.push_back({e});
    }
    auto lb = lowerbound(x);
    auto const N = std::size(stack);

    auto loop = 1;

    std::vector<int64_t> vertical, slant;
    while (true) {
        auto i = 0;
        std::tie(vertical, slant) = bounds(std::size(x), lb);
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
                if (retain(std::size(x), lb, vertical.at(i), slant.at(i+1)-1, i, std::size(aprev), std::size(a))) {
                    stackchildren(x, stack);
                    i += 1;
                    a = stack.at(N + i - 1).back();
                    if (a == x) {
                        return i;
                    }
                } else if (dropsegment(N, i, stack)) {
                    loop += 1;
                    break;
                }
            } else if (dropsegment(N, i, stack)) {
                loop += 1;
                break;
            }
            loop += 1;
        }
        lb += 1;
    }
}

auto stringspace(std::size_t n, std::string chars) -> std::vector<std::string> {
    std::vector<char> alphabet(std::begin(chars), std::end(chars));
    std::sort(std::begin(alphabet), std::end(alphabet));
    auto const last = std::unique(std::begin(alphabet), std::end(alphabet));
    alphabet.erase(last, std::end(alphabet));

    auto const max = std::size(alphabet);

    auto space = std::vector<std::string>{};
    if (max > 0) {
        space.reserve(static_cast<int>(std::pow(max, n)));
        auto x = std::vector<std::size_t>(n, 0);
        std::string s = std::string(n, alphabet.at(0));
        space.push_back(s);
        while (true) {
            std::size_t i = 0;
            while (i < std::size(x) && x.at(i) + 1 == max) {
                x.at(i) = 0;
                s.at(i) = alphabet.at(0);
                ++i;
            }
            if (i == std::size(x)) {
                break;
            }
            x.at(i) += 1;
            s.at(i) = alphabet.at(x.at(i));
            space.push_back(s);
        }
    }
    return space;
}
