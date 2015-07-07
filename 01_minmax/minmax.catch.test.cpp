#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "minmax.hpp"


TEST_CASE("Directed Test", "[maximum]")
{
	std::vector<size_t> datum{1,2,3,4,5};
	REQUIRE(minmax::maximum(datum) == 5);
}



