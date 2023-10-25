#pragma once

//#include "raylib.h"
#include "Component.hpp"
#include "Registry.hpp"
#include "Bundle.hpp"

void move(
    Registry &r, sparse_array<Position> &positions, sparse_array<Speed> &speed,
    sparse_array<Direction> &dir
#ifdef SERVER
    , sparse_array<Player> &players
#endif
);
void colision(Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size, sparse_array<SpawnGrace> &grace, sparse_array<Damages> &dam, sparse_array<Health> &helth);
