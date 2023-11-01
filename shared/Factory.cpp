#include "Factory.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include "Systems.hpp"
#include <chrono>
#include <stdlib.h>     /* srand, rand */

#ifndef SERVER
    #include <raylib.h>
    #include "../gui/include/GraphicComponents.hpp"
    #include "../gui/include/GraphicSystems.hpp"
#endif


void Factory::register_components()
{
    _reg.register_components<
#ifndef SERVER
        Sprite,
        Weapon,
        InputField,
        Rectangle,
        // HUD,
        Rect,
        Color,
        Text,
#endif
        Player,
        Current_Player,
        Position,
        Damages,
        Size,
        Health,
        Speed,
        Direction,
        SpawnGrace,
        NetworkedEntity,
        Animation,
        Couleur,
        Backgrounds
    >();
}

void Factory::add_systems()
{
    _reg.add_system<SpawnGrace>(update_grace);
    _reg.add_system<Position, Size, SpawnGrace, Damages, Health>(colision);
    _reg.add_system<Position, Speed, Direction>(move);
#ifndef SERVER
    _reg.add_system<Position, Size, Sprite, Player, Rectangle, InputField, Rect, Color, Text>(
        display);
    _reg.add_system<Direction, Player, Sprite, Speed, Couleur>(
        handle_dir_inputs);
    _reg.add_system<Couleur, Size, Weapon, Position>(handle_shoot_inputs);
    //    _reg.add_system<InputField, Rectangle>(hadle_text_inputs);
    _reg.add_system<
        Position, NetworkedEntity, Speed, Current_Player, Size, Player>(
        updateWithSnapshots);
    _reg.add_system<Sprite, Couleur>(
        do_animation);
    _reg.add_system<Sprite, Couleur, Weapon>(
        do_ship_animation);
    _reg.add_system<Position, Size, Backgrounds>(
        make_infinite_background);
    // _reg.add_system<Weapon, Couleur, HUD>(
    //     updateHUD);

#else

#endif
}

Factory::Factory(Registry &reg) : _reg(reg)
{
}

const Entity Factory::create_background(const int ScreenWidth, const int ScreenHeight)
{
    Entity const background = _reg.spawn_entity();
    std::string bgpath = "./gui/ressources/Backgrounds/Backtest.png"; // temp > 2 > 3 > 1

    _reg.emplace_component<Position>(background, 0, 0);
    _reg.emplace_component<Size>(background, ScreenWidth, ScreenHeight);
#ifndef SERVER
    _reg.emplace_component<Sprite>(background, bgpath.c_str(), 3 * ScreenWidth, ScreenHeight);
#endif
    _reg.emplace_component<Speed>(background, 2);
    _reg.emplace_component<Direction>(background, -1, 0);
    _reg.emplace_component<Backgrounds>(background);
    return background;
}

const Entity Factory::create_player(int id, Position pos)
{
    Entity const new_entity = _reg.spawn_entity();
    std::string const path = "./gui/ressources/Sprites/Testships.png";

    _reg.emplace_component<Player>(new_entity);
    _reg.emplace_component<Position>(new_entity, pos);
    _reg.emplace_component<Size>(new_entity, 83, 43);
#ifndef SERVER
    _reg.emplace_component<Sprite>(new_entity, path.c_str(), 83, 43, 2, 5);
#endif
    _reg.emplace_component<Speed>(new_entity, 5);
    _reg.emplace_component<Direction>(new_entity, 0, 0);
    _reg.emplace_component<SpawnGrace>(new_entity, std::chrono::seconds(1));
    _reg.emplace_component<Animation>(new_entity);
    _reg.emplace_component<Couleur>(new_entity, 0);
    _reg.emplace_component<NetworkedEntity>(new_entity, id);

    return new_entity;
}

const Entity Factory::create_weapon(Entity owner)
{
    Entity weapon = _reg.spawn_entity();

    _reg.emplace_component<Weapon>(weapon, owner);
    _reg.emplace_component<Position>(weapon);
    return weapon;
}

