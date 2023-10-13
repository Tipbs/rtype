#include <raylib.h>
#include "GraphicComponents.hpp"
#include "../../shared/Registry.hpp"
#include "../../shared/Sparse_array.hpp"
#include "GraphicSystems.hpp"

void display(Registry &r,
sparse_array<Position> &positions, 
sparse_array<Size> &size, 
sparse_array<Sprite> &sprite,
sparse_array<MoveAnimCounter> &anim)
{
    BeginDrawing();
    for (size_t ind = 0; ind < sprite.size(); ind++) {
        auto &pos = positions[ind];
        auto &siz = size[ind];
        auto &spri = sprite[ind];
        if (!(pos && siz && spri))
            continue;
        if (anim[ind]) {
            if (anim[ind]->count >= 1.85) {
                sprite[ind]->sprite.x = 4 * sprite[ind]->width_padding;
            } else if (anim[ind]->count <= 0.15) {
                sprite[ind]->sprite.x = 0 * sprite[ind]->width_padding;
            } else if (anim[ind]->count <= 0.85) {
                sprite[ind]->sprite.x = 1 * sprite[ind]->width_padding;
            } else if (anim[ind]->count >= 1.15) {
                sprite[ind]->sprite.x = 3 * sprite[ind]->width_padding;
            } else {
                sprite[ind]->sprite.x = 2 * sprite[ind]->width_padding;
            }
            
        }

        Vector2 Rectpos = {(float)(positions[ind].value().pos_X), (float)positions[ind].value().pos_Y};
        DrawTextureRec(sprite[ind].value().spritesheet, sprite[ind].value().sprite, Rectpos, WHITE);
    }
    EndDrawing();
}

void handle_dir_inputs(Registry &r, 
sparse_array<Direction> &dir,
sparse_array<MoveAnimCounter> &anim, 
sparse_array<Sprite> &sprite)
{
    const double AnimationPad = 0.02;
    double heigh = 1;
    if (anim[1])
        heigh = anim[1]->count;
    Vector2 Moves = {0, 0};

    if (IsKeyDown(KEY_RIGHT)) Moves.x += 1;
    if (IsKeyDown(KEY_LEFT)) Moves.x -= 1;
    if (IsKeyDown(KEY_DOWN) == IsKeyDown(KEY_UP)) {
        heigh = (heigh < 1) ? heigh + AnimationPad : (heigh == 1) ? 1 : heigh - AnimationPad;
    }
    if (IsKeyDown(KEY_UP)) {
        Moves.y -= 1;
        heigh = (heigh >= 2) ? 2 : heigh + (5 * AnimationPad);
    }
    if (IsKeyDown(KEY_DOWN)) {
        Moves.y += 1;
        heigh = (heigh <= 0) ? 0 : heigh - (5 * AnimationPad);
    }

    if (dir[1]) { //1 is the entity num representing the player seen here
        dir[1].value().dir_X = Moves.x;
        dir[1].value().dir_Y = Moves.y;
        r.currentCmd.mutex.lock();
			r.currentCmd.cmd.moved.x += Moves.x;
			r.currentCmd.cmd.moved.y += Moves.y;
        r.currentCmd.mutex.unlock();
    }

    if (anim[1]) {
        anim[1]->count = heigh;
    }
}

// temporary bundle
void create_player(Registry &reg, int id, Position &pos)
{
    Entity const new_entity = reg.spawn_entity();
    Player player(id);
    Size Size(83, 43);
    std::string path = "./gui/ressources/Sprites/r-typesheet42.gif";
    Speed speedo(300);
    Direction diro(0, 0);
    SpawnGrace gra(std::chrono::seconds(1));
    Sprite sprite(path.c_str(), 83, 43, 5, 5);

    reg.add_component(new_entity, std::move(pos));
    reg.add_component(new_entity, std::move(Size));
    reg.add_component(new_entity, std::move(sprite));
    reg.add_component(new_entity, std::move(speedo));
    reg.add_component(new_entity, std::move(diro));
    reg.add_component(new_entity, std::move(gra));
    reg.add_component(new_entity, std::move(player));
}

void updateWithSnapshots(Registry &r, sparse_array<Position> &positions, sparse_array<Player> &players)
{
    auto &net_ents = r.netEnts.ents;

    r.netEnts.mutex.lock();
    for (auto it = net_ents.begin(); it != net_ents.end(); ++it) {
        auto net = *it;
        auto finded = std::find_if(players.begin(), players.end(), [&](std::optional<Player> &player) {
			if (player) return player.value().id == net.id;
        });
        if (finded != players.end()) {
            continue;
        }
        auto pos = Position(net.pos.x, net.pos.y);
        create_player(r, net.id, pos);
        // create entity with info from net ent
        net_ents.erase(it);
    }
    for (auto i = 0; i < positions.size(); ++i) {
        auto &pos = positions[i];
        auto const &player = players[i];
        if (pos && player) {
            auto finded = std::find_if(net_ents.begin(), net_ents.end(), [&] (NetEnt &ent) {
                return ent.id == player.value().id;
			});
            if (finded == net_ents.end())
                continue;
            pos.value().pos_X = finded->pos.x;
            pos.value().pos_Y = finded->pos.y;
        } // pour le moment il n'y a pas l'ajout de nouvelles entités
    }
    r.netEnts.mutex.unlock();
}
