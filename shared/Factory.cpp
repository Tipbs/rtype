#include "Factory.hpp"
#include "Component.hpp"
#include "Systems.hpp"
#include <chrono>
#include <stdlib.h>     /* srand, rand */

#ifndef SERVER
    #include "../gui/include/GraphicComponents.hpp"
    #include "../gui/include/GraphicSystems.hpp"
#endif


void Factory::register_components()
{
    _reg.register_components<
#ifndef SERVER
        Sprite,
        Weapon,
        InputField,
        Rectangle,
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

void Factory::add_systems()
{
    _reg.add_system<SpawnGrace>(update_grace);
    _reg.add_system<Position, Size, SpawnGrace, Damages, Health>(colision);
    _reg.add_system<Position, Speed, Direction>(move);
#ifndef SERVER
    _reg.add_system<Position, Size, Sprite, Player, Rectangle, InputField>(
        display);
    _reg.add_system<Direction, Player, Sprite, Speed, Animation>(
        handle_dir_inputs);
    _reg.add_system<Player, Size, Weapon, Position>(handle_shoot_inputs);
    //    _reg.add_system<InputField, Rectangle>(hadle_text_inputs);
    _reg.add_system<
        Position, NetworkedEntity, Speed, Current_Player, Size, Player>(
        updateWithSnapshots);
#else

#endif
}

Factory::Factory(Registry &reg) : _reg(reg)
{
}

const Entity Factory::create_background(const int ScreenWidth, const int ScreenHeight)
{
    Entity const background = _reg.spawn_entity();
    std::string bgpath = "./gui/ressources/Backgrounds/Backtest.png"; // temp > 2 > 3 > 1

    _reg.emplace_component<Position>(background, 0, 0);
    _reg.emplace_component<Size>(background, ScreenWidth, ScreenHeight);
#ifndef SERVER
    _reg.emplace_component<Sprite>(background, bgpath.c_str(), 3 * ScreenWidth, ScreenHeight);
#endif
    _reg.emplace_component<Speed>(background, 2);
    _reg.emplace_component<Direction>(background, -1, 0);
    return background;
}

const Entity Factory::create_player(int id, Position pos)
{
    Entity const new_entity = _reg.spawn_entity();
    std::string const path = "./gui/ressources/Sprites/Testships.png";

    _reg.emplace_component<Player>(new_entity);
    _reg.emplace_component<Position>(new_entity, pos);
    _reg.emplace_component<Size>(new_entity, 83, 43);
#ifndef SERVER
    _reg.emplace_component<Sprite>(new_entity, path.c_str(), 83, 43, 5, 5);
#endif
    _reg.emplace_component<Speed>(new_entity, 5);
    _reg.emplace_component<Direction>(new_entity, 0, 0);
    _reg.emplace_component<SpawnGrace>(new_entity, std::chrono::seconds(1));
    _reg.emplace_component<Animation>(new_entity, 1);
    _reg.emplace_component<NetworkedEntity>(new_entity, id);

    return new_entity;
}

const Entity Factory::create_weapon(Entity owner)
{
    Entity weapon = _reg.spawn_entity();

    _reg.emplace_component<Weapon>(weapon, owner);
    _reg.emplace_component<Position>(weapon);
    return weapon;
}

const Entity Factory::create_enemy()
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

const Entity Factory::create_ammo(Position pos, float damage_mult, int color_id)
{
    Entity const new_entity = _reg.spawn_entity();
    int hitwidth = 120 * (damage_mult / 2);
    int hitheight = 24 * (damage_mult / 2);

    _reg.emplace_component<Position>(new_entity, pos.pos_X - (float) hitwidth / 2, pos.pos_Y - (float) hitheight / 2);
    _reg.emplace_component<Size>(new_entity, hitwidth, hitheight);
    #ifndef SERVER
    _reg.emplace_component<Sprite>(new_entity, "./gui/ressources/Sprites/shoot_ammo.png");
    #endif
    _reg.emplace_component<Speed>(new_entity, 450);
    _reg.emplace_component<Direction>(new_entity, 1, 0);
    _reg.emplace_component<Damages>(new_entity, damage_mult);
    _reg.emplace_component<Health>(new_entity, 1);
    return new_entity;
}
