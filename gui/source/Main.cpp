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
#include <raylib.h>

int main(int ac, char **av)
{
    const int ScreenWidth = 1280;
    const int ScreenHeight = 720;
    //const int ScreenWidth = 900;
    //const int ScreenHeight = 1000;
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
    factory.create_hud(ScreenWidth, ScreenHeight);
    Entity player =
        factory.create_player(net_client.get_player_id(), Position(0, 0));
    factory.create_weapon(player);
    factory.add_systems();
    factory.create_boss(Position(100, 100), 0);

    while (!WindowShouldClose()) {
        reg.run_systems();
        context.poll();
        context.reset();
    }
}
