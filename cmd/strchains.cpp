#include <chrono>
#include <fastpathways.h>
#include <iomanip>
#include <iostream>

auto usage() -> void
{
    std::cerr << "usage: strchains <stringn\n" << std::endl;
}

auto main(int argc, char **argv) -> int {
    using namespace std::chrono;

    try {
        if (argc < 2) {
            usage();
            return -1;
        } else if (argc == 2) {
            std::cout << thurber(std::string(argv[1])) << std::endl;
        }
    } catch (std::exception const &e) {
        std::cerr << "error: " << e.what() << std::endl;
        return -2;
    }
}
