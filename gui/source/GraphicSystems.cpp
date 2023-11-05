#include "GraphicSystems.hpp"
#include <cstddef>
#include <cstdlib>
#include <syncstream>
#include <raylib.h>
#include <sys/types.h>
#include "../../shared/Factory.hpp"
#include "../../shared/indexed_zipper.hpp"
#include "../../shared/Registry.hpp"
#include "../../shared/Sparse_array.hpp"
#include "../../shared/zipper.hpp"
#include "GraphicComponents.hpp"
#include "GraphicSystems.hpp"

std::vector<Color> ColorById {BLUE, PURPLE, RED, YELLOW, GREEN};

void draw_rectangle(sparse_array<Rect> &rect, sparse_array<Color> &col)
{
    for (auto &&[recto, color] : zipper(rect, col))
        if (recto->draw_lines)
            DrawRectangleLines(
                recto->rect.x, recto->rect.y, recto->rect.width,
                recto->rect.height, color.value());
        else
            DrawRectangle(
                recto->rect.x, recto->rect.y, recto->rect.width,
                recto->rect.height, color.value());
}

void draw_text(
    sparse_array<Text> &text, sparse_array<Position> &positions,
    sparse_array<Color> &col)
{
    for (auto &&[texto, pos, color] : zipper(text, positions, col))
        DrawText(
            texto->text.c_str(), pos->pos_X, pos->pos_Y, texto->font_size,
            color.value());
}

void display(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size,
    sparse_array<Sprite> &sprite, sparse_array<Player> &anim,
    sparse_array<Rectangle> &rectangles, sparse_array<InputField> &inputFields,
    sparse_array<Rect> &rect, sparse_array<Color> &col,
    sparse_array<Text> &text, sparse_array<MenuFields> &menuFields, sparse_array<CustomText> &texts,
    sparse_array<CanBeSelected> &selectables)
{
    BeginDrawing();
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

    for (auto &&[inputField, rectangle]: zipper(inputFields, rectangles)) {
        DrawRectangleRec(rectangle.value(), BLANK);
        DrawRectangleLinesEx(rectangle.value(), 3, RED);
        DrawText("IP du Serveur", (int)rectangle->x + 50, (int)rectangle->y - 48, 40, MAROON);
        DrawText(inputField->field.c_str(), (int)rectangle->x + 5, (int)rectangle->y + 8, 40, MAROON);
    }
    for (auto &&[text, position, selectable]: zipper(texts, positions, selectables)) {
        DrawTextureRec(text->texture, {
                0.f,
                0.f,
                550,
                (float) MeasureTextEx(text->font, text->str.c_str(), 100, 1.f).y
            }, {
                (float) (position->pos_X - (float) MeasureTextEx(text->font, text->str.c_str(), 50, 1.f).x * 1.5),
                (float) (position->pos_Y - (float) MeasureTextEx(text->font, text->str.c_str(), 50, 1.f).y + 35),
            }, WHITE);
        DrawRectangleLinesEx({
            (float) (position->pos_X - (float) MeasureTextEx(text->font, text->str.c_str(), 50, 1.f).x * 1.5),
            (float) (position->pos_Y - (float) MeasureTextEx(text->font, text->str.c_str(), 50, 1.f).y + 35),
            550,
            MeasureTextEx(text->font, text->str.c_str(), 50, 1.f).y * 2,
        }, 5, selectable->isSelected ? WHITE : DARKBLUE);
        DrawTextEx(text->font, text->str.c_str(),
            { (float) position->pos_X - (float) MeasureTextEx(text->font, text->str.c_str(), 50, 1.f).x,
            (float) position->pos_Y + 10  }, 70, 1.0f, WHITE);
    }

    EndDrawing();
}

