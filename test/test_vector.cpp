#include "catch.hpp"
#include "vie/vector.hpp"

TEST_CASE("vector constructor", "[vector]") {
    vie::simple_vector<int> integers;

    REQUIRE(integers.capacity() == 0);
    REQUIRE(integers.size() == 0);
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

TEST_CASE("vector at()", "[vector]") {
    vie::simple_vector<std::string> strings;

    SECTION("accessing an available element") {
        const auto string = "vienna.cpp user group";
        strings.push_back(string);

        REQUIRE(strings.at(0) == string);
    }

    SECTION("accessing the vector out of range throws") {
        REQUIRE_THROWS_AS(strings.at(1), std::out_of_range);
    }
}

TEST_CASE("vector operator[]", "[vector]") {
    vie::simple_vector<short> shorts;

    SECTION("accessing an available element") {
        const auto short_max = std::numeric_limits<short>::max();
        shorts.push_back(short_max);

        REQUIRE(shorts[0] == short_max);
    }
}

TEST_CASE("vector begin() and end()", "[vector]") {
    vie::simple_vector<unsigned char> bytes;

    SECTION("begin() and end() of an empty vector are equal") {
        REQUIRE(bytes.begin() == bytes.end());
    }

    SECTION("std::distance of begin() and end() is the number of elements in the vector") {
        bytes.push_back(0x01);
        bytes.push_back(0x02);
        bytes.push_back(0x03);

        REQUIRE(std::distance(bytes.begin(), bytes.end()) == 3);
    }
}