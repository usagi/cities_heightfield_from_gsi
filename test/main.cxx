#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <catch.hpp>

constexpr auto factorial( const unsigned number ) -> unsigned
{
  return number <= 1 ? number : factorial( number - 1 ) * number;
}

TEST_CASE( "how-to-write-a-test", "[factorial]" )
{
  REQUIRE( factorial(  1 ) ==       1 );
  REQUIRE( factorial(  2 ) ==       2 );
  REQUIRE( factorial(  3 ) ==       6 );
  REQUIRE( factorial( 10 ) == 3628800 );
}
