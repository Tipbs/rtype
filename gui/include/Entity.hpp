#pragma once

#include <compare>
#include <cstddef>

class Entity {
	size_t _id;
public:
	Entity(size_t id) : _id(id) {}

	Entity &operator=(size_t id)
	{
		this->_id = id;
		return *this;
	}
	Entity &operator+(size_t id)
	{
		this->_id += id;
        return *this;
	}
	Entity &operator-(size_t id)
	{
		this->_id -= id;
        return *this;
	}
	Entity &operator*(size_t id)
	{
		this->_id *= id;
        return *this;
	}
	auto operator<= >(const size_t id) const 
	{
		return _id <= > id;
	}
	explicit operator size_t() const
	{
		return _id;
	}
};
