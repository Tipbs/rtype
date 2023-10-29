#include <chrono>
#include <climits>
#include <cstdio>
#include <iostream>
#include <semaphore>
#include "../../shared/Parser.hpp"
#include "../../shared/Registry.hpp"
#include "Client.hpp"
#include "GraphicComponents.hpp"
#include "GraphicSystems.hpp"
#include "raylib.h"

int main(int ac, char **av)
{
    const int ScreenWidth = 1280;
    const int ScreenHeight = 720;
    boost::asio::io_context context;
    Registry reg;
    std::string port = "5000";
    std::string ip = "127.0.0.1";
    if (ac == 3 && std::stoi(av[2])) {
        ip = av[1];
        port = av[2];
    }
    udp_client net_client(context, ip, port, reg);
    context.run();
    InitWindow(ScreenWidth, ScreenHeight, "R-Type");
    SetTargetFPS(60);

    Factory factory(reg);

    factory.register_components();
    factory.create_background(ScreenWidth, ScreenHeight);
    Entity player =
        factory.create_player(net_client.get_player_id(), Position(0, 0));
    factory.create_weapon(player);
    factory.add_systems();

    while (!WindowShouldClose()) {
        reg.run_systems();
        context.poll();
        context.reset();
    }
}

// int main(void)
// {
//     Registry reg;
//     // Parser parser(reg, "./server/ressources/Maps/map_01.json");
//     // Factory factory(reg);
//     Utils::Vec2 pos = { 50, 50 };
//     // factory.create(1, 1, pos, 1);
//     Entity const new_entity = reg.spawn_entity();
//     reg.register_components<
// #ifndef SERVER
//         Sprite,
//         Weapon,
// #endif
//         Player,
//         Current_Player,
//         Position,
//         Damages,
//         Size,
//         Health,
//         Speed,
//         Direction,
//         SpawnGrace,
//         NetworkedEntity,
//         Animation
//     >();

//     reg.emplace_component<Player>(new_entity);
//     reg.emplace_component<Position>(new_entity, pos);
//     reg.emplace_component<Size>(new_entity, 83, 43);
// #ifndef SERVER
//     reg.emplace_component<Sprite>(new_entity,
//     "./gui/ressources/Sprites/r-typesheet42.gif", 83, 43, 5, 5);
// #endif
//     reg.emplace_component<Speed>(new_entity, 300);
//     reg.emplace_component<Direction>(new_entity, 0, 0);
//     reg.emplace_component<SpawnGrace>(new_entity, std::chrono::seconds(1));
//     reg.emplace_component<Animation>(new_entity, 1);
//     reg.emplace_component<NetworkedEntity>(new_entity, 1);

// }
