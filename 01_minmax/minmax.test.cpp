#include <algorithm>
#include <vector>
#include <iostream>
#include <type_traits>

#include <gtest/gtest.h>
#include <autocheck/sequence.hpp>
#include <autocheck/check.hpp>
#include <autocheck/generator.hpp>

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

struct prop_maximum_should_return_a_value_from_the_list_t
{
	template <typename SinglePassRange>
	bool operator()(const SinglePassRange& xs)
	{
		const auto m = minmax::maximum(xs);
		const auto is_present = std::find(
				std::begin(xs),
				std::end(xs),
				m);
		return is_present != std::end(xs);
	}
};

TEST_F(MinMaxFixture, prop_maximum_should_return_a_value_from_the_list)
{
	const auto arbitrary = autocheck::discard_if([](const std::vector<size_t>& xs) -> bool { return xs.size() == 0; },
			autocheck::make_arbitrary<std::vector<size_t>>());

  
	autocheck::classifier<std::vector<size_t>> cls;
	cls.trivial([] (const std::vector<size_t>& xs) { return xs.size() == 1; });
	cls.collect([] (const std::vector<size_t>& xs) { return (xs.size() % 10) * 10; });
	cls.classify([] (const std::vector<size_t>& xs) { return xs.empty() || xs.size() % 2 == 0; }, "even-length");
	cls.classify([] (const std::vector<size_t>& xs) { return xs.empty() || (xs.size() % 2 != 0); }, "odd-length");
	
	autocheck::check<std::vector<size_t>>(
		prop_maximum_should_return_a_value_from_the_list_t(),
		100,
		arbitrary,
		rep,
		cls);
}

struct prop_minimum_pair_should_return_a_measure_pair_t
{
	bool operator()(const std::vector<minmax::measure_pair_t>& xs) const
	{
		using namespace minmax;
		const auto test_pair = minimum_pair(xs);
		return is_measure_pair(test_pair);
	}
};

namespace autocheck
{
template <>
class generator<minmax::measure_pair_t>
{
	public:
	using result_type = minmax::measure_pair_t;

	result_type operator()()
	{
		return this->operator()(0);
	}

	result_type operator()(const size_t size)
	{
		const auto non_zero_gen = [size]()
		{
			auto rational_gen = generator<std::size_t>();
			auto temp = rational_gen(size);
			if(temp == 0)
				return temp + 1;
			return temp;
		};
		const auto measure = non_zero_gen();
		const auto factor = non_zero_gen();
		return std::make_pair(measure*factor, measure);
	}
};

//Make sure we can print something if there is an error
std::ostream& operator<<(std::ostream& os, const minmax::measure_pair_t& p)
{
	os << "{" << p.first << ", " << p.second << "}";
	return os;
}

}

TEST_F(MinMaxFixture, prop_minimum_pair_should_return_a_measure_pair)
{
	const auto arbitrary = autocheck::discard_if([](const std::vector<minmax::measure_pair_t>& xs) -> bool { return xs.size() == 0; },
			autocheck::make_arbitrary<std::vector<minmax::measure_pair_t>>());
  
	autocheck::check<std::vector<minmax::measure_pair_t>>(
		prop_minimum_pair_should_return_a_measure_pair_t(),
		100,
		arbitrary,
		rep);
}
