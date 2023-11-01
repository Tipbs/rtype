#include "GraphicSystems.hpp"
#include <cstddef>
#include <cstdlib>
#include <syncstream>
#include <raylib.h>
#include "../../shared/Factory.hpp"
#include "../../shared/indexed_zipper.hpp"
#include "../../shared/Registry.hpp"
#include "../../shared/Sparse_array.hpp"
#include "../../shared/zipper.hpp"
#include "GraphicComponents.hpp"
#include "GraphicSystems.hpp"

std::vector<Color>ColorById {
    BLUE,
    PURPLE,
    RED,
    YELLOW,
    GREEN
};

void draw_rectangle(sparse_array<Rect> &rect, sparse_array<Color> &col)
{
    for (auto &&[recto, color] : zipper(rect, col))
        if (recto->draw_lines)
            DrawRectangleLines(recto->rect.x, recto->rect.y, recto->rect.width, recto->rect.height, color.value());
        else
            DrawRectangle(recto->rect.x, recto->rect.y, recto->rect.width, recto->rect.height, color.value());
}

void draw_text(sparse_array<Text> &text, sparse_array<Position> &positions, sparse_array<Color> &col)
{
    for (auto &&[texto, pos, color] : zipper(text, positions, col))
        DrawText(texto->text.c_str(), pos->pos_X, pos->pos_Y, texto->font_size, color.value());
}

void display(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size,
    sparse_array<Sprite> &sprite, sparse_array<Player> &anim,
    sparse_array<Rectangle> &rectangles, sparse_array<InputField> &inputFields,
    sparse_array<Rect> &rect,
    sparse_array<Color> &col,
    sparse_array<Text> &text)
{
    BeginDrawing();
    auto now = std::chrono::steady_clock::now();
    for (auto &&[ind, pos, siz, spri] :
         indexed_zipper(positions, size, sprite)) {
        Vector2 Rectpos = {
            (float) (positions[ind].value().pos_X),
            (float) positions[ind].value().pos_Y};
        DrawTextureRec(
            sprite[ind].value().spritesheet, sprite[ind].value().sprite,
            Rectpos, WHITE);
    }

    draw_rectangle(rect, col);
    draw_text(text, positions, col);

    EndDrawing();
}

void do_animation(
    Registry &r, sparse_array<Sprite> &sprites,
    sparse_array<Couleur> &couleurs)
{
    for (auto &&[sprite, colors] : zipper(sprites, couleurs)) {
        if (sprite->width_max == 8 && sprite->height_max == 5) { //Ammunition case
            sprite->sprite.y =
                colors->color_id * sprite->height_padding;
            sprite->sprite.x +=
                (sprite->sprite.x / sprite->width_padding ==
                         sprite->width_max - 1
                     ? -6 * sprite->width_padding
                     : sprite->width_padding);
        } else { //Looping sprites frames
            sprite->sprite.x =
                (sprite->sprite.x / sprite->width_padding ==
                         sprite->width_max - 1
                     ? 0
                     : sprite->sprite.x + sprite->width_padding);
        }
    }
}

void do_ship_animation(
    Registry &r, sparse_array<Sprite> &sprites,
    sparse_array<Couleur> &couleurs, sparse_array<Weapon> &weapons)
{
    for (auto &&[weapon] : zipper(weapons)) {
        sprites[(size_t)weapon->owner_id]->sprite.y =
            couleurs[(size_t)weapon->owner_id]->color_id * sprites[(size_t)weapon->owner_id]->height_padding;
        if (weapon->IsShooting)
            sprites[(size_t)weapon->owner_id]->sprite.x = 1 * sprites[(size_t)weapon->owner_id]->width_padding;
        else
            sprites[(size_t)weapon->owner_id]->sprite.x = 0 * sprites[(size_t)weapon->owner_id]->width_padding;
    }
}

