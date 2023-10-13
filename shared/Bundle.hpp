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
	int id = 0;
    double count = 1;
    bool IsShooting = false;
    Weapon weapon = Weapon(1, 5);

	Player(double x): count(x) {IsShooting = false; weapon = Weapon(1, 5);};
	Player(int x): id(x) {count = 1;IsShooting = false; weapon = Weapon(1, 5);};
	Player(double x, int i): id(i), count(x) {IsShooting = false; weapon = Weapon(1, 5);};
	Player(int i, double x): id(i), count(x) {IsShooting = false; weapon = Weapon(1, 5);};
};


void create_player(Registry &reg, int id);
void create_ammo(Registry &reg, Position pos, Weapon original_weapon);