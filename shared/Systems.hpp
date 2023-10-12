#pragma once

//#include "raylib.h"
#include "Component.hpp"
#include "Registry.hpp"

void move(Registry &r, sparse_array<Position> &positions, sparse_array<Speed> &speed, sparse_array<Direction> &dir);
void colision(Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size, sparse_array<SpawnGrace> &grace);