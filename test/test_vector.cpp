#include "catch/include/catch.hpp"
#include "../include/vie/vector.hpp"

TEST_CASE("vector constructor", "[vector]") {
    vie::simple_vector<int> integers;

    REQUIRE(integers.size() == 0);
    REQUIRE(integers.begin() == integers.end());
}

TEST_CASE("vector size()", "[vector]") {
    vie::simple_vector<double> doubles;

    SECTION("constructed vectors are empty") {
        REQUIRE(doubles.size() == 0);
    }
    SECTION("push_back(...) increases the vector's size") {
        doubles.push_back(1.0);
        doubles.push_back(2.0);
        doubles.push_back(3.0);

        REQUIRE(doubles.size() == 3);
    }
}
