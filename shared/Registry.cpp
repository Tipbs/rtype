#include "Registry.hpp"

Entity Registry::spawn_entity()
{
    for (const auto &[key, _] : _components_arrays)
        _insert_funcs[key](std::ref(*this), _maxId);
    ++_maxId;
    return _maxId - 1;
}

Entity Registry::entity_from_index(std::size_t idx) { return idx; }

void Registry::kill_entity(Entity const &e)
{
    for (auto &arr : _components_arrays)
        _erase_funcs[arr.first](std::ref(*this), e);
}

void Registry::kill_all_entities()
{
    for (const auto &[key, _] : _components_arrays) {
        std::cout << "test1" << std::endl;
        for (int i = 0; i < _maxId; i++) {
            std::cout << i << std::endl;
            _erase_funcs[key](std::ref(*this), i);
        }
    }
    _maxId = 0;
}

void Registry::run_systems()
{
    for (auto &system : _systems)
        system(std::ref(*this));
}
