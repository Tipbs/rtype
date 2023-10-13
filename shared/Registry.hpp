#pragma once

#include <any>
#include <unordered_map>
#include <typeindex>
#include <functional>
#include "Sparse_array.hpp"
#include "Entity.hpp"
#include "NetEnt.hpp"
#ifndef SERVER
	#include "UserCmd.hpp"
#endif

class Registry {
	template<typename Component>
	void erase(Entity const &entity);
	template<typename Component>
	void insert(size_t entity);
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
	#ifndef SERVER
		ThreadNetEnt netEnts;
        ThreadUserCmd currentCmd;
	#endif

private:
	std::unordered_map<std::type_index, std::any> _components_arrays;
	std::unordered_map<std::type_index, std::function<void(Registry &, Entity const &)>> _erase_funcs;
	std::unordered_map<std::type_index, std::function<void(Registry &, size_t)>> _insert_funcs;
	std::vector<std::function<void(Registry&)>> _systems;
	size_t _maxId = 0;
};

/**
* @brief Erase one entity from one component.
*
* @tparam  Component   The component from the entity will be erase.
* @param   entity      The entity to erase.
*/
template<typename Component>
inline void Registry::erase(const Entity &entity)
{
	std::any_cast<sparse_array<Component>>(_components_arrays[typeid(Component)])[(size_t)entity] = std::nullopt;
}

/**
* @brief Insert one entity to one component.
*
* @tparam  Component   The component from the entity will be insert.
* @param   entity      The entity to insert.
*/
template<typename Component>
inline void Registry::insert(size_t entity)
{
	auto &comp = std::any_cast<sparse_array<Component> &>(_components_arrays[typeid(Component)]);
	comp.add_entity(entity);
}

/**
* @brief Add a new component with its erase and insert function.
*
* @tparam  Component   The Component to insert.
* @return  The sparse_array of Component newly created.
*/
template<class Component>
inline sparse_array<Component>& Registry::register_component()
{
	_components_arrays.insert_or_assign(typeid(Component), std::any(sparse_array<Component>(_maxId)));
	_erase_funcs.insert_or_assign(typeid(Component), &Registry::erase<Component>);
	_insert_funcs.insert_or_assign(typeid(Component), &Registry::insert<Component>);
	return std::any_cast<sparse_array<Component> &>(_components_arrays.at(typeid(Component)));
}

/**
* @brief Get component in component array
*
* @tparam  Component   The Component to get
* @return  The sparse array of the component.
*/
template<class Component>
inline sparse_array<Component>& Registry::get_components()
{
	return std::any_cast<sparse_array<Component> &>(_components_arrays[std::type_index(typeid(Component))]);
}

/**
* @brief Get component in component array.
*
* @tparam  Component   The Component to get.
* @return  The sparse array of the component.
*/
template<class Component>
inline sparse_array<Component> const& Registry::get_components() const
{
	return std::any_cast<sparse_array<Component> &>(_components_arrays.at(std::type_index(typeid(Component))));
}

/**
* @brief Add a component to an entity in a sparse array
*
* @tparam  Component   The Component from sparse_array
* @param   to          The index in sparse array
* @param   c           The new component to add
* @return  The new component created
*/
template<typename Component>
inline typename sparse_array<Component>::reference_type Registry::add_component(Entity const& to, Component&& c)
{
	auto &comp_array = std::any_cast<sparse_array<Component> &>(_components_arrays[typeid(Component)]);
	comp_array[(size_t)to] = std::move(c);
	return comp_array[(size_t)to];
}

/**
* @brief Create and insert a component
*
* @tparam  Component   The Component to create
* @tparam  ...Params   The param's type to forward to constructor
* @param   to          The index in sparse array
* @param   c           The new component to add
* @return  The new component created
*/
template<typename Component, typename ...Params>
inline typename sparse_array<Component>::reference_type Registry::emplace_component(Entity const& to, Params && ...p)
{
	auto &sparse_arr = std::any_cast<sparse_array<Component> &>(_components_arrays[typeid(Component)]);
	sparse_arr[(size_t)to] = std::make_optional<Component>(p...);
	return sparse_arr[(size_t) to];
}

/**
* @brief Remove a compoenent in a sparse array
*
* @tparam  Component   The Component who point sparse array.
* @param   from        The entity (index in sparse array)
*/
template<typename Component>
inline void Registry::remove_component(Entity const& from)
{
	_erase_funcs[std::type_index(typeid(Component))](std::ref(*this), from);
}

/**
* @brief Add a system to one or more Component
*
* @tparam  ...Component The Component who will be assign to the system.
* @tparam  ...Component The type of the system.
* @param   t            The system who will be assign.
*/
template<class ...Components, typename Function>
inline void Registry::add_system(Function&& f)
{
	auto lambda = [&](Registry& reg) {
		f(reg, get_components<Components>()...);
	};
	_systems.push_back(lambda);
}

/**
* @brief Add a system to one or more Component
*
* @tparam  ...Component The Component who will be assign to the system.
* @tparam  ...Component The type of the system.
* @param   t            The system who will be assign.
*/
template<class ...Components, typename Function>
inline void Registry::add_system(Function const& f)
{
	auto lambda = [&](Registry& reg) {
		f(reg, get_components<Components>()...);
	};
	_systems.push_back(lambda);
}
