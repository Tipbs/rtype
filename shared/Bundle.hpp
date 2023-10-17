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

    Weapon(int type, int ammo_per_sec){
        type = type;
        size = Size(1,1);
        attack_speed = 1. / ammo_per_sec;
        sprite = Sprite("./gui/ressources/Sprites/r-typesheet42.gif");
    };
};

#endif SERVER

struct Player {
	int color_id = -1;
    int id = -1;
    double count = 1;
    bool IsShooting = false;
	#ifndef SERVER
    Weapon weapon = Weapon(1, 5);
    #endif

    Player(size_t id) : id(id) {}
    Player(int x, size_t id) : id(id), count(x) {}
	Player(double x): count(x) {
        id = -1;
        IsShooting = false;
        #ifndef SERVER
			weapon = Weapon(1, 5);
        #endif
    }
	Player(int color_i): color_id(color_i) {
        id = -1;
        count = 1;
        IsShooting = false;
        #ifndef SERVER
        weapon = Weapon(1, 5);
        #endif
    }
	Player(double x, int color_i): color_id(color_i), count(x) {
        id = -1;
        IsShooting = false;
        #ifndef SERVER
        weapon = Weapon(1, 5);
        #endif
    }
	Player(int color_i, double x): color_id(color_i), count(x) {
        id = -1;
        IsShooting = false;
        #ifndef SERVER
        weapon = Weapon(1, 5);
        #endif
    }
};


size_t create_player(Registry &reg, bool i);
size_t create_player(Registry &reg, int id, Position &pos);
#ifndef SERVER
void create_ammo(Registry &reg, Position pos, Weapon original_weapon);
#endif