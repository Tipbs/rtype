#pragma once

#include <tuple>
#include <vector>
#include "indexed_zipper_iterator.hpp"

template<class... Containers>
class indexed_zipper {
  public:
    using iterator = indexed_zipper_iterator<Containers...>;
    using iterator_tuple = typename iterator::iterator_tuple;
    indexed_zipper(Containers &...cs);
    iterator begin();
    iterator end();

  private:
    // helper function to know the maximum index of our iterators .
    static size_t _compute_size(Containers &...containers);
    // helper function to compute an iterator_tuple that will allow us to
    // build our end iterator.
    static iterator_tuple _compute_end(Containers &...containers);

  private:
    iterator_tuple _begin;
    iterator_tuple _end;
    size_t _size;
};

template<class... Containers>
inline indexed_zipper<Containers...>::indexed_zipper(Containers &...cs)
{
    _size = _compute_size(cs...);
    _begin = std::make_tuple(cs.begin()...);
    _end = _compute_end(cs...);
}

template<class... Containers>
inline indexed_zipper<Containers...>::iterator
indexed_zipper<Containers...>::begin()
{
    return iterator(_begin, _size);
}

template<class... Containers>
inline indexed_zipper<Containers...>::iterator
indexed_zipper<Containers...>::end()
{
    return iterator(_end, _size, _size);
}

template<class... Containers>
inline size_t
indexed_zipper<Containers...>::_compute_size(Containers &...containers)
{
    return std::max({containers.size()...});
}

template<class... Containers>
inline indexed_zipper<Containers...>::iterator_tuple
indexed_zipper<Containers...>::_compute_end(Containers &...containers)
{
    return std::make_tuple(containers.end()...);
}