void handle_menu_inputs(
    Registry &r, sparse_array<MenuFields> &menuFields,
    sparse_array<Rectangle> &rectangles, sparse_array<CustomText> &texts)
{
/*    for (auto &&[menuField, rectangle]: zipper(menuFields, rectangles)) {
        //menuField->mouseOnText = CheckCollisionPointRec(GetMousePosition(), rectangle.value());
        int key = GetCharPressed();
        while (key > 0) {
            std::cout << key << std::endl;
            if (key == KeyboardKey::KEY_DOWN) {
                ++menuField.value().actual_field;
            } else if (key == KeyboardKey::KEY_UP) {
                --menuField.value().actual_field;
            }
            key = GetCharPressed();
        }
    }*/
}

void selectable_text(
    Registry &r, sparse_array<CustomText> &texts, sparse_array<Position> &positions,
    sparse_array<CanBeSelected> &selectables)
{
    ssize_t tmp = -1;

    for (auto &&[text, position, selectable]: zipper(texts, positions, selectables)) {
        if (IsKeyPressed(KEY_UP) && selectable->isSelected) {
                selectable->isSelected = false;
                tmp = text->index;
            if (text->index > 0)
                tmp -= 1;         
            else
                tmp = 2;
        } else if (IsKeyPressed(KEY_DOWN) && selectable->isSelected) {
            selectable->isSelected = false;
            tmp = text->index;
            if (text->index < 2)
                tmp += 1;
            else
                tmp = 0;
        }
    }
    for (auto &&[text, position, selectable]: zipper(texts, positions, selectables)) {
        if (tmp == text->index) {
            selectable->isSelected = true;
        }
        if (IsKeyPressed(KEY_SPACE) && selectable->isSelected) {
            selectable->function();
        }
    }
}

void do_animation(
    Registry &r, sparse_array<Sprite> &sprites, sparse_array<Couleur> &couleurs)
{
    for (auto &&[sprite, colors] : zipper(sprites, couleurs)) {
        if (sprite->width_max == 8 &&
            sprite->height_max == 5) { // Ammunition case
            sprite->sprite.y = colors->color_id * sprite->height_padding;
            sprite->sprite.x +=
                (sprite->sprite.x / sprite->width_padding ==
                         sprite->width_max - 1
                     ? -6 * sprite->width_padding
                     : sprite->width_padding);
        } else { // Looping sprites frames
            if (sprite->width_max == 2 && sprite->height_max == 5)
                continue;
            sprite->sprite.x =
                (sprite->sprite.x / sprite->width_padding ==
                         sprite->width_max - 1
                     ? 0
                     : sprite->sprite.x + sprite->width_padding);
        }
    }
}

void do_ship_animation(
    Registry &r, sparse_array<Sprite> &sprites, sparse_array<Couleur> &couleurs,
    sparse_array<Weapon> &weapons, sparse_array<Current_Player> &cur_player)
{
    for (auto &&[weapon] : zipper(weapons)) {
        if (!cur_player[(size_t) weapon->owner_id])
            continue;
        sprites[(size_t) weapon->owner_id]->sprite.y =
            couleurs[(size_t) weapon->owner_id]->color_id *
            sprites[(size_t) weapon->owner_id]->height_padding;
        if (weapon->IsShooting)
            sprites[(size_t) weapon->owner_id]->sprite.x =
                1 * sprites[(size_t) weapon->owner_id]->width_padding;
        else
            sprites[(size_t) weapon->owner_id]->sprite.x =
                0 * sprites[(size_t) weapon->owner_id]->width_padding;
    }
}

void make_infinite_background(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &sizes,
    sparse_array<Backgrounds> &bg)
{
    for (auto &&[position, size, back] : zipper(positions, sizes, bg))
        if (position->pos_X < -2 * size->size_X)
            position->pos_X += 2 * size->size_X;
}

void handle_dir_inputs(
    Registry &r, sparse_array<Direction> &dir,
    sparse_array<Current_Player> &players, sparse_array<Sprite> &sprite,
    sparse_array<Speed> &speeds, sparse_array<Couleur> &colors)
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

size_t getSoundManager(sparse_array<SoundManager> &managers)
{
    for (auto &&[index, manager] : indexed_zipper(managers))
        return index;
    return -1;
}

