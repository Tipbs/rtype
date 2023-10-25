#pragma once

#include <string>
#include <utility>
#ifndef SERVER
#include <raylib.h>
#endif
#include "Registry.hpp"
#include "Component.hpp"

#ifndef SERVER
#include "GraphicComponents.hpp"
struct Ammo {
    Sprite sprite = Sprite("./gui/ressources/Sprites/shoot_charge.png");
    Speed speed = Speed(300);
    Damages damages = Damages(1);
    Size size = Size(1,1);
    Health health = Health(1);
    Direction dir = Direction(1,0);
};

struct Weapon {
    int type = 1;
    Size size = Size(1,1);
    double attack_speed = 1.;
    Sprite sprite = Sprite("./gui/ressources/Sprites/r-typesheet42.gif");

    Weapon(int type, int ammo_per_sec): type(type)
    {
        attack_speed = 1. / ammo_per_sec;
    };
};
#endif


struct Player {
    int color_id = -1;
    Entity id = -1;
    double count = 1;
    bool IsShooting = false;
    float current_charge = 0.;
    #ifndef SERVER
    Weapon weapon = Weapon(1, 5);
    #endif

    Player(double count, int color_id = -1) : color_id(color_id), count(count) {};

    Player(Entity id, int color_i = -1) : color_id(color_i), id(id) {};
};

size_t create_player(Registry &reg, size_t id, Position &pos);
#ifndef SERVER
void create_ammo(Registry &reg, Position pos, Weapon original_weapon);
void create_ammo(Registry &reg, Position pos, float damage_mult);
#endif
