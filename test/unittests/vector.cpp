#include <catch2/catch.hpp>
#include <fastpathways.h>
#include <iostream>

TEST_CASE("vector-chains", "[vector]" ) {
    SECTION("10x10") {
        std::vector<int64_t> table = {
               0, 1, 2, 2, 3, 3, 4, 3, 4, 4,
            0, 1, 2, 3, 3, 4, 4, 5, 4, 5, 5,
            1, 2, 2, 3, 3, 4, 4, 5, 4, 5, 5,
            2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5,
            2, 3, 3, 4, 3, 4, 4, 5, 4, 5, 5,
            3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
            3, 4, 4, 4, 4, 5, 4, 5, 5, 5, 5,
            4, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6,
            3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5,
            4, 5, 5, 5, 5, 5, 5, 6, 5, 5, 6,
            4, 5, 5, 5, 5, 5, 5, 6, 5, 6, 5
        };

        auto const space = vectorspace(2, 10);
        REQUIRE(std::size(table) == std::size(space));

        std::size_t i = 0;
        for (auto const &x : space) {
            REQUIRE(thurber(x) == table.at(i));
            ++i;
        }
    }

    SECTION("5x5x5") {
        std::vector<int64_t> table = {
            0, 1, 2, 2, 3, 0, 1, 2, 3, 3, 4, 1, 2, 2, 3, 3, 4, 2, 3, 3, 3, 4, 4,
            2, 3, 3, 4, 3, 4, 3, 4, 4, 4, 4, 4, 0, 1, 2, 3, 3, 4, 1, 2, 3, 4, 4,
            5, 2, 3, 3, 4, 4, 5, 3, 4, 4, 4, 5, 5, 3, 4, 4, 5, 4, 5, 4, 5, 5, 5,
            5, 5, 1, 2, 2, 3, 3, 4, 2, 3, 3, 4, 4, 5, 2, 3, 3, 4, 4, 5, 3, 4, 4,
            4, 5, 5, 3, 4, 4, 5, 4, 5, 4, 5, 5, 5, 5, 5, 2, 3, 3, 3, 4, 4, 3, 4,
            4, 4, 5, 5, 3, 4, 4, 4, 5, 5, 3, 4, 4, 4, 5, 5, 4, 5, 5, 5, 5, 6, 4,
            5, 5, 5, 6, 5, 2, 3, 3, 4, 3, 4, 3, 4, 4, 5, 4, 5, 3, 4, 4, 5, 4, 5,
            4, 5, 5, 5, 5, 6, 3, 4, 4, 5, 4, 5, 4, 5, 5, 6, 5, 5, 3, 4, 4, 4, 4,
            4, 4, 5, 5, 5, 5, 5, 4, 5, 5, 5, 5, 5, 4, 5, 5, 5, 6, 5, 4, 5, 5, 6,
            5, 5, 4, 5, 5, 5, 5, 5
        };

        auto const space = vectorspace(3, 5);
        REQUIRE(std::size(table) == std::size(space));

        std::size_t i = 0;
        for (auto const &x : space) {
            REQUIRE(thurber(x) == table.at(i));
            ++i;
        }
    }

    SECTION("simple cases") {
        for (int n = 1; n <= 100; ++n) {
            REQUIRE(thurber(std::vector<int64_t>{n, 0}) == thurber(n));
            REQUIRE(thurber(std::vector<int64_t>{0, n}) == thurber(n));
            REQUIRE(thurber(std::vector<int64_t>{n, 1}) == thurber(n) + 1);
            REQUIRE(thurber(std::vector<int64_t>{1, n}) == thurber(n) + 1);
            REQUIRE(thurber(std::vector<int64_t>{n, n}) == thurber(n) + 1);
            REQUIRE(thurber(std::vector<int64_t>{n, n, n}) == thurber(n) + 2);
        }

        REQUIRE(thurber(std::vector<int64_t>{607, 0}) == thurber(607));
    }
}
