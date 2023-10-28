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
//     return _reg.register_components<
// #ifndef SERVER
//         Sprite,
//         Ammo,
//         Weapon,
// #endif
//         Current_Player,
//         Position,
//         Damages,
//         Size,
//         Health,
//         Speed,
//         Direction,
//         SpawnGrace
//         // Animation
//     >();
}

Factory::Factory(Registry &reg) : _reg(reg)
{
    _register_components();
}

const Entity Factory::_create_player(int id, Utils::Vec2 pos, int type)
{
    Entity const new_entity = _reg.spawn_entity();
    Player player(1);
    Size Size(83, 43);
    std::string path = "./gui/ressources/Sprites/r-typesheet42.gif";
    Speed speedo(300);
    Direction diro(0, 0);
    SpawnGrace gra(std::chrono::seconds(1));
    #ifndef SERVER
    Sprite sprite(path.c_str(), 83, 43, 5, 5);
    #endif

    _reg.add_component(new_entity, std::move(pos));
    _reg.add_component(new_entity, std::move(Size));
    #ifndef SERVER
    _reg.add_component(new_entity, std::move(sprite));
    #endif
    _reg.add_component(new_entity, std::move(speedo));
    _reg.add_component(new_entity, std::move(diro));
    _reg.add_component(new_entity, std::move(gra));
    _reg.add_component(new_entity, std::move(player));

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
