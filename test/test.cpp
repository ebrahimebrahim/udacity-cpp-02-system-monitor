#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch_amalgamated.hpp"

#include "util.h"

TEST_CASE( "jej test" ) {
    REQUIRE( jej() == 142 );
}

TEST_CASE( "jej test meant to fail" ) {
    REQUIRE( jej() == 143 );
}