#include "Factory.hpp"
#include <chrono>
#include <functional>
#include <numbers>
#include <math.h>
#include <stdlib.h> /* srand, rand */
#include "Component.hpp"
#include "Entity.hpp"
#include "NetEnt.hpp"
#include "Systems.hpp"

#ifndef SERVER
#include <raylib.h>
#include "../gui/include/GraphicComponents.hpp"
#include "../gui/include/GraphicSystems.hpp"
#else
#include "../server/include/ServerSystems.hpp"
#endif

void Factory::register_components()
{
    _reg.register_components<
#ifndef SERVER
        Sprite, InputField, Rectangle,
        // HUD,
        Rect, Color, Text, ScoreText, ChargeRect, MusicComponent, SoundManager,
#endif
        Player, Weapon, Current_Player, Position, Damages, Size, Health, Speed,
        Direction, SpawnGrace, NetworkedEntity, Animation, Couleur,
        ProjectileShooter, Score, Backgrounds, AlwaysShoot, EnemyCount,
        BossCount, Colision, Point, Boss>();
}

void Factory::add_systems()
{
#ifdef SERVER
    _reg.add_system<EnemyCount, BossCount>(spawn_enemy);
    _reg.add_system<EnemyCount, BossCount, NetworkedEntity, Position, Health>(
        kill_zord);
    _reg.add_system<Direction, Speed, Position, Size, Weapon, Player>(
        synchronize);
#endif
    _reg.add_system<Boss, Position, Direction>(stopAtCenter);
    _reg.add_system<SpawnGrace>(update_grace);
    _reg.add_system<
        Position, Size, SpawnGrace, Damages, Health, Colision, Point, Score>(
        colision);
    _reg.add_system<
        Position, Speed, Direction
#ifdef SERVER
        ,
        Player
#endif
        >(move);
    _reg.add_system<Player, Position, Direction, Size>(block_player_in_map);
    _reg.add_system<Position, Colision>(kill_outside_entities);
    _reg.add_system<ProjectileShooter, Position, Size, Player>(
        shootProjectiles);
#ifdef SERVER
    _reg.add_system<AlwaysShoot, Position, Size>(enemyAlwaysShoot);
#endif
#ifndef SERVER
    _reg.add_system<Position, Sprite, Rectangle, InputField, Rect, Color, Text>(
        display);
    _reg.add_system<Direction, Current_Player, Sprite, Speed, Couleur>(
        handle_dir_inputs);
    _reg.add_system<Couleur, Size, Weapon, Position>(handle_shoot_inputs);
    //    _reg.add_system<InputField, Rectangle>(hadle_text_inputs);
    _reg.add_system<Sprite, Couleur>(do_animation);
    _reg.add_system<Sprite, Couleur, Weapon, Current_Player>(do_ship_animation);
    _reg.add_system<Position, Size, Backgrounds>(make_infinite_background);
    _reg.add_system<
        Position, NetworkedEntity, Speed, Current_Player, Size, Player, Boss, ProjectileShooter>(
        updateWithSnapshots);
    // _reg.add_system<Weapon, Couleur, HUD>(
    //     updateHUD);
    _reg.add_system<Score, ScoreText, Text>(update_score_text);
    _reg.add_system<Weapon, ChargeRect, Rect>(update_charge_rect);
    // _reg.add_system<MusicComponent>(handle_music);
    _reg.add_system<SoundManager>(play_sound);
#else
    _reg.add_system<Position, Speed, Weapon, NetworkedEntity, Direction, ProjectileShooter>(
        extract);
    _reg.add_system<Player, Direction>(resetPlayersDir);
#endif
}

Factory::Factory(Registry &reg) : _reg(reg) {}

