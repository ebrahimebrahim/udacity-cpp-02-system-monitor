#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch_amalgamated.hpp"

#include "util.h"

using vecstr = std::vector<std::string>;

TEST_CASE( "default split two words", "[split]" ) {
    REQUIRE( split("hello world") == vecstr({"hello", "world"}) );
}