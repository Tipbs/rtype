#pragma once

#include <iostream>
#include <vector>
#include <optional>
#include <iterator>

struct Position {
    float x, y;
};

template <typename Component> // You can also mirror the definition of std::vector, that takes an additional allocator.
class sparse_array {
public:
	using value_type = std::optional<Component>; // optional component type
	using reference_type = value_type&;
	using const_reference_type = value_type const&;
	using container_t = std::vector<value_type>; // optionally add your allocator template here.
	using size_type = typename container_t::size_type;
	using iterator = typename container_t::iterator;
	using const_iterator = typename container_t::const_iterator;

public:
	sparse_array() { }; // You can add more constructors .
	sparse_array(size_type size) : _data(size, std::nullopt) { }
	sparse_array(value_type c) { _data.push_back(c); }; // You can add more constructors .
	sparse_array(sparse_array const&) = default; // copy constructor
	sparse_array(sparse_array&&) noexcept = default; // move constructor
	~sparse_array() = default;
	sparse_array& operator=(sparse_array const&) = default; // copy assignment operator
	sparse_array& operator=(sparse_array&&) noexcept = default; // move assignment operator
	reference_type operator[](size_t idx);
	const_reference_type operator[](size_t idx) const;
	iterator begin();
	const_iterator begin() const;
	const_iterator cbegin() const;
	iterator end();
	const_iterator end() const;
	const_iterator cend() const;
	size_type size() const;
	//reference_type insert_at(size_type pos, Component const&);
	reference_type add_entity(size_type pos);
	reference_type insert_at(size_type pos, Component&&);
	template <class ...Params>
	reference_type emplace_at(size_type pos, Params && ...); // optional
	void erase(size_type pos);
	size_type get_index(value_type const&) const;
private:
	container_t _data;
};

template<typename Component>
inline typename sparse_array<Component>::reference_type sparse_array<Component>::operator[](size_t idx)
{
	return _data[idx];
}

template<typename Component>
inline typename sparse_array<Component>::const_reference_type sparse_array<Component>::operator[](size_t idx) const
{
	return _data[idx];
}

template<typename Component>
inline typename sparse_array<Component>::iterator sparse_array<Component>::begin()
{
	return _data.begin();
}

template<typename Component>
inline typename sparse_array<Component>::const_iterator sparse_array<Component>::begin() const
{
	return _data.begin();
}

template<typename Component>
inline typename sparse_array<Component>::const_iterator sparse_array<Component>::cbegin() const
{
	return _data.cbegin();
}

template<typename Component>
inline typename sparse_array<Component>::iterator sparse_array<Component>::end()
{
	return _data.end();
}

template<typename Component>
inline typename sparse_array<Component>::const_iterator sparse_array<Component>::end() const
{
	return _data.end();
}

template<typename Component>
inline typename sparse_array<Component>::const_iterator sparse_array<Component>::cend() const
{
	return _data.cend();
}

template<typename Component>
inline typename sparse_array<Component>::size_type sparse_array<Component>::size() const
{
	return _data.size();
}

/**
* @brief Add a new Component to one entity
*
* @tparam  Component   The type of component.
* @param   pos         the position in sparse array.
* @return  The new component.
*/
template<typename Component>
inline sparse_array<Component>::reference_type sparse_array<Component>::add_entity(size_type pos)
{
	if (pos > this->size())
		pos = this->size();
	if (pos < this->size())
		_data.erase(begin() + pos);
	return *(_data.insert(begin() + pos, std::nullopt));
}

/**
* @brief Add a new Component to one entity
*
* @tparam  Component   The type of component.
* @param   pos         the position in sparse array.
* @param   pos         the component to insert.
* @return  The component.
*/
template<typename Component>
inline typename sparse_array<Component>::reference_type sparse_array<Component>::insert_at(size_type pos, Component&& comp)
{
	if (pos > this->size())
		pos = this->size();
	if (pos < this->size())
		_data.erase(begin() + pos);
	return *(_data.insert(begin() + pos, comp));
}

/**
* @brief Erase a component at a position
*
* @tparam  Component   The type of component.
* @param   pos         the position in sparse array.
*/
template<typename Component>
inline void sparse_array<Component>::erase(size_type pos)
{
	_data.erase(begin() + pos);
}

/**
* @brief Get the index of a component
*
* @tparam  Component   The type of component.
* @param   pos         The component..
*/
template<typename Component>
inline typename sparse_array<Component>::size_type sparse_array<Component>::get_index(value_type const& comp) const
{
	auto it = 0;
	while (it < size())
	{
		if (_data[it] == comp)
			return it;
		it++;
	}
	return _data.end() - _data.begin();
}

/**
* @brief Create and insert a component
*
* @tparam  Component   The Component to create
* @tparam  ...Params   The param's type to forward to constructor
* @param   pos         The index in sparse array
* @param   c           The new component to add
* @return  The new component created
*/
template<typename Component>
template<class ...Params>
inline typename sparse_array<Component>::reference_type sparse_array<Component>::emplace_at(size_type pos, Params &&...params)
{
    int x = 1.0, y = 2.0;
    std::optional<Position> comp = std::make_optional<Component>(x, y);
    std::optional<Position> &temp = *(_data.emplace(begin() + pos, comp));
    std::cout << sizeof(temp) << std::endl;
	return temp;
}