const Entity
Factory::create_background(const int ScreenWidth, const int ScreenHeight)
{
    Entity const background = _reg.spawn_entity();
    std::string bgpath =
        "./gui/ressources/Backgrounds/Backtest.png"; // temp > 2 > 3 > 1

    _reg.emplace_component<Position>(background, 0, 0);
    _reg.emplace_component<Size>(background, ScreenWidth, ScreenHeight);
#ifndef SERVER
    _reg.emplace_component<Sprite>(
        background, bgpath.c_str(), 3 * ScreenWidth, ScreenHeight);
#endif
    _reg.emplace_component<Speed>(background, 20);
    _reg.emplace_component<Direction>(background, -1, 0);
    _reg.emplace_component<Backgrounds>(background);

    Entity const background1 = _reg.spawn_entity();
    std::string bgpath1 =
        "./gui/ressources/Backgrounds/Back_stars.png"; // temp > 2 > 3 > 1

    _reg.emplace_component<Position>(background1, 0, 0);
    _reg.emplace_component<Size>(background1, ScreenWidth, ScreenHeight);
#ifndef SERVER
    _reg.emplace_component<Sprite>(
        background1, bgpath1.c_str(), 3 * ScreenWidth, ScreenHeight);
#endif
    _reg.emplace_component<Speed>(background1, 50);
    _reg.emplace_component<Direction>(background1, -1, 0);
    _reg.emplace_component<Backgrounds>(background1);

    Entity const background2 = _reg.spawn_entity();
    std::string bgpath2 =
        "./gui/ressources/Backgrounds/Back_planets.png"; // temp > 2 > 3 > 1

    _reg.emplace_component<Position>(background2, 0, 0);
    _reg.emplace_component<Size>(background2, ScreenWidth, ScreenHeight);
#ifndef SERVER
    _reg.emplace_component<Sprite>(
        background2, bgpath2.c_str(), 3 * ScreenWidth, ScreenHeight);
#endif
    _reg.emplace_component<Speed>(background2, 80);
    _reg.emplace_component<Direction>(background2, -1, 0);
    _reg.emplace_component<Backgrounds>(background2);

    Entity const background3 = _reg.spawn_entity();
    std::string bgpath3 =
        "./gui/ressources/Backgrounds/Back_front.png"; // temp > 2 > 3 > 1

    _reg.emplace_component<Position>(background3, 0, 0);
    _reg.emplace_component<Size>(background3, ScreenWidth, ScreenHeight);
#ifndef SERVER
    _reg.emplace_component<Sprite>(
        background3, bgpath3.c_str(), 3 * ScreenWidth, ScreenHeight);
#endif
    _reg.emplace_component<Speed>(background3, 100);
    _reg.emplace_component<Direction>(background3, -1, 0);
    _reg.emplace_component<Backgrounds>(background3);
    return background;
}

const Entity Factory::create_player(Position pos, size_t net_id)
{
    Entity const new_entity = _reg.spawn_entity();
    std::string const path = "./gui/ressources/Sprites/Testships.png";

    _reg.emplace_component<Player>(new_entity);
    _reg.emplace_component<Position>(new_entity, pos);
    _reg.emplace_component<Size>(new_entity, 83, 43);
#ifndef SERVER
    _reg.emplace_component<Sprite>(new_entity, path.c_str(), 83, 43, 2, 5);
#endif
    _reg.emplace_component<Speed>(new_entity, 300);

    _reg.emplace_component<Direction>(new_entity, 0, 0);
    _reg.emplace_component<SpawnGrace>(new_entity, std::chrono::seconds(1));
    _reg.emplace_component<Animation>(new_entity);
    _reg.emplace_component<Couleur>(new_entity, 0);
    _reg.emplace_component<Score>(new_entity, 0);
    _reg.emplace_component<Health>(new_entity, 1000);
    _reg.emplace_component<Damages>(new_entity, 1);
    _reg.emplace_component<Colision>(new_entity, Tag::Player);
    _reg.emplace_component<NetworkedEntity>(
        new_entity, net_id, EntityType::Player);
    return new_entity;
}

