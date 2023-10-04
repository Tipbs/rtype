#include "raylib.h"
#include "Component.hpp"

#ifndef SYSTEMS_HPP
    #define SYSTEMS_HPP

    void move(Registry &r, sparse_array<Position> positions, sparse_array<Speed> speed, sparse_array<Direction> dir);

#endif