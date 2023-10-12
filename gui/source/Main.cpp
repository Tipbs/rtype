#include <cstdio>
#include <iostream>
#include <utility>
#include <semaphore>
#include "../include/Registry.hpp"
#include "../include/GraphicComponents.hpp"
#include "GraphicSystems.hpp"
#include "Sparse_array.hpp"
#include "Zipper.hpp"
#include "raylib.h"

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
    Sprite bgsprite(bgpath.c_str(), ScreenWidth, ScreenHeight);

    Entity const new_entity = reg.spawn_entity();
    Position nePos(0, 0);
    Size neSize(83, 43);
    std::string nepath = "./gui/ressources/Sprites/r-typesheet42.gif";
    Speed speedo(300);
    Direction diro(50, 0);
    SpawnGrace gra(5);
    Sprite nesprite(nepath.c_str(), 83, 43, 5, 5);
    MoveAnimCounter play0count(1);

    reg.register_component<Size>();
    reg.register_component<Position>();
    reg.register_component<Sprite>();
    reg.register_component<Speed>();
    reg.register_component<Direction>();
    reg.register_component<SpawnGrace>();
    reg.register_component<MoveAnimCounter>();

    reg.add_component(background, std::move(bgPos));
    reg.add_component(background, std::move(bgSize));
    reg.add_component(background, std::move(bgsprite));

    reg.add_component(new_entity, std::move(nePos));
    reg.add_component(new_entity, std::move(neSize));
    reg.add_component(new_entity, std::move(nesprite));
    reg.add_component(new_entity, std::move(speedo));
    reg.add_component(new_entity, std::move(diro));
    reg.add_component(new_entity, std::move(gra));
    reg.add_component(new_entity, std::move(play0count));


    reg.add_system<Position, Size, SpawnGrace>(colision);
    reg.add_system<Position, Speed, Direction>(move);
    reg.add_system<Position, Size, Sprite, MoveAnimCounter>(display);
    reg.add_system<Direction, MoveAnimCounter, Sprite>(handle_dir_inputs);
    while (!WindowShouldClose()) {
        reg.run_systems();
    }
}