const Entity Factory::create_weapon(Entity owner)
{
    Entity weapon = _reg.spawn_entity();

    std::cout << "owner: " << (size_t) owner << "\n";
    _reg.emplace_component<Weapon>(weapon, owner);
    _reg.emplace_component<Position>(weapon);
    return weapon;
}

const Entity Factory::create_enemy(Position pos, size_t net_id)
{
    const Entity ent = _reg.spawn_entity();

    _reg.emplace_component<Position>(ent, pos);
    _reg.emplace_component<Speed>(ent, 100);
    _reg.emplace_component<Direction>(ent, 50, 0);
    _reg.emplace_component<SpawnGrace>(ent, std::chrono::seconds(5));
    _reg.emplace_component<NetworkedEntity>(ent, net_id, EntityType::Enemy);
    return ent;
}

const Entity Factory::create_ammo(
    Position pos, float damage_mult, int color_id, Tag type, Direction diro)
{
    Entity const new_entity = _reg.spawn_entity();
    int hitwidth = 120 * (damage_mult / 2);
    int hitheight = 24 * (damage_mult / 2);

    _reg.emplace_component<Position>(
        new_entity, pos.pos_X - (float) hitwidth / 2,
        pos.pos_Y - (float) hitheight / 2);
    _reg.emplace_component<Size>(new_entity, hitwidth, hitheight);
#ifndef SERVER
    _reg.emplace_component<Sprite>(
        new_entity, "./gui/ressources/Sprites/shoot_ammo.png", hitwidth,
        hitheight, 8, 5);
#endif
    _reg.emplace_component<Speed>(new_entity, 300);
    _reg.add_component(new_entity, std::move(diro));
    _reg.emplace_component<Damages>(new_entity, damage_mult);
    _reg.emplace_component<Health>(new_entity, 1);
    _reg.emplace_component<Animation>(new_entity);
    _reg.emplace_component<Couleur>(new_entity, color_id);
    _reg.emplace_component<Colision>(new_entity, Tag::Damages, type);
    return new_entity;
}

const Entity Factory::create_ammo(
    Position pos, float damage_mult, int color_id, size_t net_id, Tag type,
    Direction diro)
{
    Entity const new_entity =
        create_ammo(pos, damage_mult, color_id, type, diro);

    _reg.emplace_component<NetworkedEntity>(
        new_entity, net_id, EntityType::Ammo);
    return new_entity;
}

const Entity Factory::create_zorg(Position pos, size_t net_id)
{
    Entity const new_entity = _reg.spawn_entity();
    Size Size(48, 48);
    Speed speedo(200);
    Direction diro(-0.40, 0);
    SpawnGrace gra(std::chrono::seconds(1));
#ifndef SERVER
    std::string path = "./gui/ressources/Sprites/Drone.png";
    Sprite sprite(path.c_str(), 48, 48);
#endif

    _reg.add_component(new_entity, std::move(pos));
    _reg.add_component(new_entity, std::move(Size));
#ifndef SERVER
    _reg.add_component(new_entity, std::move(sprite));
#endif
    _reg.add_component(new_entity, std::move(speedo));
    _reg.add_component(new_entity, std::move(diro));
    _reg.emplace_component<AlwaysShoot>(
        new_entity, std::chrono::milliseconds(750));
    _reg.emplace_component<SpawnGrace>(new_entity, std::chrono::seconds(1));
    _reg.emplace_component<Health>(new_entity, 1);
    _reg.emplace_component<NetworkedEntity>(
        new_entity, net_id, EntityType::Enemy);
    _reg.emplace_component<Colision>(new_entity, Tag::Enemy);

    return (size_t) new_entity;
}

