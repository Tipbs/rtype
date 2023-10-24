#include "Factory.hpp"
#include "Component.hpp"
#include <chrono>
#include <stdlib.h>     /* srand, rand */

#ifndef CLIENT
    #include "../gui/include/GraphicComponents.hpp"
#endif


inline auto &Factory::_register_components()
{
    return _reg.register_components<
#ifndef CLIENT
        Sprite,
#endif
#ifndef SERVER
        Ammo,
        Weapon,
#endif
        Current_Player,
        // NetworkEntity,
        Position,
        Damages,
        Size,
        Health,
        Speed,
        Direction,
        SpawnGrace,
        Animation
    >();
}

const Entity Factory::_create_player(int id, Utils::Vec2 pos, int type)
{
    const Entity ent = _reg.spawn_entity();

    _reg.emplace_component<Current_Player>(ent, id);
    _reg.emplace_component<Position>(ent, pos);
    // _reg.emplace_component<NetworkEntity>(ent, id);
    _reg.emplace_component<Speed>(ent, 300);
    _reg.emplace_component<Direction>(ent, 50, 0);
    _reg.emplace_component<SpawnGrace>(ent, std::chrono::seconds(5));
    #ifndef CLIENT
        switch (id) {
            case (0):
                _reg.emplace_component<Sprite>(ent, "./gui/ressources/Sprites/r-typesheet42.gif", 83, 43, 5, 5);
            case (1):
                _reg.emplace_component<Sprite>(ent, "./gui/ressources/Sprites/r-typesheet42.gif", 83, 43, 5, 5);
            case (2):
                _reg.emplace_component<Sprite>(ent, "./gui/ressources/Sprites/r-typesheet42.gif", 83, 43, 5, 5);
            case (3):
                _reg.emplace_component<Sprite>(ent, "./gui/ressources/Sprites/r-typesheet42.gif", 83, 43, 5, 5);
        }
    #endif
    return ent;
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