const Entity Factory::create_enemy()
{
    const Entity ent = _reg.spawn_entity();
    float randomNumber = rand() % 100 + 1;
    Utils::Vec2 pos = {900, randomNumber};

    _reg.emplace_component<Position>(ent, pos);
    _reg.emplace_component<Speed>(ent, 5);
    _reg.emplace_component<Direction>(ent, 50, 0);
    _reg.emplace_component<SpawnGrace>(ent, std::chrono::seconds(5));
    // _reg.emplace_component<NetworkEntity>(ent, id);
    return ent;
}

const Entity Factory::create_ammo(Position pos, float damage_mult, int color_id)
{
    Entity const new_entity = _reg.spawn_entity();
    int hitwidth = 120 * (damage_mult / 2);
    int hitheight = 24 * (damage_mult / 2);

    _reg.emplace_component<Position>(new_entity, pos.pos_X - (float) hitwidth / 2, pos.pos_Y - (float) hitheight / 2);
    _reg.emplace_component<Size>(new_entity, hitwidth, hitheight);
    #ifndef SERVER
    _reg.emplace_component<Sprite>(new_entity, "./gui/ressources/Sprites/shoot_ammo.png", hitwidth, hitheight, 8, 5);
    #endif
    _reg.emplace_component<Speed>(new_entity, 7.5);
    _reg.emplace_component<Direction>(new_entity, 1, 0);
    _reg.emplace_component<Damages>(new_entity, damage_mult);
    _reg.emplace_component<Health>(new_entity, 1);
    _reg.emplace_component<Animation>(new_entity);
    _reg.emplace_component<Couleur>(new_entity, color_id);
    return new_entity;
}

const Entity Factory::create_zorg(Registry &reg, Position pos, size_t net_id)
{
    Entity const new_entity = reg.spawn_entity();
    Size Size(48, 48);
    Speed speedo(300);
    Direction diro(-0.40, 0);
    SpawnGrace gra(std::chrono::seconds(1));
    #ifndef SERVER
    std::string path = "./gui/ressources/Sprites/Drone.png";
    Sprite sprite(path.c_str(), 1.5, 1.5);
    #endif

    reg.add_component(new_entity, std::move(pos));
    reg.add_component(new_entity, std::move(Size));
    #ifndef SERVER
    reg.add_component(new_entity, std::move(sprite));
    #endif
    reg.add_component(new_entity, std::move(speedo));
    reg.add_component(new_entity, std::move(diro));
    reg.emplace_component<AlwaysShoot>(
        new_entity, std::chrono::milliseconds(750));
    reg.emplace_component<SpawnGrace>(new_entity, std::chrono::seconds(1));
    reg.emplace_component<Health>(new_entity, 1);
    //reg.emplace_component<NetworkedEntity>(new_entity, net_id);

    return (size_t) new_entity;
}