void add_sound(std::string path, sparse_array<SoundManager> &sound)
{
    size_t ind = getSoundManager(sound);
    Sound sfx = LoadSound("./gui/ressources/Audio/lazer.wav");
    sound[ind]->sounds.push_back(sfx);
};

void handle_shoot_inputs(
    Registry &r, sparse_array<Couleur> &colors, sparse_array<Size> &sizes,
    sparse_array<Weapon> &weapons, sparse_array<Position> &positions)
{
    Factory factory(r);

    for (auto &&[ind, weapon] : indexed_zipper(weapons)) {
        size_t owner_id = static_cast<size_t>(weapon->owner_id);
        if (IsKeyDown(KEY_SPACE)) {
            weapon->IsShooting = true;
            weapon->current_charge +=
                (weapon->current_charge >= 3) ? 0 : 5 * GetFrameTime();
        } else if (IsKeyReleased(KEY_SPACE)) {
            weapon->IsShooting = false;
            factory.create_ammo(
                Position(
                    positions[owner_id]->pos_X +
                        (float) sizes[owner_id]->size_X,
                    positions[owner_id]->pos_Y +
                        (float) sizes[owner_id]->size_Y / 2),
                weapon->current_charge, colors[owner_id]->color_id);
            r.currentCmd.mutex.lock();
            r.currentCmd.cmd.setAttack(weapons[ind]->current_charge);
            r.currentCmd.mutex.unlock();
            add_sound(
                "./gui/ressources/Audio/lazer.wav",
                r.get_components<SoundManager>());
            weapons[ind]->current_charge = 1.;
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
    // auto size = entities.size();

    for (auto &&[index, _] : indexed_zipper(entities)) {
        auto finded =
            std::find_if(net_ents.begin(), net_ents.end(), [&](NetEnt &ent) {
                return ent.id == entities[index]->id;
            });
        if (finded == net_ents.end()) {
            std::cout << "netent: " << net_ents[0].id << " "
                      << entities[index]->id << std::endl;
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
    if (!r.netEnts.ents.empty())
        killDeadEntities(r, entities);
    // std::cout << "r.netEnts size: " << r.netEnts.ents.size() << std::endl;
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
        factory.create_netent(net.type, net);
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
                std::abs(finded->pos.y - pos.value().pos_Y) <
                    30.0) // doesn't rollback if the server pos is close enough
                continue;
            pos->pos_X = finded->pos.x;
            pos->pos_Y = finded->pos.y;
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

void update_score_text(
    Registry &r, sparse_array<Score> &scores,
    sparse_array<ScoreText> &scoreTexts, sparse_array<Text> &texts)
{
    for (auto &&[scoreText, text] : zipper(scoreTexts, texts)) {
        text->text =
            std::to_string(scores[static_cast<size_t>(scoreText->from)]->score);
    }
}

void update_charge_rect(
    Registry &r, sparse_array<Weapon> &weapons,
    sparse_array<ChargeRect> &chargeRects, sparse_array<Rect> &rects)
{
    for (auto &&[chargeRect, rect] : zipper(chargeRects, rects)) {
        std::cout
            << weapons[static_cast<size_t>(chargeRect->from)]->current_charge
            << std::endl;
        rect->rect.width =
            (weapons[static_cast<size_t>(chargeRect->from)]->current_charge -
             1) *
            chargeRect->maxWidth;
    }
}

void play_sound(Registry &r, sparse_array<SoundManager> &sounds)
{
    for (auto &&[sound] : zipper(sounds)) {
        for (auto &sfx : sound->sounds)
            PlaySound(sfx);
        sound->sounds.clear();
    }
}

void handle_music(Registry &r, sparse_array<MusicComponent> &musics)
{
    for (auto &&[ind, music] : indexed_zipper(musics)) {
        auto &ost = music->musics[music->context];
        if (IsSoundPlaying(ost) == false) {
            SetSoundVolume(ost, 0.3);
            PlaySound(ost);
        }
    }
}
