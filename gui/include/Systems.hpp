//#include "raylib.h"
#include "Component.hpp"
#include "Registry.hpp"

#ifndef SYSTEMS_HPP
    #define SYSTEMS_HPP

    void move(Registry &r, sparse_array<Position> &positions, sparse_array<Speed> &speed, sparse_array<Direction> &dir);
    void display(Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size, sparse_array<Sprite> &sprite);
    void colision(Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size, sparse_array<SpawnGrace> &grace);
    void handle_inputs(Registry &r, sparse_array<Direction> &dir);

#endif