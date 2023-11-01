#pragma once

// #include "raylib.h"
#include "Component.hpp"
#include "Registry.hpp"

void move(Registry &r, sparse_array<Position> &positions, sparse_array<Speed> &speed, sparse_array<Direction> &dir);
void colision(Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size, sparse_array<SpawnGrace> &grace, sparse_array<Damages> &dam, sparse_array<Health> &helth);
void update_grace(Registry &r, sparse_array<SpawnGrace> &grace);
void update_weapon_position(Registry &r, sparse_array<Weapon> &weapons, sparse_array<Position> &postions);
void spawn_enemy(Registry &r, sparse_array<EnemyCount> &enemiesCount);
void enemyAlwaysShoot(
    Registry &r, sparse_array<AlwaysShoot> &always_shoot,
    sparse_array<Position> &positions, sparse_array<Size> &sizes);
void shootProjectiles(
    Registry &r, sparse_array<ProjectileShooter> &shooters,
    sparse_array<Position> &positions, sparse_array<Size> &sizes,
    sparse_array<Player> &players);
