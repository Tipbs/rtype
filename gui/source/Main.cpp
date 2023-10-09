#include <cstdio>
#include <iostream>
#include <utility>
#include <semaphore>
#include <format>
#include "Registry.hpp"
#include "Component.hpp"
#include "Systems.hpp"
#include "Sparse_array.hpp"
#include "Zipper.hpp"
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

void logging_system(
    Registry &r, sparse_array<Position> const &positions)
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

    Registry reg;

    Entity const background = reg.spawn_entity();
    Position bgPos(0, 0);
    Size bgSize(ScreenWidth, ScreenHeight);
    std::string bgpath = "./gui/ressources/Backgrounds/Back.png";
    std::cout << "PATH : " << std::filesystem::current_path() << std::endl;
    Sprite bgsprite(bgpath.c_str(), ScreenWidth, ScreenHeight);

    reg.register_component<Size>();
    reg.register_component<Position>();
    reg.register_component<Sprite>();
    reg.add_component(background, std::move(bgPos));
    reg.add_component(background, std::move(bgSize));
    reg.add_component(background, std::move(bgsprite));
    reg.add_system<Position, Size, Sprite>(&display);
    while (!WindowShouldClose()) {
        reg.run_systems();
    }
    
}
