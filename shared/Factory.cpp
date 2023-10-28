#include "Factory.hpp"
#include "Component.hpp"
#include <chrono>
#include <stdlib.h>     /* srand, rand */

#ifndef SERVER
    #include "../gui/include/GraphicComponents.hpp"
#endif

#include "Bundle.hpp"

inline auto Factory::_register_components()
{
    return _reg.register_components<
#ifndef SERVER
        Sprite,
        Weapon,
#endif
        Player,
        Current_Player,
        Position,
        Damages,
        Size,
        Health,
        Speed,
        Direction,
        SpawnGrace,
        NetworkedEntity,
        Animation
    >();
}

Factory::Factory(Registry &reg) : _reg(reg)
{
    _register_components();
}

const Entity Factory::_create_player(int id, Utils::Vec2 pos, int type)
{
    Entity const new_entity = _reg.spawn_entity();

    _reg.emplace_component<Player>(new_entity);
    _reg.emplace_component<Position>(new_entity, pos);
    _reg.emplace_component<Size>(new_entity, 83, 43);
#ifndef SERVER
    _reg.emplace_component<Sprite>(new_entity, "./gui/ressources/Sprites/r-typesheet42.gif", 83, 43, 5, 5);
#endif
    _reg.emplace_component<Speed>(new_entity, 300);
    _reg.emplace_component<Direction>(new_entity, 0, 0);
    _reg.emplace_component<SpawnGrace>(new_entity, std::chrono::seconds(1));
    _reg.emplace_component<Animation>(new_entity, 1);
    _reg.emplace_component<NetworkedEntity>(new_entity, id);

    return new_entity;
}

const Entity Factory::_create_enemy()
{
    const Entity ent = _reg.spawn_entity();
    float randomNumber = rand() % 100 + 1;
    Utils::Vec2 pos = {900, randomNumber};

    _reg.emplace_component<Position>(ent, pos);
    _reg.emplace_component<Speed>(ent, 300);
    _reg.emplace_component<Direction>(ent, 50, 0);
    _reg.emplace_component<SpawnGrace>(ent, std::chrono::seconds(5));
    // _reg.emplace_component<NetworkEntity>(ent, id);
    return ent;
}