void make_infinite_background(
    Registry &r, sparse_array<Position> &positions,
    sparse_array<Size> &sizes, sparse_array<Backgrounds> &bg)
{    
    for (auto &&[position, size, back] : zipper(positions, sizes, bg)) {
        if (position->pos_X < -2 * size->size_X)
            position->pos_X += 2 * size->size_X;
    }
}

void handle_dir_inputs(
    Registry &r, sparse_array<Direction> &dir, sparse_array<Player> &players,
    sparse_array<Sprite> &sprite, sparse_array<Speed> &speeds,
    sparse_array<Couleur> &colors)
{
    for (auto &&[diro, player, sprit, spe, color] :
         zipper(dir, players, sprite, speeds, colors)) {
        Vector2 moves = {0, 0};
        double speedScale = 1;

        if (IsKeyDown(KEY_LEFT_SHIFT))
            speedScale /= 2;

        if (IsKeyDown(KEY_RIGHT))
            moves.x += 1;
        if (IsKeyDown(KEY_LEFT))
            moves.x -= 1;
        if (IsKeyDown(KEY_UP))
            moves.y -= 1;
        if (IsKeyDown(KEY_DOWN))
            moves.y += 1;
        diro->dir_X = moves.x * speedScale;
        diro->dir_Y = moves.y * speedScale;
        // speeds[1]->speed = speed;
        r.currentCmd.mutex.lock();
        r.currentCmd.cmd.moved.x += moves.x * GetFrameTime() * speedScale;
        r.currentCmd.cmd.moved.y += moves.y * GetFrameTime() * speedScale;
        r.currentCmd.mutex.unlock();
        if (IsKeyPressed(KEY_C))
            color->color_id = color->color_id == 4 ? 0 : color->color_id + 1;
        break;
    }
}

void handle_shoot_inputs(
    Registry &r, sparse_array<Couleur> &colors, sparse_array<Size> &sizes,
    sparse_array<Weapon> &weapons, sparse_array<Position> &positions)
{
    Factory factory(r);

    for (auto &&[weapon] : zipper(weapons)) {
        size_t owner_id = static_cast<size_t>(weapon->owner_id);
        if (IsKeyDown(KEY_SPACE)) {
            weapon->IsShooting = true;
            weapon->current_charge +=
                (weapon->current_charge >= 3) ? 0 : 5 * GetFrameTime();
        }
        if (IsKeyReleased(KEY_SPACE)) {
            weapon->IsShooting = false;
            factory.create_ammo(
                Position(
                    positions[owner_id]->pos_X +
                        (float) sizes[owner_id]->size_X,
                    positions[owner_id]->pos_Y +
                        (float) sizes[owner_id]->size_Y / 2),
                weapon->current_charge, colors[owner_id]->color_id);
            r.currentCmd.mutex.lock();
            r.currentCmd.cmd.setAttack(weapon->current_charge);
            r.currentCmd.mutex.unlock();
            weapon->current_charge = 1.;
        }
        break;
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
    Factory factory(r);

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
        factory.create_player(net.id, pos);
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
                factory.create_ammo(
                    Position(
                        pos->pos_X + (float) size->size_X,
                        pos->pos_Y + (float) size->size_Y / 2),
                    finded->attackState, player->color_id);
                // pour le moment le tir ne marche qu'avec les players
                // vu que create_ammo demande un color_id
            }
        }
    }
    net_ents.clear();
    r.netEnts.mutex.unlock();
}

// void updateHUD(
//     Registry &r, sparse_array<Weapon> &weap,
//     sparse_array<Couleur> &col,
//     sparse_array<HUD> &hud)
// {
//     for (auto &&[weapon] : zipper(weap)) {
//         for (auto &&[hudy] : zipper(hud)) {
//             hudy->charge = weapon->current_charge;
//             hudy->score += 1;
//             hudy->color_id = col[(size_t)weapon->owner_id]->color_id;
//         }
//     }
// }
