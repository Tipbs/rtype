#include "GraphicSystems.hpp"
#include <cstddef>
#include <cstdlib>
#include <syncstream>
#include <raylib.h>
#include "../../shared/Bundle.hpp"
#include "../../shared/indexed_zipper.hpp"
#include "../../shared/Registry.hpp"
#include "../../shared/Sparse_array.hpp"
#include "../../shared/zipper.hpp"
#include "GraphicComponents.hpp"
#include "GraphicSystems.hpp"

void display(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size,
    sparse_array<Sprite> &sprite, sparse_array<Player> &anim,
    sparse_array<Rectangle> &rectangles, sparse_array<InputField> &inputFields)
{
    BeginDrawing();
    for (auto &&[ind, pos, siz, spri] :
         indexed_zipper(positions, size, sprite)) {
        if (!(pos && siz && spri))
            continue;
        if (sprite[ind]->width_max == 8 && sprite[ind]->height_max == 5) {
            sprite[ind]->sprite.y =
                sprite[ind]->color_id * sprite[ind]->height_padding;
            sprite[ind]->sprite.x +=
                (sprite[ind]->sprite.x / sprite[ind]->width_padding ==
                         sprite[ind]->width_max - 1
                     ? 0 // - sprite[ind]->width_padding)
                     : sprite[ind]->width_padding);
        }
        if (anim[ind]) {
            sprite[ind]->sprite.y =
                anim[ind]->color_id * sprite[ind]->height_padding;
            if (anim[ind]->IsShooting)
                sprite[ind]->sprite.x = 1 * sprite[ind]->width_padding;
            else
                sprite[ind]->sprite.x = 0 * sprite[ind]->width_padding;
        }

        Vector2 Rectpos = {
            (float) (positions[ind].value().pos_X),
            (float) positions[ind].value().pos_Y};
        DrawTextureRec(
            sprite[ind].value().spritesheet, sprite[ind].value().sprite,
            Rectpos, WHITE);
    }
    EndDrawing();
}

void handle_dir_inputs(
    Registry &r, sparse_array<Direction> &dir, sparse_array<Player> &anim,
    sparse_array<Sprite> &sprite, sparse_array<Speed> &speeds,
    sparse_array<Current_Player> &currents)
{
    for (auto &&[diro, anima, sprit, spe, current] :
         zipper(dir, anim, sprite, speeds, currents)) {
        const double AnimationPad = 0.02;
        double heigh = 1;
        heigh = anima->count;
        Vector2 moves = {0, 0};
        double speedScale = 1;

        if (IsKeyDown(KEY_RIGHT))
            moves.x += 1;
        if (IsKeyDown(KEY_LEFT))
            moves.x -= 1;
        if (IsKeyDown(KEY_LEFT_SHIFT))
            speedScale /= 2;

        if (IsKeyDown(KEY_DOWN) == IsKeyDown(KEY_UP)) {
            heigh = (heigh < 1)    ? heigh + AnimationPad
                    : (heigh == 1) ? 1
                                   : heigh - AnimationPad;
        } else if (IsKeyDown(KEY_UP)) {
            moves.y -= 1; // 1;
            heigh = (heigh >= 2) ? 2 : heigh + (5 * AnimationPad);
        } else if (IsKeyDown(KEY_DOWN)) {
            moves.y += 1;
            heigh = (heigh <= 0) ? 0 : heigh - (5 * AnimationPad);
        }
        diro->dir_X = moves.x * speedScale;
        diro->dir_Y = moves.y * speedScale;
        // speeds[1]->speed = speed;
        r.currentCmd.mutex.lock();
        r.currentCmd.cmd.moved.x += moves.x * GetFrameTime() * speedScale;
        r.currentCmd.cmd.moved.y += moves.y * GetFrameTime() * speedScale;
        r.currentCmd.mutex.unlock();
        if (IsKeyPressed(KEY_C))
            anima->color_id = anima->color_id == 4 ? 0 : anima->color_id + 1;
        anima->count = heigh;
        break;
    }
}

void handle_shoot_inputs(
    Registry &r, sparse_array<Player> &anim, sparse_array<Position> &pos,
    sparse_array<Size> &siz, sparse_array<Current_Player> &current)
{
    for (auto &&[index, anima, posi, sizo, _] :
         indexed_zipper(anim, pos, siz, current)) {
        if (IsKeyDown(KEY_SPACE)) {
            anima->IsShooting = true;
            anima->current_charge +=
                (anima->current_charge >= 3) ? 0 : 5 * GetFrameTime();
        }
        if (IsKeyReleased(KEY_SPACE)) {
            anima->IsShooting = false;
            create_ammo(
                r,
                Position(
                    posi->pos_X + (float) sizo->size_X,
                    posi->pos_Y + (float) sizo->size_Y / 2),
                anima->current_charge, anima->color_id);
            r.currentCmd.mutex.lock();
            r.currentCmd.cmd.setAttack(anim[index]->current_charge);
            r.currentCmd.mutex.unlock();
            anim[index]->current_charge = 1.;
        }
        break;
    }
}

