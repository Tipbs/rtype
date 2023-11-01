#include "../../shared/Component.hpp"
#include "../../shared/Factory.hpp"
#include "../../shared/indexed_zipper.hpp"
#include "../../shared/Registry.hpp"
#include "../../shared/zipper.hpp"

size_t getEntityWeapon(sparse_array<Weapon> &weapons, size_t playerId)
{
    size_t finded_weapon = -1;
    for (auto&& [index, weapon] : indexed_zipper(weapons)) {
        if ((size_t)weapon->owner_id == playerId) {
            return index;
        }
    }
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
        auto &weapon = weapons[getEntityWeapon(weapons, player_cmds.first)];
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
                    cmds.attackScale, player->color_id);
                weapon->IsShooting = cmds.attacking;
                weapon->current_charge = cmds.attackScale;
            }
        }
    }
}

void extract(
    Registry &reg, sparse_array<Position> &positions,
    sparse_array<Speed> &speeds, sparse_array<Weapon> &weapons,
    sparse_array<NetworkedEntity> &ents)
{
    for (auto &&[ind, pos, _] : indexed_zipper(positions, ents)) {
        auto &spe = speeds[ind];
        if (!(pos && spe && ents[ind]))
            continue;
        NetEnt tmp;
        tmp.type = ents[ind]->_type;
        tmp.id = ind;
        tmp.pos.x = pos->pos_X;
        tmp.pos.y = pos->pos_Y;
        auto &weapon = weapons[getEntityWeapon(weapons, ind)];
        if (weapon) {
            tmp.attacking = weapon->IsShooting;
            tmp.attackState = weapon->current_charge;
        }
        if (tmp.type == EntityType::Player)
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
