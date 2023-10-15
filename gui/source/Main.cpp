#include <cstdio>
#include <iostream>
#include <semaphore>
#include "../../shared/Bundle.hpp"
#include "../../shared/Registry.hpp"
#include "GraphicComponents.hpp"
#include "GraphicSystems.hpp"
#include "Client.hpp"
#include "raylib.h"

void logging_system(Registry &r, sparse_array<Position> const &positions)
{
    for (auto &pos : positions)
        std::cout << "Position = { " << pos->pos_X << ", " << pos->pos_Y << " }"
                  << std::endl;
}

int main()
{
    const int ScreenWidth = 1280;
    const int ScreenHeight = 720;
    boost::asio::io_context context;
    Registry reg;
    udp_client net_client(context, "127.0.0.1", "5000", reg);
    context.run();
    InitWindow(ScreenWidth, ScreenHeight, "R-Type");
    SetTargetFPS(300);


    Entity const background = reg.spawn_entity();
    Position bgPos(0, 0);
    Size bgSize(ScreenWidth, ScreenHeight);
    Sprite bgsprite(bgpath.c_str(), ScreenWidth, ScreenHeight);
    std::string bgpath =
        "./gui/ressources/Backgrounds/Back1bis.png"; // 2 > 3 > 1
    Speed bgspe(200);
    Direction bgdir(-4, -1);
    Sprite bgsprite(bgpath.c_str(), 2 * ScreenWidth, 2 * ScreenHeight);

    Entity const new_entity = reg.spawn_entity();
    Player p(net_client.get_player_id());
    Position nePos(0, 0);
    Size neSize(83, 43);
    std::string nepath = "./gui/ressources/Sprites/r-typesheet42.gif";
    Speed speedo(300);
    Direction diro(50, 0);
    SpawnGrace gra(std::chrono::seconds(5));
    Sprite nesprite(nepath.c_str(), 83, 43, 5, 5);
    MoveAnimCounter play0count(1);

    reg.register_component<Size>();
    reg.register_component<Position>();
    reg.register_component<Sprite>();
    reg.register_component<Speed>();
    reg.register_component<Direction>();
    reg.register_component<SpawnGrace>();
    reg.register_component<MoveAnimCounter>();
    reg.register_component<Player>();
    reg.register_component<Current_Player>();

    reg.add_component(background, std::move(bgPos));
    reg.add_component(background, std::move(bgSize));
    reg.add_component(background, std::move(bgsprite));
    reg.add_component(background, std::move(bgspe));
    reg.add_component(background, std::move(bgdir));

    reg.add_component(new_entity, std::move(nePos));
    reg.add_component(new_entity, std::move(neSize));
    reg.add_component(new_entity, std::move(nesprite));
    reg.add_component(new_entity, std::move(speedo));
    reg.add_component(new_entity, std::move(diro));
    reg.add_component(new_entity, std::move(gra));
    reg.add_component(new_entity, std::move(play0count));
    reg.add_component(new_entity, std::move(p));

    reg.add_system<Position, Size, SpawnGrace>(colision);
    reg.add_system<Position, Speed, Direction>(move);
    reg.add_system<Position, Size, Sprite, MoveAnimCounter>(display);
    reg.add_system<Direction, MoveAnimCounter, Sprite>(handle_dir_inputs);
    reg.add_system<Player, Position, Size>(handle_shoot_inputs);
    reg.add_system<Position, Size>(make_infinite_background);
    reg.add_system<Position, Player>(updateWithSnapshots);

    while (!WindowShouldClose()) {
        reg.run_systems();
        context.poll();
        context.reset();
    }
}
