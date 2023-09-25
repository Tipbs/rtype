#pragma once

#include <any>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include "sparse_array.hpp"
#include "entity.hpp"

class Registry {
	template<typename Component>
	void erase(Entity const &entity);
	template<typename Component>
	void insert(size_t entity);
	template<class ...Components, typename Function>
	void create_system(Function&& f);
public:
	template <class Component>
	sparse_array<Component>& register_component();
	template <class Component>
	sparse_array<Component>& get_components();
	template <class Component>
	sparse_array<Component> const& get_components() const;
	Entity spawn_entity();
	Entity entity_from_index(std::size_t idx);
	void kill_entity(Entity const& e);
	template <typename Component>
	typename sparse_array<Component>::reference_type add_component(Entity const& to,
		Component&& c);
	template <typename Component, typename ...Params>
	typename sparse_array<Component>::reference_type emplace_component(Entity const& to
		, Params &&... p);
	template <typename Component>
	void remove_component(Entity const& from);
	template <class ...Components, typename Function>
	void add_system(Function&& f); // perfect forwarding in lambda capture , anyone ?
	template <class ...Components, typename Function>
	void add_system(Function const& f); // taking it by reference .
	void run_systems();

private:
	std::unordered_map<std::type_index, std::any> _components_arrays;
	std::unordered_map<std::type_index, std::function<void(Registry &, Entity const &)>> _erase_funcs;
	std::unordered_map<std::type_index, std::function<void(Registry &, size_t)>> _insert_funcs;
	std::vector<std::function<void(Registry&)>> _systems;
	size_t _maxId = 0;
};

template<typename Component>
inline void Registry::erase(const Entity &entity)
{
	std::any_cast<sparse_array<Component>>(_components_arrays[typeid(Component)])[(size_t)entity] = std::nullopt;
}

template<typename Component>
inline void Registry::insert(size_t entity)
{
	auto &comp = std::any_cast<sparse_array<Component> &>(_components_arrays[typeid(Component)]);
	comp.add_entity(entity);
}

template<class Component>
inline sparse_array<Component>& Registry::register_component()
{
	_components_arrays.insert_or_assign(typeid(Component), std::any(sparse_array<Component>(_maxId)));
	_erase_funcs.insert_or_assign(typeid(Component), &Registry::erase<Component>);
	_insert_funcs.insert_or_assign(typeid(Component), &Registry::insert<Component>);
	return std::any_cast<sparse_array<Component> &>(_components_arrays.at(typeid(Component)));
}

template<class Component>
inline sparse_array<Component>& Registry::get_components()
{
	return std::any_cast<sparse_array<Component> &>(_components_arrays[typeid(Component)]);
}

template<class Component>
inline sparse_array<Component> const& Registry::get_components() const
{
	return std::any_cast<sparse_array<Component> &>(_components_arrays[typeid(Component)]);
}

template<typename Component>
inline typename sparse_array<Component>::reference_type Registry::add_component(Entity const& to, Component&& c)
{
	auto &comp_array = std::any_cast<sparse_array<Component> &>(_components_arrays[typeid(Component)]);
	comp_array[(size_t)to] = std::move(c);
	return comp_array[(size_t)to];
}

template<typename Component, typename ...Params>
inline typename sparse_array<Component>::reference_type Registry::emplace_component(Entity const& to, Params && ...p)
{
	_components_arrays[typeid(Component)].emplace(typeid(Component), p);
	return typename sparse_array<Component>::reference_type();
}

template<typename Component>
inline void Registry::remove_component(Entity const& from)
{
	_erase_funcs(typeid(Component), (size_t)from);
}

template<class ...Components, typename Function>
inline void Registry::add_system(Function&& f)
{
	auto lambda = [&](Registry& reg) {
		f(reg, get_components<Components>()...);
	};
	_systems.push_back(lambda);
}

template<class ...Components, typename Function>
inline void Registry::add_system(Function const& f)
{
	auto lambda = [&](Registry& reg) {
		f(reg, get_components<Components>()...);
	};
	_systems.push_back(lambda);
}
