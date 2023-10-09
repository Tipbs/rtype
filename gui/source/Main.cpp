#include <cstdio>
#include <iostream>
#include <utility>
#include <semaphore>
#include "../include/Registry.hpp"
#include "../include/Component.hpp"
#include "../include/Systems.hpp"
#include "../include/Sparse_array.hpp"
#include "../include/Zipper.hpp"
#include "raylib.h"

struct Velocity {
    float vx, vy;
};

struct Drawable {
    int color;
};

struct Controllable {
    bool key_pressed[256];
};

// void redirect_all_stdout(void)
// {
//     cr_redirect_stdout();
//     cr_redirect_stderr();
// }

void print_position(Registry &r, sparse_array<Position> positions)
{
    for (auto pos : positions)
        std::cout << "Position = { " << pos->pos_X << ", " << pos->pos_Y << " }"
                  << std::endl;
}

int main()
{
    const int ScreenWidth = 1280;
    const int ScreenHeight = 720;
    InitWindow(ScreenWidth, ScreenHeight, "R-Type");
    SetTargetFPS(144);

    Image bg = LoadImage("ressources/Backgrounds/Back.png");
    ImageResize(&bg, ScreenWidth, ScreenHeight);
    Texture2D bgText = LoadTextureFromImage(bg);

    Registry reg;

    Entity const entity = reg.spawn_entity();
    Position pos(1, 5);
    Speed spe(5);
    Direction dir(6, 6);

    Entity const background = reg.spawn_entity();
    Position bgPos(0, 0);
    Size bgSize(ScreenWidth, ScreenHeight);
    Sprite bgsprite("../ressources/Backgrounds/Back.png")

    reg.register_component<Position>();
    reg.register_component<Speed>();
    reg.register_component<Direction>();
    reg.add_component(entity, std::move(pos));
    reg.add_component(entity, std::move(spe));
    reg.add_component(entity, std::move(dir));

    reg.register_component<Size>();
    reg.register_component<Sprite>();
    reg.add_component(background, std::move(bgPos));
    reg.add_component(background, std::move(bgSize));
    reg.add_component(background, std::move(bgsprite));
    // reg.emplace_component<Position>(entity, 1, 2);
    reg.add_system<Position, Speed, Direction>(&move);
    reg.add_system<Position, Size, Sprite>(&display);
    reg.run_systems();
}
