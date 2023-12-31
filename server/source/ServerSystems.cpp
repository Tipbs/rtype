#include "../../shared/Component.hpp"
#include "../../shared/Factory.hpp"
#include "../../shared/indexed_zipper.hpp"
#include "../../shared/Registry.hpp"
#include "../../shared/zipper.hpp"

size_t getEntityWeapon(sparse_array<Weapon> &weapons, size_t playerId)
{
    size_t finded_weapon = -1;
    for (auto &&[index, weapon] : indexed_zipper(weapons))
        if ((size_t) weapon->owner_id == playerId)
            return index;
    return finded_weapon;
}

void synchronize(
    Registry &reg, sparse_array<Direction> &directions,
    sparse_array<Speed> &spe, sparse_array<Position> &positions,
    sparse_array<Size> &sizes, sparse_array<Weapon> &weapons,
    sparse_array<Player> &players)
{
    Factory factory(reg);

    for (auto &player_cmds : reg.user_cmds) {
        auto &dir = directions[player_cmds.first];
        const auto &posi = positions[player_cmds.first];
        const auto &sizo = sizes[player_cmds.first];
        auto &player = players[player_cmds.first];
        if (!player)
            continue;
        auto ent_weapon = getEntityWeapon(weapons, player_cmds.first);
        if (ent_weapon == -1)
            continue;
        auto &weapon = weapons[ent_weapon];
        weapon->IsShooting = false;
        weapon->current_charge = 0;
        for (auto &cmds : player_cmds.second) {
            dir->dir_X += cmds.moved.x;
            dir->dir_Y += cmds.moved.y;
            if (cmds.attacking) {
                factory.create_ammo(
                    Position(
                        posi->pos_X + (float) sizo->size_X,
                        posi->pos_Y + (float) sizo->size_Y / 2),
                    cmds.attackScale, player->color_id, Tag::Player);
                weapon->IsShooting = cmds.attacking;
                weapon->current_charge = cmds.attackScale;
            }
        }
    }
}

void extract(
    Registry &reg, sparse_array<Position> &positions,
    sparse_array<Speed> &speeds, sparse_array<Weapon> &weapons,
    sparse_array<NetworkedEntity> &ents, sparse_array<Direction> &directions,
    sparse_array<ProjectileShooter> &shooters)
{
    if (reg.gameState != 1) {
        NetEnt tmp;
        if (reg.gameState == 2)
            tmp.type = EntityType::Win;
        if (reg.gameState == 3)
            tmp.type = EntityType::Lose;
        reg._netent.push_back(tmp);
        return;
    }
    for (auto &&[ind, pos, ent_id, dir] :
         indexed_zipper(positions, ents, directions)) {
        NetEnt tmp;
        tmp.type = ents[ind]->_type;
        tmp.id = ind;
        tmp.pos.x = pos->pos_X;
        tmp.pos.y = pos->pos_Y;
        tmp.dir = {dir->dir_X, dir->dir_Y};
        auto weapon_ind = getEntityWeapon(weapons, ind);
        if (weapon_ind != -1) {
            auto &weapon = weapons[weapon_ind];
            tmp.attacking = weapon->IsShooting;
            tmp.attackState = weapon->current_charge;
        }
        if (shooters[ind]) {
            NetEnt shooter_net;
            shooter_net.id = ind;
            shooter_net.type = EntityType::ProjectileShooter;
            shooter_net.dir.x = shooters[ind]->shotCount;
            reg._netent.push_back(shooter_net);
        }
        reg._netent.push_back(tmp);
    }
}

void resetPlayersDir(
    Registry &r, sparse_array<Player> &players,
    sparse_array<Direction> &directions)
{
    for (auto &&[_, diro] : zipper(players, directions)) {
        diro->dir_X = 0;
        diro->dir_Y = 0;
    }
}

void kill_zord(
    Registry &r, sparse_array<EnemyCount> &enemyCount,
    sparse_array<BossCount> &bosses, sparse_array<NetworkedEntity> &ents,
    sparse_array<Position> &positions, sparse_array<Health> &healths)
{
    for (auto &&[ind, net, pos, hp] :
         indexed_zipper(ents, positions, healths)) {
        if (net->_type == EntityType::Boss && hp->health <= 0) {
            std::cout << "killing BOSS\n";
            r.kill_entity(ind);
            for (auto &&[bossCount] : zipper(bosses)) {
                bossCount->leftAlive--;
                bossCount->isLastBossAlive = false;
            }
        }
        if (net->_type == EntityType::Enemy &&
            (pos->pos_X < 0 || hp->health <= 0)) {
            for (auto &&[enemy] : zipper(enemyCount))
                enemy->leftAlive--;
            std::cout << "killing zorg\n";
            r.kill_entity(ind);
        }
    }
}
