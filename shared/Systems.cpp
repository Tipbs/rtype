#include "Component.hpp"
#include "Systems.hpp"
#include "indexed_zipper.hpp"
#include "zipper.hpp"

void move(
    Registry &r, sparse_array<Position> &positions, sparse_array<Speed> &speed,
    sparse_array<Direction> &dir)
{

    for (auto &&[pos, spe, diro]: zipper(positions, speed, dir)) {
        double x_offset = spe->speed * diro->dir_X;
        double y_offset = spe->speed * diro->dir_Y;

#ifdef SERVER
		pos->pos_X += x_offset * GetFrameTime();
		pos->pos_Y += y_offset * GetFrameTime();
#else
        pos->pos_X += x_offset;
        pos->pos_Y += y_offset;
 #endif
    }
}

void damages(
    Registry &r, sparse_array<Health> &healt, sparse_array<Damages> &dama,
    size_t i1, size_t i2)
{
    std::cout << "y a collision\n";
    std::cout << "Hello world" << std::endl;
    healt[i1]->health -= dama[i2]->damages;
    // std::osyncstream(std::cout) << "User " << i1 << " has taken " << dama[i2]->damages << " damages. He now have " << healt[i1]->health << " HP." << std::endl;
    healt[i2]->health -= dama[i1]->damages;
    // std::osyncstream(std::cout) << "User " << i2 << " has taken " << dama[i1]->damages << " damages. He now have " << healt[i2]->health << " HP." << std::endl;
    if (healt[i1]->health <= 0)
        r.kill_entity(r.entity_from_index(i1));
    if (healt[i2]->health <= 0)
        r.kill_entity(r.entity_from_index(i2));
}

void update_grace(Registry &r,
sparse_array<SpawnGrace> &graces)
{
    auto time = GetTimePoint();

    for (auto &&[ind, grace]: indexed_zipper(graces)) {
        if (grace->creation_time + grace->time >= time) {
            r.remove_component<SpawnGrace>(ind);
        }
    }
}

void colision(Registry &r,
sparse_array<Position> &positions, 
sparse_array<Size> &size, 
sparse_array<SpawnGrace> &grace, 
sparse_array<Damages> &dam, 
sparse_array<Health> &helth)
{
    for (auto &&[ind, pos, siz, dama, halth]: indexed_zipper(positions, size, dam, helth)) {
        if (grace[ind].has_value()) {
            continue;
        }
        for (auto &&[ind2, pos2, siz2, dama2, halth2]: indexed_zipper(positions, size, dam, helth)) {
            if (ind2 <= ind || grace[ind2].has_value()) {
                continue;
            }
            if (pos->pos_X > pos2->pos_X + siz2->size_X)
                continue;
            else if (pos->pos_Y > pos2->pos_Y + siz2->size_Y)
                continue;
            else if (pos2->pos_X > pos->pos_X + siz->size_X)
                continue;
            else if (pos2->pos_Y > pos->pos_Y + siz->size_Y)
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

void update_weapon_position(Registry &r, sparse_array<Weapon> &weapons, sparse_array<Position> &positions)
{
    for (auto &&[weapon, position]: zipper(weapons, positions)) {
        position->pos_X =  positions[static_cast<size_t>(weapon->owner_id)]->pos_X + 5;
        position->pos_Y =  positions[static_cast<size_t>(weapon->owner_id)]->pos_Y + 5;
    }
}

void spawn_enemy(Registry &r,
    sparse_array<EnemyCount> &enemiesCount
)
{
    for (auto &&[enemyCount]: zipper(enemiesCount)) {
        enemyCount->timeSinceLastSpawn += GetFrameTime();
        if (enemyCount->leftToSpawn > 0 && enemyCount->timeSinceLastSpawn > enemyCount->spawnFrequency) {
            std::cout << enemyCount->timeSinceLastSpawn << " enemies left : " << enemyCount->leftToSpawn << std::endl;
            const Entity ent = r.spawn_entity();
            float randomNumber = rand() % 1080;
            Utils::Vec2 pos = {1000, randomNumber + 50};

            r.emplace_component<Position>(ent, pos);
            r.emplace_component<Speed>(ent, 300);
            r.emplace_component<Direction>(ent, 50, 0);
            r.emplace_component<SpawnGrace>(ent, std::chrono::seconds(5));
            // r.emplace_component<NetworkEntity>(ent, id);
            enemyCount->timeSinceLastSpawn = 0;
            enemyCount->leftToSpawn--;
        }
    }
}
