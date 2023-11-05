#pragma once

#include <map>
#include <boost/asio/io_context.hpp>
#include "Component.hpp"
#include "Entity.hpp"
#include "Registry.hpp"
#include "NetEnt.hpp"

class Factory {
    public:
        Factory(Registry &reg);

        // template<typename ...Args>
        // const Entity create(size_t id, Args ...args);
        const Entity create_background(const int ScreenWidth, const int ScreenHeight);
        const Entity create_player(Position pos, size_t net_id);
        const Entity create_weapon(Entity owner);
        const Entity create_enemy(Position pos, size_t net_id);
        const Entity create_ammo(Position pos, float damage_mult, int color_id);
        const Entity create_ammo(
            Position pos, Direction diro, float damage_mult, int color_id,
            size_t net_id);
        const Entity create_zorg(Position pos, size_t net_id = 0);
        const Entity create_netent(
            EntityType type, NetEnt &ent);
        const Entity create_asteroids(Position pos, size_t net_id = 0);
        const Entity create_boss_projectile(Position pos, Direction diro, size_t net_id);
        const Entity create_boss(Position pos, size_t net_id);
        const void create_menu(const int ScreenWidth, const int ScreenHeight);

        void create_hud(const int ScreenWidth, const int ScreenHeight, Entity scoreFrom, Entity chargeFrom);
        void register_components();
        void add_systems();
        void start_game(boost::asio::io_context &context);

    private:
        Registry &_reg;
};

// template<typename ...Args>
// const Entity Factory::create(size_t id, Args ...args)
// {
//     if (id == 0) {
//         return _create_background(args...);
//     } else if (id > 0 && id < 5) {
//         return _create_player(id, args...);
//     }
//     std::string error = std::string("Factory: component") + std::to_string(id) + std::string("not register");
//     throw std::invalid_argument(error);
// }
