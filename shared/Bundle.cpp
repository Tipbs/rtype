#include "Bundle.hpp"
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include "Component.hpp"
#include "random"

size_t create_player(Registry &reg, bool is_curret_player)
{
    Entity const new_entity = reg.spawn_entity();
    Player player((size_t)new_entity % 5, 1.);
    Position Pos((1280./10) * ((size_t)new_entity - 1), 300);
    Size Size(83, 43);
    std::string path = "./gui/ressources/Sprites/r-typesheet42.gif";
    Speed speedo(300);
    Direction diro(0, 0);
    SpawnGrace gra(5);
    Sprite sprite(path.c_str(), 83, 43, 5, 5);
    if (is_curret_player)
        player.id = (size_t)new_entity;

    reg.add_component(new_entity, std::move(Pos));
    reg.add_component(new_entity, std::move(Size));
    reg.add_component(new_entity, std::move(sprite));
    reg.add_component(new_entity, std::move(speedo));
    reg.add_component(new_entity, std::move(diro));
    reg.add_component(new_entity, std::move(gra));
    reg.add_component(new_entity, std::move(player));
    return (size_t)new_entity;
}

void create_ammo(Registry &reg, Position pos, float damage_mult)
{
    Entity const new_entity = reg.spawn_entity();
    int hitwidth = 120 * damage_mult;
    int hitheight = 24 * damage_mult;
    Size Size(hitwidth, hitheight); //hitbox
    std::string path = "./gui/ressources/Sprites/shoot_ammo.png"; // fichier de la munition
    Speed speedo(450);
    Damages damag(damage_mult);
    SpawnGrace grac(0.5 * damage_mult);
    Direction diro = Direction(1, 0); //std::pow(-1, rand()) * rand()
    Sprite sprite(path.c_str(), hitwidth, hitheight, 8, 1); // width height are the visual box

    reg.add_component(new_entity, Position(
                        pos.pos_X - (float)hitwidth/2,
                        pos.pos_Y - (float)hitheight/2));
    reg.add_component(new_entity, std::move(Size));
    reg.add_component(new_entity, std::move(sprite));
    reg.add_component(new_entity, std::move(speedo));
    reg.add_component(new_entity, std::move(diro));
    reg.add_component(new_entity, std::move(damag));
    reg.add_component(new_entity, std::move(grac));
}
