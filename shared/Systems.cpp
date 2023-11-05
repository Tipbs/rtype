#include "Systems.hpp"
#include <cmath>
#include <numbers>
#include "Component.hpp"
#include "Factory.hpp"
#include "indexed_zipper.hpp"
#include "Utils.hpp"
#include "zipper.hpp"

void move(
    Registry &r, sparse_array<Position> &positions, sparse_array<Speed> &speed,
    sparse_array<Direction> &dir
#ifdef SERVER
    ,
    sparse_array<Player> &players
#endif
)
{
    for (auto &&[ind, pos, spe, diro] : indexed_zipper(positions, speed, dir)) {
        double x_offset = spe->speed * diro->dir_X;
        double y_offset = spe->speed * diro->dir_Y;

#ifdef SERVER
        if (players[ind]) {
            pos->pos_X += x_offset;
            pos->pos_Y += y_offset;
        } else {
            pos->pos_X += x_offset * GetFrameTime();
            pos->pos_Y += y_offset * GetFrameTime();
        }
#else
        pos->pos_X += x_offset * GetFrameTime();
        pos->pos_Y += y_offset * GetFrameTime();
#endif
    }
}

void kill_outside_entities(
    Registry &r, sparse_array<Position> &pos, sparse_array<Colision> &colisions)
{
    for (auto &&[ind, position, colision] : indexed_zipper(pos, colisions))
        if (position->pos_X > 1780)
            r.kill_entity(r.entity_from_index(ind));
        else if (position->pos_X < -500)
            r.kill_entity(r.entity_from_index(ind));
        else if (position->pos_Y > 1220)
            r.kill_entity(r.entity_from_index(ind));
        else if (position->pos_Y < -500)
            r.kill_entity(r.entity_from_index(ind));
}

void stopAtCenter(
    Registry &r, sparse_array<Boss> &boss, sparse_array<Position> &positions,
    sparse_array<Direction> &directions)
{
    for (auto &&[_, pos, dir] : zipper(boss, positions, directions))
        if (pos->pos_X <= 1280 / 2. - 50.)
            dir->dir_X = 0;
}

void block_player_in_map(
    Registry &r, sparse_array<Player> &players,
    sparse_array<Position> &positions, sparse_array<Direction> &directions,
    sparse_array<Size> &sizes)
{
    for (auto &&[_, pos, size, dir] :
         zipper(players, positions, sizes, directions)) {
        if (pos->pos_X + dir->dir_X < 0)
            pos->pos_X = 0;
        if (pos->pos_X + size->size_X > 1280)
            pos->pos_X = 1280 - size->size_X;
        if (pos->pos_Y < 0)
            pos->pos_Y = 0;
        if (pos->pos_Y + size->size_Y > 780 - 130) // 130 for hud..
            pos->pos_Y = 780 - 130 - size->size_Y;
    }
}

void update_grace(Registry &r, sparse_array<SpawnGrace> &graces)
{
    auto time = GetTimePoint();
    auto graces_size = graces.size();

    for (size_t ind = 0; ind != graces_size; ++ind) {
        if (!graces[ind])
            continue;
        if (graces[ind]->creation_time + graces[ind]->time <= time)
            r.remove_component<SpawnGrace>(ind);
    }
}

static size_t getEnemyCount(sparse_array<EnemyCount> &enemyCounts)
{
    for (auto &&[index, enemy] : indexed_zipper(enemyCounts))
        return index;
    throw std::out_of_range("Cannot find enemies");
}

void damages(
    Registry &r, sparse_array<Health> &healt, sparse_array<Damages> &dama,
    size_t i1, size_t i2)
{
    std::cout << "damages" << std::endl;
    if (dama[i2])
        healt[i1]->health -= dama[i2]->damages;
    // std::osyncstream(std::cout) << "User " << i1 << " has taken " <<
    // dama[i2]->damages << " damages. He now have " << healt[i1]->health << "
    // HP." << std::endl;
    if (healt[i1]->health <= 0) {
#ifdef SERVER
        sparse_array<Colision> &col = r.get_components<Colision>();
        sparse_array<EnemyCount> &enemyCounts = r.get_components<EnemyCount>();
        size_t ind = 0;
        try {
            ind = getEnemyCount(enemyCounts);
        } catch (std::out_of_range &e) {
            return;
        }
        if (col[i1]->check_only(Tag::Enemy)) {
            std::cout << "DEAD\n";
            enemyCounts[ind]->leftAlive--;
        }
#endif
        r.kill_entity(r.entity_from_index(i1));
    }

    std::cout << "damages" << std::endl;
    if (dama[i1])
        healt[i2]->health -= dama[i1]->damages;
        // std::osyncstream(std::cout) << "User " << i1 << " has taken " <<
        // dama[i2]->damages << " damages. He now have " << healt[i1]->health <<
        // " HP." << std::endl;
#ifdef SERVER
    if (healt[i2]->health <= 0)
        r.kill_entity(r.entity_from_index(i2));
#endif
}

static void
collect_points(Registry &reg, Score &score, Point &point, Entity pointIndex)
{
    std::cout << "collect" << std::endl;
    score.score += point.point;
    reg.kill_entity(pointIndex);
}

