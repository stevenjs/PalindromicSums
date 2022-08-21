#include <atomic>

#include <catch2/catch_test_macros.hpp>

#include "../src/palindromic.h"

std::atomic<bool> interrupted {false};

TEST_CASE( "Reverse nuber", "[reverse]") {
    REQUIRE( reverse(0) == 0 );
    REQUIRE( reverse(1) == 1 );
    REQUIRE( reverse(10) == 1 );
    REQUIRE( reverse(42) == 24 );
    REQUIRE( reverse(123) == 321 );

    REQUIRE( reverse(-1) == -1 );
    REQUIRE( reverse(-10) == -1 );
    REQUIRE( reverse(-42) == -24 );
    REQUIRE( reverse(-123) == -321 );

    REQUIRE( reverse(0xbaddcafe, 16) == 0xefacddab );
}

TEST_CASE( "Palindromic number check", "[is_palindromic]") {
    REQUIRE( is_palindromic(0) );
    REQUIRE( is_palindromic(3) );
    REQUIRE( is_palindromic(22) );
    REQUIRE( is_palindromic(959) );

    REQUIRE_FALSE( is_palindromic(12) );
    REQUIRE_FALSE( is_palindromic(123) );
    REQUIRE_FALSE( is_palindromic(-959) );
}

TEST_CASE( "Count steps to palindromic sum", "[count_steps_to_palindromic_sum]") {
    REQUIRE( count_steps_to_palindromic_sum(10, 500) == 1 );
    REQUIRE( count_steps_to_palindromic_sum(57, 500) == 2 );
    REQUIRE( count_steps_to_palindromic_sum(78, 500) == 4 );
}

TEST_CASE( "Count steps to palindromic sum failure", "[count_steps_to_palindromic_sum]") {
    REQUIRE( count_steps_to_palindromic_sum(89, 20) == 0 ); // Needs 24 steps

    // Should complete current step before interruption, but fail if no solution found
    // after current step
    interrupted = true;
    REQUIRE( count_steps_to_palindromic_sum(11, 500) == 1 ); // Needs only 1 step
    REQUIRE( count_steps_to_palindromic_sum(57, 500) == 0 ); // Needs 2 steps
}
