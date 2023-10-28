#include <chrono>
#include <climits>
#include <cstdio>
#include <iostream>
#include <semaphore>
#include "../../shared/Bundle.hpp"
#include "../../shared/Registry.hpp"
#include "../../shared/Parser.hpp"
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

    reg.register_component<Player>();
    reg.register_component<Weapon>();
    reg.register_component<Size>();
    reg.register_component<Position>();
    reg.register_component<Sprite>();
    reg.register_component<Speed>();
    reg.register_component<Direction>();
    reg.register_component<SpawnGrace>();
    reg.register_component<Animation>();
    reg.register_component<Health>();
    reg.register_component<Damages>();
    reg.register_component<Current_Player>();
    reg.register_component<InputField>();
    reg.register_component<Rectangle>();
    reg.register_component<NetworkedEntity>();
    auto current_player = create_player(reg, net_client.get_player_id(), nePos);
    Current_Player current_p;
      reg.add_component(current_player, std::move(current_p));

    Entity weapon = reg.spawn_entity();
    reg.emplace_component<Weapon>(weapon, current_player);
    reg.emplace_component<Position>(weapon);

    reg.add_system<SpawnGrace>(update_grace);
    reg.add_system<Position, Size, SpawnGrace, Damages, Health>(colision);
    reg.add_system<Position, Speed, Direction>(move);
    reg.add_system<Position, Size, Sprite, Player, Rectangle, InputField>(
        display);
    reg.add_system<Direction, Player, Sprite, Speed, Current_Player, Animation>(
        handle_dir_inputs);
    reg.add_system<Player, Size, Weapon, Position>(handle_shoot_inputs);
    //    reg.add_system<InputField, Rectangle>(hadle_text_inputs);
    reg.add_system<Position, Size>(make_infinite_background);
    reg.add_system<
        Position, NetworkedEntity, Speed, Current_Player, Size, Player>(
        updateWithSnapshots);
}


void logging_system(Registry &r, sparse_array<Position> const &positions)
{
    for (auto &pos : positions)
        std::cout << "Position = { " << pos->pos_X << ", " << pos->pos_Y << " }"
                  << std::endl;
}

// int main(int ac, char **av)
// {
//     const int ScreenWidth = 1280;
//     const int ScreenHeight = 720;
//     boost::asio::io_context context;
//     Registry reg;
//     std::string port = "5000";
//     std::string ip = "127.0.0.1";
//     if (ac == 3 && std::stoi(av[2])) {
//         ip = av[1];
//         port = av[2];
//     }
//     udp_client net_client(context, ip, port, reg);
//     context.run();
//     InitWindow(ScreenWidth, ScreenHeight, "R-Type");
//     SetTargetFPS(60);

//     Entity const background = reg.spawn_entity();
//     Position bgPos(0, 0);
//     Size bgSize(ScreenWidth, ScreenHeight);
//     std::string bgpath =
//         "./gui/ressources/Backgrounds/Backtest.png"; // temp > 2 > 3 > 1
//     Speed bgspe(200);
//     Direction bgdir(-4, 0);
//     Sprite bgsprite(bgpath.c_str(), 3 * ScreenWidth, ScreenHeight);

//     Entity const new_entity = reg.spawn_entity();
//     Player player((size_t) new_entity % 5, net_client.get_player_id());
//     Position nePos(0, 0);
//     Size neSize(83, 43);
//     std::string nepath = "./gui/ressources/Sprites/r-typesheet42.gif";
//     Speed speedo(300);
//     Direction diro(0, 0);
//     SpawnGrace gra(std::chrono::seconds(5));
//     Sprite nesprite(nepath.c_str(), 83, 43, 5, 5);

//     reg.register_component<Size>();
//     reg.register_component<Position>();
//     reg.register_component<Sprite>();
//     reg.register_component<Speed>();
//     reg.register_component<Direction>();
//     reg.register_component<SpawnGrace>();
//     reg.register_component<Player>();
//     reg.register_component<Health>();
//     reg.register_component<Damages>();
//     reg.register_component<Current_Player>();
//     reg.register_component<InputField>();
//     reg.register_component<Rectangle>();

//     reg.add_component(background, std::move(bgPos));
//     reg.add_component(background, std::move(bgSize));
//     reg.add_component(background, std::move(bgsprite));
//     reg.add_component(background, std::move(bgspe));
//     reg.add_component(background, std::move(bgdir));

//     reg.add_component(new_entity, std::move(nePos));
//     reg.add_component(new_entity, std::move(neSize));
//     reg.add_component(new_entity, std::move(nesprite));
//     reg.add_component(new_entity, std::move(speedo));
//     reg.add_component(new_entity, std::move(diro));
//     reg.add_component(new_entity, std::move(gra));
//     reg.add_component(new_entity, std::move(player));

//     reg.add_system<Position, Size, SpawnGrace, Damages, Health>(colision);
//     reg.add_system<Position, Speed, Direction>(move);
//     reg.add_system<Position, Size, Sprite, Player, Rectangle, InputField>(
//         display);
//     reg.add_system<Direction, Player, Sprite, Speed>(handle_dir_inputs);
//     reg.add_system<Player, Position, Size>(handle_shoot_inputs);
//     //    reg.add_system<InputField, Rectangle>(hadle_text_inputs);
//     reg.add_system<Position, Size>(make_infinite_background);
//     reg.add_system<Position, Player, Speed>(updateWithSnapshots);

//     while (!WindowShouldClose()) {
//         reg.run_systems();
//         context.poll();
//         context.reset();
//     }
// }

int main(void)
{
    Registry reg;
    Parser parser(reg, "./server/ressources/Maps/map_01.json");
}
