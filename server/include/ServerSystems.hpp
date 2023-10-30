#pragma once

#include "../../shared/Bundle.hpp"
#include "../../shared/Registry.hpp"
#include "../../shared/zipper.hpp"

void synchronize(
    Registry &reg, sparse_array<Direction> &directions,
    sparse_array<Speed> &spe, sparse_array<Position> &positions,
    sparse_array<Size> &sizes, sparse_array<Player> &players);
void extract(
    Registry &reg, sparse_array<Position> &positions,
    sparse_array<Speed> &speeds, sparse_array<Player> &players,
    sparse_array<NetworkedEntity> &ents);
void updateWithSnapshots(
    Registry &r, sparse_array<Position> &positions,
    sparse_array<NetworkedEntity> &entities, sparse_array<Speed> &speeds,
    sparse_array<Current_Player> &currents, sparse_array<Size> &sizes,
    sparse_array<Player> &players);
void handle_shoot_inputs(
    Registry &r, sparse_array<Player> &anim, sparse_array<Position> &pos,
    sparse_array<Size> &siz, sparse_array<Current_Player> &current);
void resetPlayersDir(
    Registry &r, sparse_array<Player> &players,
    sparse_array<Direction> &directions);
