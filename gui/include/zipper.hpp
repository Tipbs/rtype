#pragma once

#include "zipper_iterator.hpp"
#include <vector>
#include <tuple>

template <class... Containers >
class zipper {
public:
	using iterator = zipper_iterator<Containers...>;
	using iterator_tuple = typename iterator::iterator_tuple;
	zipper(Containers &... cs);
	iterator begin();
	iterator end();
private:
	// helper function to know the maximum index of our iterators .
	static size_t _compute_size(Containers &... containers);
	// helper function to compute an iterator_tuple that will allow us to
	// build our end iterator.
	static iterator_tuple _compute_end(Containers &... containers);
private:
	iterator_tuple _begin;
	iterator_tuple _end;
	size_t _size;
};

template<class ...Containers>
inline zipper<Containers...>::zipper(Containers & ...cs)
{
    _size = _compute_size(cs...);
    _begin = std::make_tuple(cs.begin()...);
    _end = _compute_end(cs...);
}

template<class ...Containers>
inline zipper<Containers...>::iterator zipper<Containers...>::begin()
{
	return zipper_iterator<Containers...>(_begin, _size);
}

template<class ...Containers>
inline zipper<Containers...>::iterator zipper<Containers...>::end()
{
    return zipper_iterator<Containers...>(_end, _size, _size);
}

template<class ...Containers>
inline size_t zipper<Containers...>::_compute_size(Containers & ...containers)
{
    return std::max({containers.size()...});
}

template<class ...Containers>
inline zipper<Containers...>::iterator_tuple zipper<Containers...>::_compute_end(Containers & ...containers)
{
    return std::make_tuple(containers.end()...);
}
