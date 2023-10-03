#pragma once

#include "Zipper_iterator.hpp"
#include <vector>
#include <tuple>

template <class... Containers >
class Zipper {
public:
	using iterator = zipper_iterator<Containers...>;
	using iterator_tuple = typename iterator::iterator_tuple;
	Zipper(Containers &... cs);
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
inline Zipper<Containers...>::Zipper(Containers & ...cs)
{
    iterator(cs...);
}

template<class ...Containers>
inline Zipper<Containers...>::iterator Zipper<Containers...>::begin()
{
	return iterator();
}

template<class ...Containers>
inline Zipper<Containers...>::iterator Zipper<Containers...>::end()
{
	return iterator();
}

template<class ...Containers>
inline size_t Zipper<Containers...>::_compute_size(Containers & ...containers)
{
	return size_t();
}

template<class ...Containers>
inline Zipper<Containers...>::iterator_tuple Zipper<Containers...>::_compute_end(Containers & ...containers)
{
	return iterator_tuple();
}
