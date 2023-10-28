#include "Bundle.hpp"
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include "Component.hpp"
#include "random"

#ifndef SERVER
#include "../gui/include/GraphicSystems.hpp"
#endif

Entity create_player(Registry &reg, size_t id, Position pos)
{
    Player player;
    Entity const new_entity = reg.spawn_entity();
    Animation animation(1);
    Size Size(83, 43);
    // std::string path = "./gui/ressources/Sprites/Testships.png";
    Speed speedo(300);
    Direction diro(0, 0);
    SpawnGrace gra(std::chrono::seconds(1));
    // #ifndef SERVER
    // Sprite sprite(path.c_str());
    // #endif

    reg.add_component(new_entity, std::move(player));
    reg.add_component(new_entity, std::move(pos));
    reg.add_component(new_entity, std::move(Size));
    // #ifndef SERVER
    // reg.add_component(new_entity, std::move(sprite));
    // #endif
    reg.add_component(new_entity, std::move(speedo));
    reg.add_component(new_entity, std::move(diro));
    reg.add_component(new_entity, std::move(gra));
    reg.add_component(new_entity, std::move(animation));
    reg.emplace_component<NetworkedEntity>(new_entity, id);

    return (size_t) new_entity;
}

Entity create_ammo(Registry &reg, Position pos, float damage_mult, int color_id)
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
    Direction diro = Direction(1, 0); // std::pow(-1, rand()) * rand()
    Position posCopy(
        pos.pos_X - (float) hitwidth / 2, pos.pos_Y - (float) hitheight / 2);
    #ifndef SERVER
    Sprite sprite(
        path.c_str()); // width height are the visual box

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
    return new_entity;
}