void play_sound(Registry &r, sparse_array<SoundComponent> &sounds)
{
    for (auto &&[sound] : zipper(sounds)) {
        for (auto &sfx : sound->sounds) {
            if (std::get<1>(sfx) == true && std::get<2>(sfx) == false)
                PlaySound(std::get<0>(sfx));
            std::get<2>(sfx) = std::get<1>(sfx);
        }
    }
}

void hadle_text_inputs(
    Registry &r, sparse_array<InputField> &inputFields,
    sparse_array<Rectangle> &rectangles)
{
    for (auto &&[inputField, rectangle] : zipper(inputFields, rectangles)) {
        if (CheckCollisionPointRec(GetMousePosition(), rectangle.value())) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            int key = GetCharPressed();
            int letterCount = 0;

            // Check if more characters have been pressed on the
            // same frame
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (letterCount < 16)) {
                    inputField->field[letterCount] = (char) key;
                    inputField->field[letterCount + 1] =
                        '\0'; // Add null terminator at the end of
                              // the string.
                    letterCount++;
                }
                key = GetCharPressed(); // Check next character in
                                        // the queue
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                letterCount--;
                if (letterCount < 0)
                    letterCount = 0;
                inputField->field[letterCount] = '\0';
            }
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
    }
}

void make_infinite_background(
    Registry &r, sparse_array<Position> &pos, sparse_array<Size> &siz)
{
    if (pos[0] && siz[0]) {

        // BG going to the Left
        if (pos[0]->pos_X < -2 * siz[0]->size_X)
            pos[0]->pos_X += 2 * siz[0]->size_X;

        // BG going Upwards
        // if (pos[0]->pos_Y < -siz[0]->size_Y)
        //     pos[0]->pos_Y += siz[0]->size_Y;

        // BG going to the Right
        // if (pos[0]->pos_X > 0)
        //     pos[0]->pos_X -= siz[0]->size_X;

        // BG going Downwards
        // if (pos[0]->pos_Y > 0)
        //     pos[0]->pos_Y -= siz[0]->size_Y;
    }
}

void killDeadEntities(Registry &r, sparse_array<NetworkedEntity> &entities)
{
    auto &net_ents = r.netEnts.ents;

    for (auto &&[index, _] : indexed_zipper(entities)) {
        auto finded =
            std::find_if(net_ents.begin(), net_ents.end(), [&](NetEnt &ent) {
                return ent.id == entities[index]->id;
            });
        if (finded == net_ents.end()) {
            r.kill_entity(index);
            std::cout << "killing entity " << index << std::endl;
        }
    }
}

void updateWithSnapshots(
    Registry &r, sparse_array<Position> &positions,
    sparse_array<NetworkedEntity> &entities, sparse_array<Speed> &speeds,
    sparse_array<Current_Player> &currents, sparse_array<Size> &sizes,
    sparse_array<Player> &players)
{
    auto &net_ents = r.netEnts.ents;

    r.netEnts.mutex.lock();
    for (auto it = net_ents.begin(); it != net_ents.end(); ++it) {
        auto &net = *it;
        auto finded = std::find_if(
            entities.begin(), entities.end(),
            [&](std::optional<NetworkedEntity> &ent) {
                if (ent)
                    return ent->id == net.id;
                return false;
            });
        if (finded != entities.end())
            continue;
        std::cout << "id: " << net.id << std::endl;
        auto pos = Position(net.pos.x, net.pos.y);
        create_player(r, net.id, pos);
        std::cout << "Creating player\n";
        it = net_ents.erase(it);
        if (it == net_ents.end())
            break;
    }
    for (size_t i = 0; i < positions.size(); ++i) {
        auto &pos = positions[i];
        auto const &entity = entities[i];
        auto const &current = currents[i];
        auto const &size = sizes[i];
        auto const &player = players[i];
        if (pos && entity) {
            auto finded = std::find_if(
                net_ents.begin(), net_ents.end(), [&](NetEnt &net_ent) {
                    return net_ent.id == entity.value().id;
                });
            if (finded == net_ents.end())
                continue;
            if (current && std::abs(finded->pos.x - pos.value().pos_X) < 30.0 &&
                std::abs(finded->pos.y - pos.value().pos_Y) < 30.0) {
                continue;
            }
            pos.value().pos_X = finded->pos.x;
            pos.value().pos_Y = finded->pos.y;
            if (!current && player && finded->attacking) {
                create_ammo(
                    r,
                    Position(
                        pos->pos_X + (float) size->size_X,
                        pos->pos_Y + (float) size->size_Y / 2),
                    finded->attackState, player->color_id);
            }
        }
    }
    net_ents.clear();
    r.netEnts.mutex.unlock();
}

void handle_music(Registry &r, sparse_array<MusicComponent> &musics)
{
    for (auto &&[index, music] : indexed_zipper(musics)) {
        if (music->type == MusicFx::Battle) {
            if (IsSoundPlaying(music->music) == false) {
                SetSoundVolume(music->music, 0.3);
                PlaySound(music->music);
            }
        }
    }
}
