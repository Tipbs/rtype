#pragma once

#include "zipper_iterator.hpp"
#include <vector>

template <class ... Containers >
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
	//using iterator = zipper_iterator<Containers...>;
	//using iterator_tuple = typename iterator::iterator_tuple;
	//zipper_iterator(iterator_tuple const& it_tuple, size_t max);
	//using iterator_tuple = std::tuple<iterator_t<Containers>...>;
	//using iterator_t = decltype(sparse_array<Container>.begin()); // type of Container::begin() return value
	std::vector vec = { 0, 1, 2 };
	++vec.begin();
	//auto tuple = std::tuple<
	//auto ite = zipper_iterator<...Containers>();
	//_begin = 
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
