#pragma once

#include <string>
#include <utility>
#include <raylib.h>
#include "Registry.hpp"
#include "Component.hpp"
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

    Weapon(int type, int ammo_per_sec){
        type = type;
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
    float current_charge = 0.;
    Weapon weapon = Weapon(1, 5);

	Player(double x): count(x) {current_charge = 0.;id = -1;IsShooting = false; weapon = Weapon(1, 5);};
	Player(int color_i): color_id(color_i) {current_charge = 0.;id = -1;count = 1;IsShooting = false; weapon = Weapon(1, 5);};
	Player(double x, int color_i): color_id(color_i), count(x) {current_charge = 0.;id = -1;IsShooting = false; weapon = Weapon(1, 5);};
	Player(int color_i, double x): color_id(color_i), count(x) {current_charge = 0.;id = -1;IsShooting = false; weapon = Weapon(1, 5);};
};


size_t create_player(Registry &reg, bool i);
void create_ammo(Registry &reg, Position pos, float current_charge);