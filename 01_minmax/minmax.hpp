#include <utility>

namespace minmax
{

template <typename SinglePassRange>
typename SinglePassRange::value_type maximum(const SinglePassRange& range)
{
	//First Implementation
	//return 0; 
	return *std::max_element(
			std::begin(range),
			std::end(range));
}

using measure_pair_t = std::pair<std::size_t, std::size_t>;

bool is_measure_pair(const measure_pair_t& x)
{
	//A measure pair is a two element list, who's first element (a rational
	//number), is the measure of its other element
	const auto first_element_is_rational = std::is_integral<decltype(x.first)>::value;

	const auto other_element_is_measure_of_first = 
		//To be a measure the first value must be equal-to or less-than the second.
		(x.first == x.second) ||
		((x.first < x.second )
		//And the second element must evenly divide the first element.
		&& (x.second % x.first == 0));
	return first_element_is_rational
		&& other_element_is_measure_of_first;
}

bool operator<(const measure_pair_t& rhs, const measure_pair_t& lhs)
{
	return std::tie(rhs.second, rhs.first) 
		< std::tie(lhs.second, lhs.first);
}

measure_pair_t min_pair(const measure_pair_t& a, const measure_pair_t& b)
{
	assert(is_measure_pair(a) && "First argument is not a measure-pair");
	assert(is_measure_pair(b) && "Second argument is not a measure-pair");

	if(a < b)
		return a;
	return b;
}

template <typename SinglePassRange>
measure_pair_t minimum_pair(const SinglePassRange& xs)
{
	return *std::min_element(
			std::begin(xs),
			std::end(xs),
			::minmax::operator<);
}

}
