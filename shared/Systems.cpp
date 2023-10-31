#include "Systems.hpp"
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include <syncstream>
#include "Bundle.hpp"
#include <chrono>
#include "Component.hpp"
#include "indexed_zipper.hpp"
#include "Registry.hpp"
#include "zipper.hpp"

#ifdef SERVER
std::mutex mutex;
static auto time_since_last_tick =
    std::chrono::high_resolution_clock::now(); // voir si raylib utilise m�me
                                               // chose

// float GetFrameTime()
//{
//	std::scoped_lock lock(mutex);
//	const auto now = std::chrono::high_resolution_clock::now();
//	return std::chrono::duration<double>(now -
// time_since_last_tick).count();
// }
float GetFrameTime() { return 1; }

void ResetFrameTime()
{
    std::scoped_lock lock(mutex);
    time_since_last_tick = std::chrono::high_resolution_clock::now();
}
#else
#include <raylib.h>
#endif // !SERVER

std::chrono::steady_clock::time_point GetTimePoint()
{
    return std::chrono::steady_clock::now();
}

void move(
    Registry &r, sparse_array<Position> &positions, sparse_array<Speed> &speed,
    sparse_array<Direction> &dir)
{
    for (auto &&[index, pos, spe, diro] :
         indexed_zipper(positions, speed, dir)) {
        // std::osyncstream(std::cout) << "y = " << pos->pos_Y << "  x = " <<
        // pos->pos_X << std::endl;
        pos->pos_X += spe->speed * diro->dir_X * GetFrameTime();
        pos->pos_Y += spe->speed * diro->dir_Y * GetFrameTime();
    }
}

void damages(
    Registry &r, sparse_array<Health> &healt, sparse_array<Damages> &dama,
    size_t i1, size_t i2)
{
    std::cout << "y a collision\n";
    std::cout << "Hello world" << std::endl;
    healt[i1]->health -= dama[i2]->damages;
    healt[i2]->health -= dama[i1]->damages;
    if (healt[i1]->health <= 0)
        r.kill_entity(r.entity_from_index(i1));
    if (healt[i2]->health <= 0)
        r.kill_entity(r.entity_from_index(i2));
}

void colision(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size,
    sparse_array<SpawnGrace> &grace, sparse_array<Damages> &dam,
    sparse_array<Health> &helth)
{
    auto time = GetTimePoint();
    for (auto &&[ind, pos, siz, dama, halth]: indexed_zipper(positions, size, dam, helth)) {
        if (grace[ind].value_or(SpawnGrace(std::chrono::seconds(0))).creation_time + grace[ind].value_or(SpawnGrace(std::chrono::seconds(0))).time >= time)
                continue;
        for (size_t ind2 = ind + 1; ind2 < positions.size(); ind2++) {
            if (grace[ind2]
                .value_or(SpawnGrace(std::chrono::seconds(0)))
                        .creation_time +
                grace[ind2]
                    .value_or(SpawnGrace(std::chrono::seconds(0)))
                    .time >=
                time)
                continue;
            if (positions[ind].value().pos_X >
                positions[ind2].value().pos_X + size[ind2].value().size_X / 2.)
                continue;
            else if (
                positions[ind].value().pos_Y >
                positions[ind2].value().pos_Y + size[ind2].value().size_Y / 2.)
                continue;
            else if (
                positions[ind2].value().pos_X >
                positions[ind].value().pos_X - size[ind].value().size_X / 2.)
                continue;
            else if (
                positions[ind2].value().pos_Y >
                positions[ind].value().pos_Y - size[ind].value().size_Y / 2.)
                continue;
            else
                damages(r, helth, dam, ind, ind2);
        }
    }
}

void enemyAlwaysShoot(
    Registry &r, sparse_array<AlwaysShoot> &always_shoot,
    sparse_array<Position> &positions, sparse_array<Size> &sizes)
{
    auto now = std::chrono::steady_clock::now();
    for (auto index = 0; index != always_shoot.size(); ++index) {
        auto &shoot = always_shoot[index];
        const auto &pos = positions[index];
        const auto &size = sizes[index];
        if (!(pos && shoot && size))
            continue;
        if (now - shoot->last_shoot > shoot->delay) {
            shoot->last_shoot = now;
            create_ammo(
                r,
                Position(
                    pos->pos_X - (size->size_X / 2),
                    pos->pos_Y + (size->size_Y / 2)),
                Direction(-1, 0), 1.0, 3);
        }
    }
}

void shootProjectiles(
    Registry &r, sparse_array<ProjectileShooter> &shooters,
    sparse_array<Position> &positions, sparse_array<Size> &sizes)
{
    auto now = std::chrono::steady_clock::now();
    for (auto index = 0; index != shooters.size(); ++index) {
        if (!(shooters[index] && positions[index]) && sizes[index])
            continue;
        if (now > shooters[index]->lastShot + shooters[index]->delay) {
            shooters[index]->lastShot = now;
            auto size_as_pos =
                Position(sizes[index]->size_X / 2, sizes[index]->size_Y / 2);
            for (auto &proj : shooters[index]->infos) {
                create_boss_projectile(
                    r, *positions[index] + size_as_pos + proj.offset, proj.dir);
            }
        }
    }
}
