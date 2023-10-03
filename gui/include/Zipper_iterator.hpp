#pragma once

#include <utility>
#include <tuple>
#include <optional>

template <class ...Containers>
class zipper_iterator {
	template <class Container>
	using iterator_t = typename Container::iterator; // type of Container::begin() return value

	template <class Container>
	using it_reference_t = typename iterator_t<Container>::reference;
public:
	using value_type = std::tuple<typename Containers::value_type...>;
	using reference = value_type;
	using pointer = void;
	using difference_type = size_t;
	//using iterator_category = /* proper iterator tag */;
	using iterator_tuple = std::tuple<iterator_t<Containers>...>;
	// If we want zipper_iterator to be built by zipper only .
	//friend containers::zipper<Containers...>;
	zipper_iterator(iterator_tuple const& it_tuple, size_t max);
public:
	zipper_iterator(zipper_iterator const& z) = default;
	zipper_iterator operator++();
	zipper_iterator& operator++(int);
	value_type operator*();
	value_type operator->();
	friend bool operator==(zipper_iterator const& lhs, zipper_iterator const
		& rhs);
	friend bool operator!=(zipper_iterator const& lhs, zipper_iterator const
		& rhs);
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
	static constexpr std::index_sequence_for<Containers...> _seq{};
};

/**
* @brief Erase one entity from one component.
*
* @tparam  Component   The component from the entity will be erase.
* @param   entity      The entity to erase.
*/
template<class ...Containers>
inline zipper_iterator<Containers...>::zipper_iterator(iterator_tuple const& it_tuple, size_t max)
{
	_current = it_tuple;
	_max = max;
}

template<class ...Containers>
inline zipper_iterator<Containers...> zipper_iterator<Containers...>::operator++()
{
    incr_all(_seq);
    // return _current[_idx];
}

template<class ...Containers>
inline zipper_iterator<Containers...>& zipper_iterator<Containers...>::operator++(int)
{
    incr_all(_seq);
    // return _current[_idx - 1];
}

template<class ...Containers>
inline zipper_iterator<Containers...>::value_type zipper_iterator<Containers...>::operator*()
{
	// return std::make_tuple<typename Containers::reference_type...>(_current[_idx]);
}

template<class ...Containers>
inline zipper_iterator<Containers...>::value_type zipper_iterator<Containers...>::operator->()
{
	// return std::make_tuple<typename Containers::reference_type...>(*_current);
}

template<class ...Containers>
bool operator==(zipper_iterator<Containers...> const& lhs, zipper_iterator<Containers...> const& rhs)
{
	return lhs._idx == rhs._idx;
}

template<class ...Containers>
bool operator!=(zipper_iterator<Containers...> const& lhs, zipper_iterator<Containers...> const& rhs)
{
	return !(lhs == rhs);
}

template<class ...Containers>
template <size_t ...Is>
inline void zipper_iterator<Containers...>::incr_all(std::index_sequence<Is...>)
{
    for (; all_set(_seq); _idx++) {
        _idx += 1;
    }
}

template<class ...Containers>
template <size_t... Is>
inline bool zipper_iterator<Containers...>::all_set(std::index_sequence<Is ...> seq)
{
    // return (... && (*_current[Is] == std::nullopt));
}

template<class ...Containers>
template<size_t ...Is>
inline zipper_iterator<Containers...>::value_type zipper_iterator<Containers...>::to_value(std::index_sequence<Is...>)
{
	return value_type();
}