static bool
check_colision(Position &pos1, Position &pos2, Size &size1, Size &size2)
{
    if (pos1.pos_X > pos2.pos_X + size2.size_X)
        return false;
    else if (pos1.pos_Y > pos2.pos_Y + size2.size_Y)
        return false;
    else if (pos2.pos_X > pos1.pos_X + size1.size_X)
        return false;
    else if (pos2.pos_Y > pos1.pos_Y + size1.size_Y)
        return false;
    else
        return true;
}

void colision(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &sizes,
    sparse_array<SpawnGrace> &grace, sparse_array<Damages> &dmgs,
    sparse_array<Health> &healths, sparse_array<Colision> &colisions,
    sparse_array<Point> &points, sparse_array<Score> &scores)
{
    auto pos_size = positions.size();
    for (size_t ind1 = 0; ind1 != pos_size; ++ind1) {
        if (!colisions[ind1] || grace[ind1])
            continue;
        for (size_t ind2 = 0; ind2 != pos_size; ++ind2) {
            if (!colisions[ind2] || grace[ind2] || !sizes[ind2] || !sizes[ind1])
                continue;
            if (!colisions[ind1]) // need to recheck because damages may have
                                  // kill the entity
                continue;
            if (ind2 <= ind1)
                continue;
            if (!check_colision(
                    positions[ind1].value(), positions[ind2].value(),
                    sizes[ind1].value(), sizes[ind2].value()))
                continue;

            if (colisions[ind1]->check_only(Tag::Player) &&
                colisions[ind2]->check(Tag::Damages, Tag::Enemy))
                damages(r, healths, dmgs, ind1, ind2);
            else if (
                colisions[ind1]->check(Tag::Damages, Tag::Enemy) &&
                colisions[ind2]->check_only(Tag::Player))
                damages(r, healths, dmgs, ind1, ind2);
            else if (
                colisions[ind1]->check_only(Tag::Enemy) &&
                colisions[ind2]->check(Tag::Damages, Tag::Player))
                damages(r, healths, dmgs, ind1, ind2);
            else if (
                colisions[ind1]->check(Tag::Damages, Tag::Player) &&
                colisions[ind2]->check_only(Tag::Enemy))
                damages(r, healths, dmgs, ind1, ind2);
            else if (
                colisions[ind1]->check(Tag::Player) &&
                colisions[ind2]->check(Tag::Collactable))
                collect_points(
                    r, scores[ind1].value(), points[ind2].value(), ind2);
            else if (
                colisions[ind1]->check(Tag::Collactable) &&
                colisions[ind2]->check(Tag::Player))
                collect_points(
                    r, scores[ind2].value(), points[ind1].value(), ind1);
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
                    pos->pos_X - (size->size_X / 2.),
                    pos->pos_Y + (size->size_Y / 2.)),
                1.0, 3, 0, Tag::Enemy, Direction(-2, 0));
        }
    }
}

void update_weapon_position(
    Registry &r, sparse_array<Weapon> &weapons,
    sparse_array<Position> &positions)
{
    for (auto &&[weapon, position] : zipper(weapons, positions)) {
        position->pos_X =
            positions[static_cast<size_t>(weapon->owner_id)]->pos_X + 5;
        position->pos_Y =
            positions[static_cast<size_t>(weapon->owner_id)]->pos_Y + 5;
    }
}

void spawn_enemy(
    Registry &r, sparse_array<EnemyCount> &enemiesCount,
    sparse_array<BossCount> &bossCount)
{
    for (auto index = 0; index != enemiesCount.size(); ++index) {
        if (!enemiesCount[index])
            continue;
        Factory f(r);
        auto now = std::chrono::steady_clock::now();
        if (enemiesCount[index]->leftToSpawn > 0 &&
            now > enemiesCount[index]->timeSinceLastSpawn +
                      enemiesCount[index]->delay) {
            enemiesCount[index]->timeSinceLastSpawn =
                std::chrono::steady_clock::now();
            enemiesCount[index]->leftToSpawn--;
            float randomNumber = rand() % (580);
            Position pos = {1280, randomNumber};

            f.create_zorg(pos);
        }
        if (enemiesCount[index]->leftToSpawn <= 0) {
            auto &boss = bossCount[index];
            if (!(boss))
                continue;
            Position pos = {1280, 580 / 2.};
            if (boss->isLastBossAlive == false && boss->leftToSpawn > 0) {
                boss->isLastBossAlive = true;
                boss->leftToSpawn--;
                f.create_boss(pos, 0);
            }
        }
#ifdef SERVER
        if (enemiesCount[index]->leftAlive <= 0 &&
            (r.gameState != 2 && r.gameState != 3)) {
            auto &boss = bossCount[index];
            if (!(boss))
                r.gameState = 2;
            if (boss->leftAlive <= 0)
                r.gameState = 2;
            std::cout << "YOU WIN\n";
        }
#endif
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
                Position(sizes[index]->size_X / 2., sizes[index]->size_Y / 2.);
            for (auto &proj : shooters[index]->infos) {
                factory.create_boss_projectile(
                    *positions[index] + size_as_pos + proj.offset, proj.dir, 0);
            }
            ++shooters[index]->shotCount;
            updateBossProjectiles(
                *shooters[index], positions,
                players); // atm called for each shooter but should check if
                          // it's the boss and the phase of the boss (health)
        }
    }
}
