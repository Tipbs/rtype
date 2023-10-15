#pragma once

#include <optional>
#include <tuple>
#include <utility>

template<class... Containers>
class zipper_iterator {
    template<class Container>
    using iterator_t =
        typename Container::iterator; // type of Container::begin() return value

    template<class Container>
    using it_reference_t = typename iterator_t<Container>::reference;

  public:
    using value_type = std::tuple<typename Containers::value_type &...>;
    using reference = value_type;
    using pointer = void;
    using difference_type = size_t;
    // using iterator_category = /* proper iterator tag */;
    using iterator_tuple = std::tuple<iterator_t<Containers>...>;
    // If we want zipper_iterator to be built by zipper only .
    // friend containers::zipper<Containers...>;
    zipper_iterator(iterator_tuple const &it_tuple, size_t max, size_t idx = 0);

  public:
    zipper_iterator(zipper_iterator const &z) = default;
    zipper_iterator operator++();
    zipper_iterator &operator++(int);
    value_type operator*();
    value_type operator->();

    template<class... T>
    friend bool
    operator==(zipper_iterator const &lhs, zipper_iterator const &rhs)
    {
        return (lhs._idx == rhs._idx);
    }

    template<class... T>
    friend bool
    operator!=(zipper_iterator const &lhs, zipper_iterator const &rhs)
    {
        return (lhs._idx != rhs._idx);
    }

  private:
    // Increment every iterator at the same time. It also skips to the next
    template<size_t... Is>
    void incr_all(std::index_sequence<Is...>);
    // check if every std::optional are set.
    template<size_t... Is>
    bool all_set(std::index_sequence<Is...>);
    // return a tuple of reference to components .
    template<size_t... Is>
    value_type to_value(std::index_sequence<Is...>);

  private:
    iterator_tuple _current;
    size_t _max; // compare this value to _idx to prevent infinite loop .
    size_t _idx;
    std::index_sequence_for<Containers...> _seq {};
};

template<class... Containers>
inline zipper_iterator<Containers...>::zipper_iterator(
    iterator_tuple const &it_tuple, size_t max, size_t idx)
{
    _current = it_tuple;
    _max = max;
    _idx = 0;
    for (; _idx < _max && all_set(_seq); _idx++)
        ;
}

template<class... Containers>
inline zipper_iterator<Containers...>
zipper_iterator<Containers...>::operator++()
{
    auto it = *this;
    incr_all(_seq);
    return it;
}

template<class... Containers>
inline zipper_iterator<Containers...> &
zipper_iterator<Containers...>::operator++(int)
{
    incr_all(_seq);
    return *this;
}

template<class... Containers>
inline zipper_iterator<Containers...>::value_type
zipper_iterator<Containers...>::operator*()
{
    return to_value(_seq);
}

template<class... Containers>
inline zipper_iterator<Containers...>::value_type
zipper_iterator<Containers...>::operator->()
{
    return to_value(_seq);
}

template<class... Containers>
template<size_t... Is>
inline void zipper_iterator<Containers...>::incr_all(std::index_sequence<Is...>)
{
    if (_idx >= _max)
        return;
    _idx++;
    for (; all_set(_seq) && _idx < _max; _idx++)
        ;
}

template<class... Containers>
template<size_t... Is>
inline bool
zipper_iterator<Containers...>::all_set(std::index_sequence<Is...> seq)
{
    return (... || (!std::get<Is>(_current)[_idx].has_value()));
}

template<class... Containers>
template<size_t... Is>
inline zipper_iterator<Containers...>::value_type
zipper_iterator<Containers...>::to_value(std::index_sequence<Is...>)
{
    return std::tie(std::get<Is>(_current)[_idx]...);
}
