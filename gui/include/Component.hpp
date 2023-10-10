#include <raylib.h>
#include <string>
#include <utility>

#ifndef COMPONENT_HPP
    #define COMPONENT_HPP

    struct Player {
        int id = 0;
        Player(): id(0) {};
        Player(int x): id(x) {};
    };

    struct Position {
        double pos_X = 0;
        double pos_Y = 0;
        Position(double x, double y): pos_X(x), pos_Y(y) {};
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
        double speed = 0;
        Speed(double x): speed(x) {};
    };
    struct Direction {
        double dir_X = 0;
        double dir_Y = 0;
        Direction(double x, double y): dir_X(x), dir_Y(y) {};
    };
    struct SpawnGrace {
        double timer = 0;
        double creation_time = 0;
        SpawnGrace(double x, double y): timer(x), creation_time(y) {};
        SpawnGrace(double timer): timer(timer) {
            creation_time = GetTime();
        };
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
        double attack_speed;
        Animation sprite;
        Ammo ammo;
    };
#endif