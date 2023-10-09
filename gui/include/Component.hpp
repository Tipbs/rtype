#include <raylib.h>
#include <string>
#include <utility>

#ifndef COMPONENT_HPP
    #define COMPONENT_HPP

    struct Position {
        int pos_X = 0;
        int pos_Y = 0;
        Position(int x, int y): pos_X(x), pos_Y(y) {};
    };
    struct Damages {
        int damages = 0;
        Damages(int x): damages(x) {};
    };
    struct Size {
        int size_X = 1;
        int size_Y = 1;
        Size(int x, int y): size_X(x), size_Y(y) {};
    };
    struct Health {
        int health = 1;
        Health(int x): health(x) {};
    };
    struct Speed {
        float speed = 0.0f;
        Speed(float x): speed(x) {};
    };
    struct Direction {
        float dir_X = 0.0f;
        float dir_Y = 0.0f;
        Direction(float x, float y): dir_X(x), dir_Y(y) {};
    };
    struct SpawnGrace {
        float timer = 0;
        float creation_time = 0;
        SpawnGrace(float x, float y): timer(x), creation_time(y) {};
    };
    struct Sprite {
        Texture2D sprite;

        Sprite(char *path, int w, int h) {
            Image sprit = LoadImage(path);
            ImageResize(&sprit, w, h);
            sprite = LoadTextureFromImage(sprit);
            };

        Sprite(const char *path, int w, int h) {
            Image sprit = LoadImage(path);
            ImageResize(&sprit, w, h);
            sprite = LoadTextureFromImage(sprit);
            };

        Sprite(const char *path): sprite(LoadTextureFromImage(LoadImage(path))) {};
        Sprite(char *path): sprite(LoadTextureFromImage(LoadImage(path))) {};
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