const Entity Factory::create_asteroids(Position pos, size_t net_id)
{
    Entity const new_entity = _reg.spawn_entity();

    _reg.emplace_component<Position>(new_entity, pos.pos_X, pos.pos_Y);
    _reg.emplace_component<Size>(new_entity, 48, 48);
#ifndef SERVER
    int ast_color = rand() % 3;
    switch (ast_color) {
        case 1:
            _reg.emplace_component<Sprite>(
                new_entity, "./gui/ressources/Sprites/Asteroid2.png", 48, 48, 1,
                1);
            break;
        case 2:
            _reg.emplace_component<Sprite>(
                new_entity, "./gui/ressources/Sprites/Asteroid3.png", 48, 48, 1,
                1);
            break;
        default:
            _reg.emplace_component<Sprite>(
                new_entity, "./gui/ressources/Sprites/Asteroid.png", 48, 48, 1,
                1);
    }
#endif
    _reg.emplace_component<Speed>(new_entity, 150);
    _reg.emplace_component<Direction>(
        new_entity, Direction {-1, (double) (((rand() % 21) - 10) / 10.)});
    _reg.emplace_component<SpawnGrace>(new_entity, std::chrono::seconds(1));
    _reg.emplace_component<Health>(new_entity, 1);
    _reg.emplace_component<Damages>(new_entity, 1);
    // _reg.emplace_component<Tags>(new_entity, false, true, false, true, false,
    // false, false, true);
    _reg.emplace_component<Colision>(new_entity, Tag::Enemy);
    // _reg.emplace_component<NetworkedEntity>(new_entity, net_id);

    return new_entity;
}

