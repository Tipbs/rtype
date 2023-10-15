#pragma once
#include "../../shared/Bundle.hpp"
#include "../../shared/Sparse_array.hpp"
#include "../../shared/Systems.hpp"
#include "GraphicComponents.hpp"

void updateWithSnapshots(
    Registry &r, sparse_array<Position> &positions,
    sparse_array<Player> &players);
void display(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size,
    sparse_array<Sprite> &sprite, sparse_array<Player> &anim);
void handle_dir_inputs(
    Registry &r, sparse_array<Direction> &dir, sparse_array<Player> &anim,
    sparse_array<Sprite> &sprite);
void handle_shoot_inputs(
    Registry &r, sparse_array<Player> &anim, sparse_array<Position> &pos,
    sparse_array<Size> &siz);
void make_infinite_background(
    Registry &r, sparse_array<Position> &pos, sparse_array<Size> &siz);
