#pragma once
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
        moved[0] = 0;
        moved[1] = 0;
    }
    int8_t attackState; // bitflag if we have multiple states ?
    Utils::Vec2 moved[2]; // how much it moved in x, y directions
    // no idea if we must provide the id in the packet
};
