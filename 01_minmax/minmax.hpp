

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

}
