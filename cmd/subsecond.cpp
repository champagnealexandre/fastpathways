#include <chrono>
#include <fastpathways.h>
#include <iomanip>
#include <iostream>

auto time(int64_t n, int N) -> double {
    using namespace std::chrono;

    auto const start = steady_clock::now();
    auto sum = 0;
    for (auto i = 0; i < N; ++i) {
        sum += thurber(n);
    }
    auto const stop = steady_clock::now();
    auto const elapsed = 1e-9 * duration_cast<nanoseconds>(stop-start).count();
    return elapsed / N;
}

auto main(int argc, char **argv) -> int {
    try {
        auto const N = 10;
        auto n = 1;
        auto mean = 0.0;
        while ((mean = time(n, N)) < 1.0) n++;
        std::cout << n << "\n" << std::setprecision(9) << mean << std::endl;
    } catch (std::exception const &e) {
        std::cerr << "error: " << e.what() << std::endl;
        return -2;
    }
}