#ifndef SERVER
void Factory::create_hud(const int ScreenWidth, const int ScreenHeight)
{
    float PosWidth = 0;
    float PosHeight = 9. * ScreenHeight / 10.;
    float SizWidth = ScreenWidth;
    float SizHeight = ScreenHeight / 10.;

    //DATA A RENDRE DYNAMIQUES

    Color play1color = BLUE;
    Color play2color = MAGENTA;
    Color play3color = RED;
    Color play4color = ORANGE;
    float multiplier = 0.65;
    std::string score = "42";

    //FIN DES DATA A RENDRE DYNAMIQUE

    Entity const hudBlackLayer = _reg.spawn_entity();
    _reg.emplace_component<Rect>(hudBlackLayer, false, Rectangle{PosWidth, PosHeight, SizWidth, SizHeight});
    _reg.emplace_component<Color>(hudBlackLayer, BLACK);

    Entity const hudPlay1Rect = _reg.spawn_entity();
    _reg.emplace_component<Rect>(hudPlay1Rect, false, Rectangle{PosWidth + MeasureText("Charge : ", 32), PosHeight, (SizWidth / 2) * multiplier, (SizHeight / 2)});
    _reg.emplace_component<Color>(hudPlay1Rect, play1color);

    Entity const hudPlay1RectLines = _reg.spawn_entity();
    _reg.emplace_component<Rect>(hudPlay1RectLines, true, Rectangle{PosWidth + MeasureText("Charge : ", 32), PosHeight, (SizWidth / 2), (SizHeight / 2)});
    _reg.emplace_component<Color>(hudPlay1RectLines, WHITE);


    float Side_Bar = PosWidth + MeasureText("Charge : ", 32) + (SizWidth / 2.);

    Entity const hudPlay2Rect = _reg.spawn_entity();
    _reg.emplace_component<Rect>(hudPlay2Rect, false, Rectangle{Side_Bar, PosHeight, (SizWidth - Side_Bar) * multiplier, SizHeight / 3});
    _reg.emplace_component<Color>(hudPlay2Rect, play2color);

    Entity const hudPlay2RectLines = _reg.spawn_entity();
    _reg.emplace_component<Rect>(hudPlay2RectLines, true, Rectangle{Side_Bar, PosHeight, (SizWidth - Side_Bar), SizHeight / 3});
    _reg.emplace_component<Color>(hudPlay2RectLines, WHITE);

    Entity const hudPlay3Rect = _reg.spawn_entity();
    _reg.emplace_component<Rect>(hudPlay3Rect, false, Rectangle{Side_Bar, PosHeight + (SizHeight / 3), (SizWidth - Side_Bar) * multiplier, SizHeight / 3});
    _reg.emplace_component<Color>(hudPlay3Rect, play3color);

    Entity const hudPlay3RectLines = _reg.spawn_entity();
    _reg.emplace_component<Rect>(hudPlay3RectLines, true, Rectangle{Side_Bar, PosHeight + (SizHeight / 3), (SizWidth - Side_Bar), SizHeight / 3});
    _reg.emplace_component<Color>(hudPlay3RectLines, WHITE);

    Entity const hudPlay4Rect = _reg.spawn_entity();
    _reg.emplace_component<Rect>(hudPlay4Rect, false, Rectangle{Side_Bar, PosHeight + 2 * (SizHeight / 3), (SizWidth - Side_Bar) * multiplier, SizHeight / 3});
    _reg.emplace_component<Color>(hudPlay4Rect, play4color);

    Entity const hudPlay4RectLines = _reg.spawn_entity();
    _reg.emplace_component<Rect>(hudPlay4RectLines, true, Rectangle{Side_Bar, PosHeight + 2 * (SizHeight / 3), (SizWidth - Side_Bar), SizHeight / 3});
    _reg.emplace_component<Color>(hudPlay4RectLines, WHITE);


    Entity const ChargeText = _reg.spawn_entity();
    _reg.emplace_component<Text>(ChargeText, "Charge : ", 32);
    _reg.emplace_component<Position>(ChargeText, PosWidth, PosHeight);
    _reg.emplace_component<Color>(ChargeText, WHITE);

    Entity const ScoreText = _reg.spawn_entity();
    _reg.emplace_component<Text>(ScoreText, "Score : ", 32);
    _reg.emplace_component<Position>(ScoreText, PosWidth, PosHeight + (SizHeight / 2));
    _reg.emplace_component<Color>(ScoreText, WHITE);

    Entity const ScoreValueText = _reg.spawn_entity();
    _reg.emplace_component<Text>(ScoreValueText, score, 32);
    _reg.emplace_component<Position>(ScoreValueText, PosWidth + (MeasureText("Score : ", 32)), PosHeight + (SizHeight / 2));
    _reg.emplace_component<Color>(ScoreValueText, WHITE);


}
#endif
