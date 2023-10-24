#pragma once

#include <map>
#include "Entity.hpp"
#include "Registry.hpp"
#include "../../shared/NetEnt.hpp"

class Factory {
    public:
        Factory(Registry &reg) : _reg(reg) {};

        template<typename ...Args>
        const Entity create(size_t id, Args ...args);
        
    private: 
        auto &_register_components();
        const Entity _create_player(int id, Utils::Vec2 pos, int type);
        const Entity _create_enemy();

    private:
        Registry &_reg;
};

template<typename ...Args>
const Entity Factory::create(size_t id, Args ...args)
{
    if (id < 4) {
        return _create_player(args...);
    }
}
