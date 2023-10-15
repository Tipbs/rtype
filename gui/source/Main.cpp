#include <climits>
#include <cstdio>
#include <iostream>
#include <semaphore>
#include "../../shared/Bundle.hpp"
#include "../../shared/Registry.hpp"
#include "../include/GraphicComponents.hpp"
#include "GraphicSystems.hpp"
#include "raylib.h"

void logging_system(Registry &r, sparse_array<Position> const &positions)
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
    SetTargetFPS(10);

    Registry reg;

    Entity const background = reg.spawn_entity();
    SpawnGrace bgGrace(INT_MAX);
    Position bgPos(0, 0);
    Size bgSize(ScreenWidth, ScreenHeight);
    std::string bgpath =
        "./gui/ressources/Backgrounds/Back1bis.png"; // 2 > 3 > 1
    Speed bgspe(200);
    Direction bgdir(-4, -1);
    Sprite bgsprite(bgpath.c_str(), 2 * ScreenWidth, 2 * ScreenHeight);

    reg.register_component<Size>();
    reg.register_component<Position>();
    reg.register_component<Sprite>();
    reg.register_component<Speed>();
    reg.register_component<Direction>();
    reg.register_component<SpawnGrace>();
    reg.register_component<Player>();
    reg.register_component<Health>();
    reg.register_component<Damages>();
    reg.register_component<Current_Player>();

    reg.add_component(background, std::move(bgPos));
    reg.add_component(background, std::move(bgSize));
    reg.add_component(background, std::move(bgsprite));
    reg.add_component(background, std::move(bgspe));
    reg.add_component(background, std::move(bgdir));
    reg.add_component(background, std::move(bgGrace));

    create_player(reg, true);

    reg.add_system<Position, Size, SpawnGrace, Damages, Health>(colision);
    reg.add_system<Position, Speed, Direction>(move);
    reg.add_system<Position, Size, Sprite, Player>(display);
    reg.add_system<Direction, Player, Sprite>(handle_dir_inputs);
    reg.add_system<Player, Position, Size>(handle_shoot_inputs);
    reg.add_system<Position, Size>(make_infinite_background);
    while (!WindowShouldClose())
        reg.run_systems();
}
