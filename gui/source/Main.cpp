#include <chrono>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <semaphore>
#include <raylib.h>
#include "../../shared/Parser.hpp"
#include "../../shared/Registry.hpp"
#include "Client.hpp"
#include "GraphicComponents.hpp"
#include "GraphicSystems.hpp"

int main(int ac, char **av)
{
    const int ScreenWidth = 1280;
    const int ScreenHeight = 720;
    // const int ScreenWidth = 900;
    // const int ScreenHeight = 1000;
    boost::asio::io_context context;
    Registry reg;
    std::string port = "5000";
    std::string ip = "127.0.0.1";
    if (ac == 3 && std::stoi(av[2])) {
        ip = av[1];
        port = av[2];
    }
    udp_client net_client(context, reg);
    InitWindow(ScreenWidth, ScreenHeight, "R-Type");
    InitAudioDevice();
    SetTargetFPS(60);

    Factory factory(reg);

    factory.register_components();
    factory.create_background(ScreenWidth, ScreenHeight);
    factory.create_game(net_client, ip, port, ScreenWidth, ScreenHeight);

    factory.add_systems();

    while (!WindowShouldClose()) {
        reg.run_systems();
        context.poll();
        context.reset();
    }
}
