#include <chrono>
#include <climits>
#include <cstdio>
#include <iostream>
#include <semaphore>
#include "../../shared/Bundle.hpp"
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

    Entity const background = reg.spawn_entity();
    Position bgPos(0, 0);
    Size bgSize(ScreenWidth, ScreenHeight);
    std::string bgpath =
        "./gui/ressources/Backgrounds/Backtest.png"; // temp > 2 > 3 > 1
    Speed bgspe(50);
    Direction bgdir(-4, 0);
    Sprite bgsprite(bgpath.c_str(), 3 * ScreenWidth, ScreenHeight);

    Position nePos(0, 0);

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
    reg.register_component<InputField>();
    reg.register_component<Rectangle>();
    reg.register_component<NetworkedEntity>();
    reg.register_component<AlwaysShoot>();
    auto current_player = create_player(reg, net_client.get_player_id(), nePos);
    Current_Player current_p;
    create_zorg(reg, Position(1200, 50), 5);
    create_zorg(reg, Position(1300, 300), 5);

    reg.add_component(background, std::move(bgPos));
    reg.add_component(background, std::move(bgSize));
    reg.add_component(background, std::move(bgsprite));
    reg.add_component(background, std::move(bgspe));
    reg.add_component(background, std::move(bgdir));

    reg.add_component(current_player, std::move(current_p));

    reg.add_system<Position, Size, SpawnGrace, Damages, Health>(colision);
    reg.add_system<Position, Speed, Direction>(move);
    reg.add_system<Position, Size, Sprite, Player, Rectangle, InputField>(
        display);
    reg.add_system<Direction, Player, Sprite, Speed, Current_Player>(handle_dir_inputs);
    reg.add_system<Player, Position, Size, Current_Player>(handle_shoot_inputs);
    //    reg.add_system<InputField, Rectangle>(hadle_text_inputs);
    reg.add_system<Position, Size>(make_infinite_background);
    reg.add_system<AlwaysShoot, Position, Size>(enemyAlwaysShoot);
    reg.add_system<Position, NetworkedEntity, Speed, Current_Player, Size, Player>(updateWithSnapshots);

    while (!WindowShouldClose()) {
        reg.run_systems();
        context.poll();
        context.reset();
    }
}
