#include <algorithm>
#include <vector>
#include <iostream>
#include <type_traits>

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
	template<typename SinglePassRange>
		bool operator()(const SinglePassRange& xs)
		{
			auto ref_max = std::max_element(std::cbegin(xs), std::cend(xs));
			const auto test_max = minmax::maximum(xs);
			return *ref_max == test_max;
		}
};

class MinMaxFixture : public ::testing::Test
{
	public:
	autocheck::gtest_reporter rep;

	virtual void SetUp()
	{
	}

};

TEST_F(MinMaxFixture, prop_maximum_should_give_greatest_value)
{
	const auto arbitrary = autocheck::discard_if([](const std::vector<size_t>& xs) -> bool { return xs.size() == 0; },
			autocheck::make_arbitrary<std::vector<size_t>>());

	autocheck::check<std::vector<size_t>>(
			prop_max_element_t(),
			100,
			arbitrary,
			rep);
}

struct prop_maximum_should_give_rational_number_t
{
	template<typename SinglePassRange>
		bool operator()(const SinglePassRange& xs)
		{
			static_assert(std::is_integral<decltype(minmax::maximum(xs))>::value, "Maximum only works on rational numbers.");
			return std::is_integral<decltype(minmax::maximum(xs))>::value;
		}
};

TEST_F(MinMaxFixture, prop_maximum_should_give_rational_number)
{
	const auto arbitrary = autocheck::discard_if([](const std::vector<size_t>& xs) -> bool { return xs.size() == 0; },
			autocheck::make_arbitrary<std::vector<size_t>>());

	//This property in a static language can be checked statically.
	autocheck::check<std::vector<size_t>>(
			prop_maximum_should_give_rational_number_t(),
			100,
			arbitrary,
			rep);
}
