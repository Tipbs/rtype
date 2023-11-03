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

void create_sounds(Registry &reg)
{
    Entity sounds = reg.spawn_entity();
    reg.emplace_component<SoundManager>(sounds);
    reg.emplace_component<MusicComponent>(
        sounds, "./gui/ressources/Audio/battle_ost.mp3", MusicFx::Battle);
}

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
    udp_client net_client(context, ip, port, reg);
    context.run();
    InitWindow(ScreenWidth, ScreenHeight, "R-Type");
    InitAudioDevice();
    SetTargetFPS(60);

    Factory factory(reg);

    factory.register_components();
    factory.create_background(ScreenWidth, ScreenHeight);
    auto net_player_info = net_client.get_player_id();
    Entity player =
        factory.create_player(net_player_info.pos, net_player_info.id);
    reg.emplace_component<Current_Player>(player);
    std::cout << "player pos id: " << net_player_info.id << std::endl;
    std::cout << "player pos x: " << net_player_info.pos.x << std::endl;
    std::cout << "player pos y: " << net_player_info.pos.y << std::endl;
    Entity weapon = factory.create_weapon(player);
    factory.create_hud(ScreenWidth, ScreenHeight, player, weapon);
    create_sounds(reg);
    factory.add_systems();

    while (!WindowShouldClose()) {
        reg.run_systems();
        context.poll();
        context.reset();
    }
}
/*
    Entity const background = reg.spawn_entity();
    Position bgPos(0, 0);
    Size bgSize(ScreenWidth, ScreenHeight);
    std::string bgpath =
        "./gui/ressources/Backgrounds/Backtest.png"; // temp > 2 > 3 > 1
    Speed bgspe(200);
    Direction bgdir(-4, 0);
    Sprite bgsprite(bgpath.c_str(), 3 * ScreenWidth, ScreenHeight);

    Entity const new_entity = reg.spawn_entity();
    Player player((size_t) new_entity % 5, net_client.get_player_id());
    Position nePos(0, 0);
    Size neSize(83, 43);
    std::string nepath = "./gui/ressources/Sprites/r-typesheet42.gif";
    Speed speedo(300);
    Direction diro(0, 0);
    SpawnGrace gra(std::chrono::seconds(5));
    Sprite nesprite(nepath.c_str(), 83, 43, 5, 5);

    Entity const menuFields = reg.spawn_entity();
    reg.emplace_component<MenuFields>(menuFields);
    reg.emplace_component<Rectangle>(menuFields, ScreenWidth / 2.0f - 200, 180, 400, 50);

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
    reg.register_component<MenuFields>();
    reg.register_component<Rectangle>();

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
    reg.add_component(new_entity, std::move(player));

    reg.add_system<Position, Size, SpawnGrace, Damages, Health>(colision);
    reg.add_system<Position, Speed, Direction>(move);
    reg.add_system<Position, Size, Sprite, Player, Rectangle, InputField>(
        display);
    reg.add_system<Direction, Player, Sprite>(handle_dir_inputs);
    reg.add_system<Player, Position, Size>(handle_shoot_inputs);
    //    reg.add_system<InputField, Rectangle>(hadle_text_inputs);
    reg.add_system<Position, Size>(make_infinite_background);
    reg.add_system<Position, Player>(updateWithSnapshots);
    reg.add_system<MenuFields, Rectangle>(hadle_menu_inputs);
*/