#ifndef SERVER
void Factory::create_hud(
    const int ScreenWidth, const int ScreenHeight, Entity scoreFrom,
    Entity chargeFrom)
{
    float PosWidth = 0;
    float PosHeight = 9. * ScreenHeight / 10.;
    float SizWidth = ScreenWidth;
    float SizHeight = ScreenHeight / 10.;

    // DATA A RENDRE DYNAMIQUES

    Color play1color = BLUE;
    Color play2color = MAGENTA;
    Color play3color = RED;
    Color play4color = ORANGE;
    float multiplier = 0.65;
    std::string score = "42";

    // FIN DES DATA A RENDRE DYNAMIQUE

    Entity const hudBlackLayer = _reg.spawn_entity();
    _reg.emplace_component<Rect>(
        hudBlackLayer, false,
        Rectangle {PosWidth, PosHeight, SizWidth, SizHeight});
    _reg.emplace_component<Color>(hudBlackLayer, BLACK);

    Entity const hudPlay1Rect = _reg.spawn_entity();
    double rectChargeWidth = (SizWidth / 2) * multiplier;
    _reg.emplace_component<ChargeRect>(
        hudPlay1Rect, std::move(chargeFrom), std::move(rectChargeWidth));
    _reg.emplace_component<Rect>(
        hudPlay1Rect, false,
        Rectangle {
            PosWidth + MeasureText("Charge : ", 32), PosHeight,
            (SizWidth / 2) * multiplier, (SizHeight / 2)});
    _reg.emplace_component<Color>(hudPlay1Rect, play1color);

    Entity const hudPlay1RectLines = _reg.spawn_entity();
    _reg.emplace_component<Rect>(
        hudPlay1RectLines, true,
        Rectangle {
            PosWidth + MeasureText("Charge : ", 32), PosHeight, (SizWidth / 2),
            (SizHeight / 2)});
    _reg.emplace_component<Color>(hudPlay1RectLines, WHITE);

    float Side_Bar = PosWidth + MeasureText("Charge : ", 32) + (SizWidth / 2.);

    Entity const hudPlay2Rect = _reg.spawn_entity();
    _reg.emplace_component<Rect>(
        hudPlay2Rect, false,
        Rectangle {
            Side_Bar, PosHeight, (SizWidth - Side_Bar) * multiplier,
            SizHeight / 3});
    _reg.emplace_component<Color>(hudPlay2Rect, play2color);

    Entity const hudPlay2RectLines = _reg.spawn_entity();
    _reg.emplace_component<Rect>(
        hudPlay2RectLines, true,
        Rectangle {Side_Bar, PosHeight, (SizWidth - Side_Bar), SizHeight / 3});
    _reg.emplace_component<Color>(hudPlay2RectLines, WHITE);

    Entity const hudPlay3Rect = _reg.spawn_entity();
    _reg.emplace_component<Rect>(
        hudPlay3Rect, false,
        Rectangle {
            Side_Bar, PosHeight + (SizHeight / 3),
            (SizWidth - Side_Bar) * multiplier, SizHeight / 3});
    _reg.emplace_component<Color>(hudPlay3Rect, play3color);

    Entity const hudPlay3RectLines = _reg.spawn_entity();
    _reg.emplace_component<Rect>(
        hudPlay3RectLines, true,
        Rectangle {
            Side_Bar, PosHeight + (SizHeight / 3), (SizWidth - Side_Bar),
            SizHeight / 3});
    _reg.emplace_component<Color>(hudPlay3RectLines, WHITE);

    Entity const hudPlay4Rect = _reg.spawn_entity();
    _reg.emplace_component<Rect>(
        hudPlay4Rect, false,
        Rectangle {
            Side_Bar, PosHeight + 2 * (SizHeight / 3),
            (SizWidth - Side_Bar) * multiplier, SizHeight / 3});
    _reg.emplace_component<Color>(hudPlay4Rect, play4color);

    Entity const hudPlay4RectLines = _reg.spawn_entity();
    _reg.emplace_component<Rect>(
        hudPlay4RectLines, true,
        Rectangle {
            Side_Bar, PosHeight + 2 * (SizHeight / 3), (SizWidth - Side_Bar),
            SizHeight / 3});
    _reg.emplace_component<Color>(hudPlay4RectLines, WHITE);

    Entity const ChargeText = _reg.spawn_entity();
    _reg.emplace_component<Text>(ChargeText, "Charge : ", 32);
    _reg.emplace_component<Position>(ChargeText, PosWidth, PosHeight);
    _reg.emplace_component<Color>(ChargeText, WHITE);

    Entity const scoreText = _reg.spawn_entity();
    _reg.emplace_component<Text>(scoreText, "Score : ", 32);
    _reg.emplace_component<Position>(
        scoreText, PosWidth, PosHeight + (SizHeight / 2));
    _reg.emplace_component<Color>(scoreText, WHITE);

    Entity const scoreValueText = _reg.spawn_entity();
    _reg.add_component<ScoreText>(scoreValueText, std::move(scoreFrom));
    _reg.emplace_component<Text>(scoreValueText, "", 32);
    _reg.emplace_component<Position>(
        scoreValueText, PosWidth + (MeasureText("Score : ", 32)),
        PosHeight + (SizHeight / 2));
    _reg.emplace_component<Color>(scoreValueText, WHITE);
}
#endif

const Entity
Factory::create_boss_projectile(Position pos, Direction diro, size_t net_id)
{
    Entity const entity = _reg.spawn_entity();

    _reg.add_component(entity, std::move(pos));
    _reg.emplace_component<Size>(entity, 25, 25);
#ifndef SERVER
    _reg.emplace_component<Sprite>(
        entity, "./gui/ressources/Sprites/red_projectile.png", 25, 25, 1, 1);
#endif
    _reg.emplace_component<Speed>(entity, 200);
    _reg.add_component(entity, std::move(diro));
    _reg.emplace_component<Damages>(entity, 20);
    _reg.emplace_component<Health>(entity, 1);
    // _reg.emplace_component<Tags>(entity, false, true, false, false, false,
    // true, false, true);
    _reg.emplace_component<Colision>(entity, Tag::Enemy);
    return entity;
}

