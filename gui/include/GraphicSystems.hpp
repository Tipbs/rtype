#pragma once
#include "../../shared/Systems.hpp"
#include "GraphicComponents.hpp"
#include "../../shared/Sparse_array.hpp"

void display(Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size, sparse_array<Sprite> &sprite, sparse_array<MoveAnimCounter> &anim);
void handle_dir_inputs(Registry &r, sparse_array<Direction> &dir, sparse_array<MoveAnimCounter> &anim, sparse_array<Sprite> &sprite);
void updateWithSnapshots(
    Registry &r, sparse_array<Position> &positions,
    sparse_array<Player> &players);