#define NOMINMAX
#include <algorithm>
#include <vector>
#include <iostream>

#include <gtest/gtest.h>
#include <autocheck/sequence.hpp>
#include <autocheck/check.hpp>

#include "minmax.hpp"

TEST(minmax, maximum_directed)
{
	std::vector<size_t> datum{1,2,3,4,5};
	EXPECT_EQ(5, minmax::maximum(datum));
}

struct prop_max_element_t
{
	template<
		typename SinglePassRange>
	bool operator()(const SinglePassRange& xs)
	{
		auto ref_max = std::max_element(std::cbegin(xs), std::cend(xs));
		const auto test_max = minmax::maximum(xs);
		return *ref_max == test_max;
	}
	
};

TEST(minmax, maximum_prop)
{
	autocheck::gtest_reporter rep;

	auto arbitrary = 
		autocheck::discard_if([](const std::vector<size_t>& xs) -> bool { return xs.size() == 0; },
		autocheck::make_arbitrary<std::vector<size_t>>());
		
	autocheck::check<std::vector<size_t>>(
			prop_max_element_t(),
			100,
			arbitrary,
			rep);
}
