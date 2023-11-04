#pragma once

#include "../../shared/Component.hpp"
#include "../../shared/Registry.hpp"
#include "../../shared/zipper.hpp"

void synchronize(
    Registry &reg, sparse_array<Direction> &directions,
    sparse_array<Speed> &spe, sparse_array<Position> &positions,
    sparse_array<Size> &sizes, sparse_array<Weapon> &weapons,
    sparse_array<Player> &players);
void extract(
    Registry &reg, sparse_array<Position> &positions,
    sparse_array<Speed> &speeds, sparse_array<Weapon> &weapons,
    sparse_array<NetworkedEntity> &ents, sparse_array<Direction> &directions);
void updateWithSnapshots(
    Registry &r, sparse_array<Position> &positions,
    sparse_array<NetworkedEntity> &entities, sparse_array<Speed> &speeds,
    sparse_array<Current_Player> &currents, sparse_array<Size> &sizes,
    sparse_array<Player> &players);
void handle_shoot_inputs(
    Registry &r, sparse_array<Animation> &anim, sparse_array<Position> &pos,
    sparse_array<Size> &siz, sparse_array<Current_Player> &current);
void resetAnimationsDir(
    Registry &r, sparse_array<Player> &players,
    sparse_array<Direction> &directions);
void resetPlayersDir(
    Registry &r, sparse_array<Player> &players,
    sparse_array<Direction> &directions);
void kill_zord(
    Registry &r, sparse_array<EnemyCount> &enemyCount,
    sparse_array<BossCount> &bosses, sparse_array<NetworkedEntity> &ents,
    sparse_array<Position> &positions, sparse_array<Health> &);
