#include <chrono>
#include <fastpathways.h>
#include <iomanip>
#include <iostream>

auto usage() -> void
{
    std::cerr << "usage: vecchain <integers> n" << std::endl;
}

auto main(int argc, char **argv) -> int {
    using namespace std::chrono;

    try {
        if (argc < 2) {
            usage();
            return -1;
        }
        std::vector<int64_t> vec(argc-1);
        for (std::size_t i = 0; i < vec.size(); ++i) {
            vec.at(i) = std::atoi(argv[i + 1]);
        }
        std::cout << thurber(vec) << std::endl;
    } catch (std::exception const &e) {
        std::cerr << "error: " << e.what() << std::endl;
        return -2;
    }
}
