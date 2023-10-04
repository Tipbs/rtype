//#include "raylib.h"

#ifndef COMPONENT_HPP
    #define COMPONENT_HPP

    struct Position {
        int pos_X = 0;
        int pos_Y = 0;
    };
    struct Damages {
        int damages = 0;
    };
    struct Size {
        int size_X = 1;
        int size_Y = 1;
    };
    struct Health {
        int health = 1;
    };
    struct Speed {
        float speed = 0;
    };
    struct Direction {
        float dir_X = 0;
        float dir_Y = 0;
    };
    struct SpawnGrace {
        float timer = 0;
        float creation_time = 0;
    };
    struct Sprite {
        // Texutre2D sprite;
    };
    struct Animation {
        // Rectangle rect; //contains size as well
        Sprite sprite;
        Position* frames; // contains a list of position for each frames of an animated picture.
    };
    struct Ammo {
        Animation sprite;
        Speed speed;
        Damages damages;
        Size size;
        Health health;
        Direction dir;
    };
    struct Weapon {
        int type;
        Size size;
        float attack_speed;
        Animation sprite;
        Ammo ammo;
    };
#endif