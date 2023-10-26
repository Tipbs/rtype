#include "../../shared/Bundle.hpp"
#include "../../shared/Registry.hpp"
#include "../../shared/zipper.hpp"

void synchronize(
    Registry &reg, sparse_array<Direction> &directions,
    sparse_array<Speed> &spe, sparse_array<Position> &positions,
    sparse_array<Size> &sizes, sparse_array<Player> &players)
{
    for (auto &player_cmds : reg.user_cmds) {
        auto &dir = directions[player_cmds.first];
        const auto &posi = positions[player_cmds.first];
        const auto &sizo = sizes[player_cmds.first];
        auto &player = players[player_cmds.first];
        player->IsShooting = false;
        player->current_charge = 0;
        for (auto &cmds : player_cmds.second) {
            dir->dir_X += cmds.moved.x;
            dir->dir_Y += cmds.moved.y;
            if (cmds.attacking) {
                create_ammo(
                    reg,
                    Position(
                        posi->pos_X + (float) sizo->size_X,
                        posi->pos_Y + (float) sizo->size_Y / 2),
                    cmds.attackScale, player->color_id);
                player->IsShooting = cmds.attacking;
                player->current_charge = cmds.attackScale;
            }
        }
    }
}

void extract(
    Registry &reg, sparse_array<Position> &positions,
    sparse_array<Speed> &speeds, sparse_array<Player> &players,
    sparse_array<NetworkedEntity> &ents)
{
    for (size_t ind = 0; ind < positions.size(); ind++) {
        auto &pos = positions[ind];
        auto &spe = speeds[ind];
        if (!(pos && spe && ents[ind]))
            continue;
        NetEnt tmp;
        tmp.id = ind;
        tmp.pos.x = pos->pos_X;
        tmp.pos.y = pos->pos_Y;
        auto &player = players[ind];
        if (player) {
            tmp.attacking = player->IsShooting;
            tmp.attackState = player->current_charge;
            reg._netent.push_back(tmp);
        }
    }
}

void resetPlayersDir(
    Registry &r, sparse_array<Player> &players,
    sparse_array<Direction> &directions)
{
    for (auto &&[_, diro] : zipper(players, directions)) {
        // std::osyncstream(std::cout) << "y = " << pos->pos_Y << "  x = " <<
        // pos->pos_X << std::endl;
        diro->dir_X = 0;
        diro->dir_Y = 0;
    }
}
