#pragma once

#include "../../shared/Registry.hpp"

size_t create_player_server(Registry& reg, Position pos)
{
    Entity const new_entity = reg.spawn_entity();
    Player player((size_t)new_entity);
    Size Size(83, 43);
    Speed speedo(300);
    Direction diro(0, 0);
    SpawnGrace gra(std::chrono::seconds(1));

    reg.add_component(new_entity, std::move(pos));
    reg.add_component(new_entity, std::move(Size));
    reg.add_component(new_entity, std::move(speedo));
    reg.add_component(new_entity, std::move(diro));
    reg.add_component(new_entity, std::move(gra));
    reg.add_component(new_entity, std::move(player));
    return (size_t) new_entity;
}