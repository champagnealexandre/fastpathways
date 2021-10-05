#include <chrono>
#include <fastpathways.h>
#include <iomanip>
#include <iostream>

auto usage() -> void
{
    std::cerr << "usage: intchain [subcommand] n\n"
        << "\n"
        << "Subcommands:\n"
        << "    <none>: compute and output the assembly index of `n`\n"
        << "    time:   compute and output the assembly index of `n` and run time.\n"
        << "    perf:   compute the assembly index of all integers from 1 to `n` (inclusive)\n"
        << "            and output the sum and run time."
        << std::endl;
}

auto main(int argc, char **argv) -> int {
    using namespace std::chrono;

    try {
        if (argc < 2) {
            usage();
            return -1;
        } else if (argc == 2) {
            auto const n = std::atoi(argv[1]);
            std::cout << thurber(n) << std::endl;
        } else if (argc == 3 && std::string(argv[1]) == "time") {
            auto const n = std::atoi(argv[2]);
            auto start = steady_clock::now();
            auto const c = thurber(n);
            auto stop = steady_clock::now();
            std::cout << c << "\n"
                      << std::setprecision(9)
                      << (1e-9 * duration_cast<nanoseconds>(stop-start).count()) << "s"
                      << std::endl;
        } else if (argc == 3 && std::string(argv[1]) == "perf") {
            auto const N = std::atoi(argv[2]);
            auto start = steady_clock::now();
            auto sum = 0;
            for (auto n = 1; n <= N; ++n) {
                sum += thurber(n);
            }
            auto stop = steady_clock::now();
            std::cout << sum << "\n"
                      << std::setprecision(9)
                      << (1e-9 * duration_cast<nanoseconds>(stop-start).count()) << "s"
                      << std::endl;
        } else {
            usage();
            return -1;
        }
    } catch (std::exception const &e) {
        std::cerr << "error: " << e.what() << std::endl;
        return -2;
    }
}
