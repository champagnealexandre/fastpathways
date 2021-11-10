#include <chrono>
#include <fastpathways.h>
#include <iomanip>
#include <iostream>

auto usage() -> void
{
    std::cerr << "usage: intchain [subcommand] [opts] n\n"
        << "\n"
        << "Subcommands:\n"
        << "    <none>: compute and output the assembly index of `n`\n"
        << "    time:   compute and output the assembly index of `n` and run time.\n"
        << "    perf:   compute the assembly index of all integers from 1 to `n` (inclusive)\n"
        << "            and output the sum and run time."
        << "\n\n"
        << "Options:\n"
        << "    -c|--chain: display the first minimal addition chain found.\n"
        << "                (only applicable to the root command or \"time\" subcommand)\n"
        << std::endl;
}

auto const ischainflag(char const *arg) -> bool {
    auto sarg = std::string(arg);
    return sarg == "--chain" || sarg == "-c";
}

auto const istimecommand(char const *arg) -> bool {
    return std::string(arg) == "time";
}

auto main(int argc, char **argv) -> int {
    using namespace std::chrono;

    try {
        if (argc < 2) {
            usage();
            return -1;
        } else if (argc == 2 || (argc == 3 && ischainflag(argv[1]))) {
            auto const n = std::atoi(argv[argc - 1]);
            auto const c = thurber(n);
            std::cout << std::get<0>(c) << std::endl;
            if (ischainflag(argv[1])) {
                std::cout << std::get<1>(c) << std::endl;
            }
        } else if ((argc == 3 && istimecommand(argv[1])) ||
                   (argc == 4 && istimecommand(argv[1]) && ischainflag(argv[2]))) {
            auto const n = std::atoi(argv[argc - 1]);
            auto start = steady_clock::now();
            auto const c = thurber(n);
            auto stop = steady_clock::now();
            std::cout << std::get<0>(c) << "\n";

            if (ischainflag(argv[2])) {
                std::cout << std::get<1>(c) << "\n";
            }
            std::cout << std::setprecision(9)
                      << (1e-9 * duration_cast<nanoseconds>(stop-start).count()) << "s"
                      << std::endl;
        } else if (argc == 3 && std::string(argv[1]) == "perf") {
            auto const N = std::atoi(argv[2]);
            auto start = steady_clock::now();
            auto sum = 0;
            for (auto n = 1; n <= N; ++n) {
                sum += std::get<0>(thurber(n));
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
