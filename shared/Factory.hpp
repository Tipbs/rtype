#pragma once

#include <map>
#include "Component.hpp"
#include "Entity.hpp"
#include "Registry.hpp"
#include "../../shared/NetEnt.hpp"

class Factory {
    public:
        Factory(Registry &reg);

        // template<typename ...Args>
        // const Entity create(size_t id, Args ...args);
        const Entity create_background(const int ScreenWidth, const int ScreenHeight);
        const Entity create_player(int id, Position pos);
        const Entity create_weapon(Entity owner);
        const Entity create_enemy();
        
    private: 
        auto _register_components();
        void _add_system();

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