const Entity Factory::create_boss(Position pos, size_t net_id)
{
    Entity const new_entity = _reg.spawn_entity();
    Size Size(98, 100);
    Speed speedo(300);
    Direction diro(-0.2, 0);
    SpawnGrace gra(std::chrono::seconds(1));
    ProjectileShooter proj_shooter(std::chrono::milliseconds(500));
#ifndef SERVER
    std::string path = "./gui/ressources/Sprites/boss.png";
    Sprite sprite(path.c_str(), 97, 102, 10, 1);
#endif

    _reg.add_component(new_entity, std::move(pos));
    _reg.add_component(new_entity, std::move(Size));
#ifndef SERVER
    _reg.add_component(new_entity, std::move(sprite));
#endif
    _reg.add_component(new_entity, std::move(speedo));
    _reg.add_component(new_entity, std::move(diro));
    // _reg.emplace_component<AlwaysShoot>(
    //     new_entity, std::chrono::milliseconds(750));
    _reg.emplace_component<SpawnGrace>(new_entity, std::chrono::seconds(1));
    _reg.emplace_component<Health>(new_entity, 1000);
    _reg.emplace_component<Damages>(new_entity, 1);
    _reg.emplace_component<Boss>(new_entity);
    _reg.emplace_component<Couleur>(new_entity, 0);
#ifdef SERVER
    auto &shooter = _reg.add_component<ProjectileShooter>(
        new_entity, std::move(proj_shooter));
    auto radius = 80;
    for (int i = 0; i <= 12; i++) {
        double angle = 2 * std::numbers::pi * i / 12;
        double x = cos(angle) * radius;
        double y = sin(angle) * radius;
        shooter->infos.push_back(ProjectileInfo(
            Position(x, y), Direction(cos(angle) / 3, sin(angle) / 3)));
    }
#endif
    _reg.emplace_component<NetworkedEntity>(
        new_entity, net_id, EntityType::Boss);

    return (size_t) new_entity;
}

const Entity Factory::create_netent(EntityType type, NetEnt &net_ent)
{
    // static std::vector<std::pair<EntityType, std::function<const
    // Entity(Position pos, size_t net_id)>>> pairs = {
    static auto pairs = {
        std::make_pair(EntityType::Boss, std::mem_fn(&Factory::create_boss)),
        std::make_pair(
            EntityType::Player, std::mem_fn(&Factory::create_player)),
        std::make_pair(EntityType::Zorg, std::mem_fn(&Factory::create_zorg)),
        std::make_pair(EntityType::Enemy, std::mem_fn(&Factory::create_zorg))};
    auto &pos = net_ent.pos;
    auto net_id = net_ent.id;
    auto dir = Direction(net_ent.dir.x, net_ent.dir.y);
    std::cout << "creating: " << (size_t) net_ent.type << "\n";
    for (auto &pair : pairs)
        if (type == pair.first)
            return pair.second(this, pos, net_id);
    switch (type) {
        case EntityType::Ammo:
            return create_ammo(pos, 1.0, 1, net_id, Tag::Enemy, dir);
        /*case EntityType::Win:
            return;
        case EntityType::Lose:
            return;
            */
        default:
            throw std::exception();
    }
}

void Factory::create_points(Position pos, int nbr, int points)
{
    int width = 32;
    int height = 32;

    for (int i = 0; i < nbr; i++) {
        Entity const entity = _reg.spawn_entity();
        int x = pos.pos_X + (rand() % 100 - 50);
        int y = pos.pos_Y + (rand() % 100 - 50);

        _reg.emplace_component<Position>(entity, x, y);
        _reg.emplace_component<Size>(entity, width, height);
        _reg.emplace_component<Colision>(entity, Tag::Collactable);
        _reg.add_component<Point>(entity, points);
#ifndef SERVER
        _reg.emplace_component<Sprite>(
            entity, "./gui/ressources/Sprites/points.png", width, height, 7, 1,
            rand() % 7);
#endif
    }
}

#ifndef SERVER
void Factory::create_sounds(Registry &reg)
{
    Entity sounds = reg.spawn_entity();
    reg.emplace_component<SoundManager>(sounds);
    reg.emplace_component<MusicComponent>(
        sounds, "./gui/ressources/Audio/battle_ost.mp3", MusicFx::Battle);
}
#endif
