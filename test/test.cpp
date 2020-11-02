#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch_amalgamated.hpp"

#include "util.h"



// Tests for string splitting

using vecstr = std::vector<std::string>;

TEST_CASE( "split one word", "[split]" ) {
    REQUIRE( split("hello"," ") == vecstr{"hello"} );
}

TEST_CASE( "split basic", "[split]" ) {
    REQUIRE( split("hello world"," ") == vecstr{"hello", "world"} );
}

TEST_CASE( "split initial tokens", "[split]" ) {
    REQUIRE( split("  hello world"," ") == vecstr{"", "", "hello", "world"} );
}

TEST_CASE( "split final tokens", "[split]" ) {
    REQUIRE( split("hello world  "," ") == vecstr{"hello", "world", "", ""} );
}

TEST_CASE( "split empty string", "[split]" ) {
    REQUIRE( split("", " ") == vecstr{""} );
}

TEST_CASE( "split string with only token", "[split]" ) {
    REQUIRE( split("t","t") == vecstr{"",""} );
}

TEST_CASE( "split string with only long token", "[split]" ) {
    REQUIRE( split("token","token") == vecstr{"",""} );
}

TEST_CASE( "split string with long token", "[split]" ) {
    REQUIRE( split("hellotokenworld","token") == vecstr{"hello","world"} );
}

TEST_CASE( "split many words with long token", "[split]" ) {
    REQUIRE( split("hellotokenworldtokenit'stokenatokengreattokenday","token") == vecstr{"hello","world","it's","a","great","day"} );
}

TEST_CASE( "split many words", "[split]" ) {
    REQUIRE( split("hello world it's just another day"," ") == vecstr{"hello", "world", "it's", "just", "another", "day"} );
}

TEST_CASE( "split_whitespace empty string", "[split]" ) {
    REQUIRE( split_whitespace("").empty() );
}

TEST_CASE( "split_whitespace only whitespace", "[split]" ) {
    REQUIRE( split_whitespace("  \n   \t \n \t  ").empty() );
}

TEST_CASE( "split_whitespace many words", "[split]" ) {
    REQUIRE( split_whitespace("hello world it's just another day") == vecstr{"hello", "world", "it's", "just", "another", "day"} );
}

TEST_CASE( "split_whitespace many words lots of space", "[split]" ) {
    REQUIRE( split_whitespace("hello     world it's \njust\tanother  \n day") == vecstr{"hello", "world", "it's", "just", "another", "day"} );
}

TEST_CASE( "split_whitespace initial whitespace", "[split]" ) {
    REQUIRE( split_whitespace("   hello  world") == vecstr{"hello", "world"} );
}

TEST_CASE( "split_whitespace final whitespace", "[split]" ) {
    REQUIRE( split_whitespace("hello  world      ") == vecstr{"hello", "world"} );
}


// Tests for string stripping

TEST_CASE( "basic strip", "[strip]" ) {
    REQUIRE( strip(" hello ") == "hello" );
}

TEST_CASE( "basic strip 2", "[strip]" ) {
    REQUIRE( strip("   hello  ") == "hello" );
}

TEST_CASE( "leading only strip", "[strip]" ) {
    REQUIRE( strip("    hello") == "hello" );
}

TEST_CASE( "trailing only strip", "[strip]" ) {
    REQUIRE( strip("hello    ") == "hello" );
}

TEST_CASE( "complicated strip", "[strip]" ) {
    REQUIRE( strip("((hello)!!!)", {'!', '(', ')'}) == "hello" );
}