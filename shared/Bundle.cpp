#include "Bundle.hpp"
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include "Component.hpp"
#include "random"

size_t create_player(Registry &reg, size_t id, Position pos)
{
    Entity const new_entity = reg.spawn_entity();
    Player player(1, id);
    Size Size(83, 43);
    std::string path = "./gui/ressources/Sprites/Testships.png";
    Speed speedo(300);
    Direction diro(0, 0);
    SpawnGrace gra(std::chrono::seconds(1));
    #ifndef SERVER
    Sprite sprite(path.c_str(), 83, 43, 2, 5);
    #endif

    reg.add_component(new_entity, std::move(pos));
    reg.add_component(new_entity, std::move(Size));
    #ifndef SERVER
    reg.add_component(new_entity, std::move(sprite));
    #endif
    reg.add_component(new_entity, std::move(speedo));
    reg.add_component(new_entity, std::move(diro));
    reg.add_component(new_entity, std::move(gra));
    reg.add_component(new_entity, std::move(player));
    reg.emplace_component<NetworkedEntity>(new_entity, id);

    return (size_t) new_entity;
}

void create_ammo(
    Registry &reg, Position pos, Direction diro,
    float damage_mult, int color_id)
{
    Entity const new_entity = reg.spawn_entity();
    int hitwidth = 120 * (damage_mult / 2);
    int hitheight = 24 * (damage_mult / 2);
    Size Size(hitwidth, hitheight); // hitbox
    std::string path =
        "./gui/ressources/Sprites/shoot_ammo.png"; // fichier de la munition
    Speed speedo(450);
    Damages damag(damage_mult);
    Health healt(1);
    Position posCopy(
        pos.pos_X - (float) hitwidth / 2, pos.pos_Y - (float) hitheight / 2);
    #ifndef SERVER
    Sprite sprite(
        path.c_str(), hitwidth, hitheight, 8, 5); // width height are the visual box

    sprite.color_id = color_id;
    #endif

    reg.add_component(
        new_entity, std::move(posCopy));
    reg.add_component(new_entity, std::move(Size));
    #ifndef SERVER
    reg.add_component(new_entity, std::move(sprite));
    #endif
    reg.add_component(new_entity, std::move(speedo));
    reg.add_component(new_entity, std::move(diro));
    reg.add_component(new_entity, std::move(damag));
    reg.add_component(new_entity, std::move(healt));
}

void create_ammo(Registry& reg, Position pos, float damage_mult, int color_id)
{
    Entity const new_entity = reg.spawn_entity();
    int hitwidth = 120 * (damage_mult / 2);
    int hitheight = 24 * (damage_mult / 2);
    Size Size(hitwidth, hitheight); // hitbox
    std::string path =
        "./gui/ressources/Sprites/shoot_ammo.png"; // fichier de la munition
    Speed speedo(450);
    Damages damag(damage_mult);
    Direction diro(1, 0);
    Health healt(1);
    Position posCopy(
        pos.pos_X - (float) hitwidth / 2, pos.pos_Y - (float) hitheight / 2);
    #ifndef SERVER
    Sprite sprite(
        path.c_str(), hitwidth, hitheight, 8, 5); // width height are the visual box

    sprite.color_id = color_id;
    #endif

    reg.add_component(
        new_entity, std::move(posCopy));
    reg.add_component(new_entity, std::move(Size));
    #ifndef SERVER
    reg.add_component(new_entity, std::move(sprite));
    #endif
    reg.add_component(new_entity, std::move(speedo));
    reg.add_component(new_entity, std::move(diro));
    reg.add_component(new_entity, std::move(damag));
    reg.add_component(new_entity, std::move(healt));
}

// net_id obviously useless for server
size_t create_zorg(Registry &reg, Position pos, size_t net_id = 0)
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
