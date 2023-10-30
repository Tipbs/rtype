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
#endif

struct Ammo {
    #ifndef SERVER
    Sprite sprite = Sprite("./gui/ressources/Sprites/shoot_charge.png");
    #endif
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
    #ifndef SERVER
    Sprite sprite = Sprite("./gui/ressources/Sprites/r-typesheet42.gif");
    #endif

    Weapon(int type, int ammo_per_sec){
        type = type;
        size = Size(1,1);
        attack_speed = 1. / ammo_per_sec;
        #ifndef SERVER
        sprite = Sprite("./gui/ressources/Sprites/r-typesheet42.gif");
        #endif
    };
};

struct Player {
    int color_id = -1;
    double count = 1;
    bool IsShooting = false;
    float current_charge = 0.;
    Weapon weapon = Weapon(1, 5);

    Player(double x) : count(x)
    {
        current_charge = 0.;
        IsShooting = false;
        weapon = Weapon(1, 5);
    };

    Player(int color_i) : color_id(color_i)
    {
        current_charge = 0.;
        count = 1;
        IsShooting = false;
        weapon = Weapon(1, 5);
    };

    Player(double x, int color_i) : color_id(color_i), count(x)
    {
        current_charge = 0.;
        IsShooting = false;
        weapon = Weapon(1, 5);
    };

    Player(int color_i, double x) : color_id(color_i), count(x)
    {
        current_charge = 0.;
        IsShooting = false;
        weapon = Weapon(1, 5);
    };
};

size_t create_player(Registry &reg, size_t id, Position pos);
void create_ammo(Registry &reg, Position pos, Direction dir, float damage_mult, int color_id);
void create_ammo(Registry &reg, Position pos, float damage_mult, int color_id);
size_t create_zorg(Registry &reg, Position pos, size_t net_id);
