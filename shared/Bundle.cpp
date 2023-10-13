#include "Bundle.hpp"
#include <cmath>
#include <cstdlib>
#include "Component.hpp"
#include "random"

void create_player(Registry &reg, int id)
{
    Entity const new_entity = reg.spawn_entity();
    Player player(id, 1.);
    Position Pos(0, 0);
    Size Size(83, 43);
    std::string path = "./gui/ressources/Sprites/r-typesheet42.gif";
    Speed speedo(300);
    Direction diro(0, 0);
    SpawnGrace gra(5);
    Sprite sprite(path.c_str(), 83, 43, 5, 5);

    reg.add_component(new_entity, std::move(Pos));
    reg.add_component(new_entity, std::move(Size));
    reg.add_component(new_entity, std::move(sprite));
    reg.add_component(new_entity, std::move(speedo));
    reg.add_component(new_entity, std::move(diro));
    reg.add_component(new_entity, std::move(gra));
    reg.add_component(new_entity, std::move(player));
}

void create_ammo(Registry &reg, Position pos, Weapon original_weapon)
{
    Entity const new_entity = reg.spawn_entity();
    // Ammo amo = original_weapon.ammo;
    Size Size(50, 10);
    std::string path = "./gui/ressources/Sprites/r-typesheet42.gif";
    Speed speedo(450);
    Direction diro = Direction(std::pow(-1, rand()) * rand(), std::pow(-1, rand()) * rand());
    Sprite sprite(path.c_str(), 83, 43, 5, 5);

    reg.add_component(new_entity, std::move(pos));
    reg.add_component(new_entity, std::move(Size));
    reg.add_component(new_entity, std::move(sprite));
    reg.add_component(new_entity, std::move(speedo));
    reg.add_component(new_entity, std::move(diro));
    // reg.add_component(new_entity, std::move(amo));
}
