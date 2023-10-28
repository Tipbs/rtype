#pragma once
#include <cstdint>
#include <boost/serialization/serialization.hpp>
#include <mutex>
#include "Utils.hpp"

class UserCmd {
public:
    UserCmd()
    {
        reset();
    }
    void reset()
    {
        attacking = false;
        attackScale = 0;
        moved.x = 0;
        moved.y = 0;
    }

    void setAttack(float scale)
    {
        attacking = true;
        attackScale = scale;
    }
    uint8_t attacking;
    float attackScale; // bitflag if we have multiple states ?
    Utils::Vec2 moved; // how much it moved in x, y directions
    // no idea if we must provide the id in the packet
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar &attacking;
        ar &attackScale;
        ar &moved;
    };
};

struct ThreadUserCmd {
    UserCmd cmd;
    std::mutex mutex;
};
