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
        attackState = 0;
        moved.x = 0;
        moved.y = 0;
    }
    uint8_t attackState; // bitflag if we have multiple states ?
    Utils::Vec2 moved; // how much it moved in x, y directions
    // no idea if we must provide the id in the packet
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar &attackState;
        ar &moved;
    };
};

struct ThreadUserCmd {
    UserCmd cmd;
    std::mutex mutex;
};
