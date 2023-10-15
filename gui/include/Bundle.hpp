#pragma once

#include <string>
#include <utility>
#include "../../shared/Registry.hpp"
#include "../../shared/Component.hpp"
#include "GraphicComponents.hpp"

#ifndef SERVER
    #include <raylib.h>
#endif

struct Ammo {
    Sprite sprite = Sprite("./gui/ressources/Sprites/shoot_charge.png");
    Speed speed = Speed(300);
    Damages damages = Damages(1);
    Size size = Size(1,1);
    Health health = Health(1);
    Direction dir = Direction(1,0);
};

struct Weapon {
    int _type = 1;
    Size size = Size(1,1);
    double attack_speed = 1.;
    Sprite sprite = Sprite("./gui/ressources/Sprites/r-typesheet42.gif");

    Weapon(int type, int ammo_per_sec){
        _type = type;
        size = Size(1,1);
        attack_speed = 1. / ammo_per_sec;
        sprite = Sprite("./gui/ressources/Sprites/r-typesheet42.gif");
    };
};

struct Player {
	int color_id = -1;
    int id = -1;
    double count = 1;
    bool IsShooting = false;
    Weapon weapon = Weapon(1, 5);

	Player(double x): count(x) {id = -1;IsShooting = false; weapon = Weapon(1, 5);};
	Player(double x, size_t _id): count(x) {id = _id;IsShooting = false; weapon = Weapon(1, 5);};
	Player(int color_i): color_id(color_i) {id = -1;count = 1;IsShooting = false; weapon = Weapon(1, 5);};
	Player(double x, int color_i): color_id(color_i), count(x) {id = -1;IsShooting = false; weapon = Weapon(1, 5);};
	Player(int color_i, double x): color_id(color_i), count(x) {id = -1;IsShooting = false; weapon = Weapon(1, 5);};
};


size_t create_player(Registry &reg, bool i);
void create_ammo(Registry &reg, Position pos, Weapon original_weapon);
