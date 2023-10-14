#pragma once

#include <utility>
#include <tuple>
#include <optional>

template <class ...Containers>
class indexed_zipper_iterator {
	template <class Container>
	using iterator_t = typename Container::iterator; // type of Container::begin() return value

	template <class Container>
	using it_reference_t = typename iterator_t<Container>::reference;
public:
	using value_type = std::tuple<int, typename Containers::value_type &...>;
	using reference = value_type;
	using pointer = void;
	using difference_type = size_t;
	//using iterator_category = /* proper iterator tag */;
	using iterator_tuple = std::tuple<iterator_t<Containers>...>;
	// If we want indexed_zipper_iterator to be built by zipper only .
	// friend containers::zipper<Containers...>;
	indexed_zipper_iterator(iterator_tuple const& it_tuple, size_t max);
	indexed_zipper_iterator(iterator_tuple const& it_tuple, size_t idx, size_t max);
public:
	indexed_zipper_iterator(indexed_zipper_iterator const& z) = default;
	indexed_zipper_iterator operator++();
	indexed_zipper_iterator& operator++(int);
	value_type operator*();
	value_type operator->();

    template <class ...T>
	friend bool operator==(indexed_zipper_iterator const& lhs, indexed_zipper_iterator const
		&rhs)
    {
        return (lhs._idx == rhs._idx);
    }
    template <class ...T>
	friend bool operator!=(indexed_zipper_iterator const& lhs, indexed_zipper_iterator const
		&rhs)
    {
        return (lhs._idx != rhs._idx);
    }
private:
	// Increment every iterator at the same time. It also skips to the next
	template <size_t ...Is>
	void incr_all(std::index_sequence<Is...>);
	// check if every std::optional are set.
	template <size_t ...Is>
	bool all_set(std::index_sequence<Is...>);
	// return a tuple of reference to components .
	template <size_t ...Is>
	value_type to_value(std::index_sequence<Is...>);
private:
	iterator_tuple _current;
	size_t _max; // compare this value to _idx to prevent infinite loop .
	size_t _idx;
	std::index_sequence_for<Containers...> _seq{};
};

template<class ...Containers>
inline indexed_zipper_iterator<Containers...>::indexed_zipper_iterator(iterator_tuple const& it_tuple, size_t max)
{
	_current = it_tuple;
	_max = max;
    _idx = 0;
    for (; all_set(_seq) && _idx < _max; _idx++);
}

template<class ...Containers>
inline indexed_zipper_iterator<Containers...>::indexed_zipper_iterator(iterator_tuple const& it_tuple, size_t idx, size_t max)
{
	_current = it_tuple;
	_max = max;
    _idx = idx;
    for (; all_set(_seq) && _idx < _max; _idx++);
}

template<class ...Containers>
inline indexed_zipper_iterator<Containers...> indexed_zipper_iterator<Containers...>::operator++()
{
    auto it = *this;
    incr_all(_seq);
    return it;
}

template<class ...Containers>
inline indexed_zipper_iterator<Containers...>& indexed_zipper_iterator<Containers...>::operator++(int)
{
    incr_all(_seq);
    return *this;
}

template<class ...Containers>
inline indexed_zipper_iterator<Containers...>::value_type indexed_zipper_iterator<Containers...>::operator*()
{
	return to_value(_seq);
}

template<class ...Containers>
inline indexed_zipper_iterator<Containers...>::value_type indexed_zipper_iterator<Containers...>::operator->()
{
	return to_value(_seq);
}

template<class ...Containers>
template <size_t ...Is>
inline void indexed_zipper_iterator<Containers...>::incr_all(std::index_sequence<Is...>)
{
    if (_idx >= _max)
        return;
    _idx++;
    for (; all_set(_seq) && _idx < _max; _idx++);
}

template<class ...Containers>
template <size_t... Is>
inline bool indexed_zipper_iterator<Containers...>::all_set(std::index_sequence<Is ...> seq)
{
    return (... || (!std::get<Is>(_current)[_idx].has_value()));
}

template<class ...Containers>
template<size_t ...Is>
inline indexed_zipper_iterator<Containers...>::value_type indexed_zipper_iterator<Containers...>::to_value(std::index_sequence<Is...>)
{
    return std::tie(_idx, std::get<Is>(_current)[_idx]...);
}
