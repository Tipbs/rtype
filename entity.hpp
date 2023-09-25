#pragma once

class Entity {
	size_t _id;
public:
	Entity(size_t id) : _id(id) {}

	Entity &operator=(size_t id)
	{
		this->_id = id;
	}
	Entity &operator+(size_t id)
	{
		this->_id += id;
	}
	Entity &operator-(size_t id)
	{
		this->_id -= id;
	}
	Entity &operator*(size_t id)
	{
		this->_id *= id;
	}
	auto operator<=>(const size_t id) const 
	{
		return _id <=> id;
	}
	explicit operator size_t() const
	{
		return _id;
	}
};
