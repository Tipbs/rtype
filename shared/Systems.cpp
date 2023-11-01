#include <cmath>
#include <numbers>
#include "Component.hpp"
#include "Factory.hpp"
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
    auto graces_size = graces.size();

	for (size_t ind = 0; ind != graces_size; ++ind) {
        if (!graces[ind])
            continue;
        if (graces[ind]->creation_time + graces[ind]->time >= time) {
            r.remove_component<SpawnGrace>(ind);
        }
    }
}

void colision(Registry &r,
sparse_array<Position> &positions, 
sparse_array<Size> &sizes,
sparse_array<SpawnGrace> &grace, 
sparse_array<Damages> &dmgs,
sparse_array<Health> &healths)
{
    auto pos_size = positions.size();
	for (size_t ind = 0; ind != pos_size; ++ind) {
        if (!(positions[ind] && sizes[ind] && dmgs[ind] && healths[ind]))
            continue;

        if (grace[ind].has_value()) {
            continue;
        }
		for (size_t ind2 = 0; ind2 != pos_size; ++ind2) {
			if (!(positions[ind2] && sizes[ind2] && dmgs[ind2] && healths[ind2]))
				continue;
            if (ind2 <= ind || grace[ind2].has_value()) {
                continue;
            }
            if (!positions[ind]) { // need to recheck because damages may have kill the entity
                continue;
            }
            if (positions[ind]->pos_X >
				positions[ind2]->pos_X + sizes[ind2]->size_X)
				continue;
            else if (
                positions[ind]->pos_Y > positions[ind2]->pos_Y + sizes[ind2]->size_Y)
                continue;
            else if (
                positions[ind2]->pos_X > positions[ind]->pos_X + sizes[ind]->size_X)
                continue;
            else if (
                positions[ind2]->pos_Y > positions[ind]->pos_Y + sizes[ind]->size_Y)
                continue;
            else
                damages(r, healths, dmgs, ind, ind2);
        }
    }
}

void enemyAlwaysShoot(
    Registry &r, sparse_array<AlwaysShoot> &always_shoot,
    sparse_array<Position> &positions, sparse_array<Size> &sizes)
{
    Factory factory(r);

    auto now = std::chrono::steady_clock::now();
    for (size_t index = 0; index != always_shoot.size(); ++index) {
        auto &shoot = always_shoot[index];
        const auto &pos = positions[index];
        const auto &size = sizes[index];
        if (!(pos && shoot && size))
            continue;
        if (now - shoot->last_shoot > shoot->delay) {
            shoot->last_shoot = now;
            factory.create_ammo(
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
            enemyCount->timeSinceLastSpawn = 0;
            enemyCount->leftToSpawn--;
            Factory f(r);
            float randomNumber = rand() % 1080;
            Utils::Vec2 pos = {1000, randomNumber + 50};

            f.create_enemy(pos);
        }
    }
}

static size_t getClosestPlayerToBoss(
    sparse_array<Position> &pos, sparse_array<Player> &player,
    Position &boss_pos)
{
    size_t closest = -1;
    size_t closest_distance = -1; // would require sqrt to be the real distance
    for (auto &&[index, pos, player] : indexed_zipper(pos, player)) {
        auto distance_to_boss = std::pow(boss_pos.pos_X - pos->pos_X, 2) +
                                std::pow(boss_pos.pos_Y - pos->pos_Y, 2);
        if (distance_to_boss < closest_distance) {
            closest = index;
            closest_distance = distance_to_boss;
        }
    }
    return closest;
}

static void updateBossProjectiles(
    ProjectileShooter &shooter, sparse_array<Position> &pos,
    sparse_array<Player> &players)
{
    auto size = shooter.infos.size();
    auto radius = 80;
    // shift the next shot
    for (size_t i = 0; i != size; ++i) {
        double angle = 2 * std::numbers::pi * i / size + shooter.shotCount * 45;
        double x = cos(angle) * radius;
        double y = sin(angle) * radius;
        shooter.infos[i].offset = Position(x, y);
        shooter.infos[i].dir = Direction(cos(angle) / 3, sin(angle) / 3);
    }
    // shoot the closest player
    Position bossPos(600, 300);
    size_t closest_player = getClosestPlayerToBoss(pos, players, bossPos);
    Position &player_pos = *pos[closest_player];
    Direction dir_to_player = Direction(
        player_pos.pos_X - bossPos.pos_X, player_pos.pos_Y - bossPos.pos_Y);
    auto dir_vec_len = std::sqrt(
        std::pow(dir_to_player.dir_X, 2) + std::pow(dir_to_player.dir_Y, 2));
    dir_to_player.dir_X /= dir_vec_len;
    dir_to_player.dir_Y /= dir_vec_len;
    if (!shooter.infos.empty())
        shooter.infos.pop_back(); // remove the last shot on closest player
    shooter.infos.push_back(ProjectileInfo(Position(0, 0), dir_to_player));
}

void shootProjectiles(
    Registry &r, sparse_array<ProjectileShooter> &shooters,
    sparse_array<Position> &positions, sparse_array<Size> &sizes,
    sparse_array<Player> &players)
{
    Factory factory(r);

    auto now = std::chrono::steady_clock::now();
    for (size_t index = 0; index != shooters.size(); ++index) {
        if (!(shooters[index] && positions[index] && sizes[index]))
            continue;
        if (now > shooters[index]->lastShot + shooters[index]->delay) {
            shooters[index]->lastShot = now;
            auto size_as_pos =
                Position(sizes[index]->size_X / 2, sizes[index]->size_Y / 2);
            for (auto &proj : shooters[index]->infos) {
                factory.create_boss_projectile(
                    *positions[index] + size_as_pos + proj.offset, proj.dir);
            }
            ++shooters[index]->shotCount;
            updateBossProjectiles(
                *shooters[index], positions,
                players); // atm called for each shooter but should check if
                          // it's the boss and the phase of the boss (health)
        }
    }
}
