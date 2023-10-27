#pragma once

// #include "raylib.h"
#include "Bundle.hpp"
#include "Component.hpp"
#include "Registry.hpp"

void move(
    Registry &r, sparse_array<Position> &positions, sparse_array<Speed> &speed,
    sparse_array<Direction> &dir);
void colision(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size,
    sparse_array<SpawnGrace> &grace, sparse_array<Damages> &dam,
    sparse_array<Health> &helth);
void enemyAlwaysShoot(
    Registry &r, sparse_array<AlwaysShoot> &always_shoot,
    sparse_array<Position> &positions, sparse_array<Size> &sizes);
