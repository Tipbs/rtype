#include "Factory.hpp"
#include <chrono>


inline auto &Factory::_register_components()
{
    return _reg.register_components<
        Player,
        NetworkEntity,
        Position,
        Damages,
        Size,
        Health,
        Speed,
        Direction,
        SpawnGrace,
        Sprite,
        Animation,
        Ammo,
        Weapon
    >();    
}

const Entity Factory::_create_player(int id, Utils::Vec2 pos, int type)
{
    const Entity ent = _reg.spawn_entity();
    _reg.emplace_component<Player>(ent);
    _reg.emplace_component<Position>(ent, pos);
    _reg.emplace_component<NetworkEntity>(ent, id);
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
