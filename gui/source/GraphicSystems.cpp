#include "GraphicSystems.hpp"
#include <cstdlib>
#include <raylib.h>
#include "../../shared/indexed_zipper.hpp"
#include "../../shared/Registry.hpp"
#include "../../shared/Sparse_array.hpp"
#include "../../shared/zipper.hpp"
#include "GraphicComponents.hpp"

void display(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size,
    sparse_array<Sprite> &sprite, sparse_array<Player> &anim)
{
    BeginDrawing();
    for (auto &&[ind, pos, siz, spri] :
         indexed_zipper(positions, size, sprite)) {
        if (!(pos && siz && spri))
            continue;
        if (sprite[ind]->width_max == 8 && sprite[ind]->height_max == 1) {
            sprite[ind]->sprite.x +=
                (sprite[ind]->sprite.x / sprite[ind]->width_padding ==
                         sprite[ind]->width_max - 1
                     ? 0 // - sprite[ind]->width_padding)
                     : sprite[ind]->width_padding);
        }
        if (anim[ind]) {
            sprite[ind]->sprite.y =
                anim[ind]->color_id * sprite[ind]->height_padding;
            if (anim[ind]->count >= 1.85)
                sprite[ind]->sprite.x = 4 * sprite[ind]->width_padding;
            else if (anim[ind]->count <= 0.15)
                sprite[ind]->sprite.x = 0 * sprite[ind]->width_padding;
            else if (anim[ind]->count <= 0.85)
                sprite[ind]->sprite.x = 1 * sprite[ind]->width_padding;
            else if (anim[ind]->count >= 1.15)
                sprite[ind]->sprite.x = 3 * sprite[ind]->width_padding;
            else
                sprite[ind]->sprite.x = 2 * sprite[ind]->width_padding;
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
    sparse_array<Sprite> &sprite)
{
    for (auto &&[ind, diro, anima, sprit] : indexed_zipper(dir, anim, sprite)) {
        if (!(diro && anima && sprit))
            continue;
        const double AnimationPad = 0.02;
        double heigh = 1;
        if (anim[ind]->id == ind) {
            heigh = anim[ind]->count;
            Vector2 Moves = {0, 0};

            if (IsKeyDown(KEY_RIGHT))
                Moves.x += 1;
            if (IsKeyDown(KEY_LEFT))
                Moves.x -= 1;

            if (IsKeyDown(KEY_DOWN) == IsKeyDown(KEY_UP)) {
                heigh = (heigh < 1)    ? heigh + AnimationPad
                        : (heigh == 1) ? 1
                                       : heigh - AnimationPad;
            } else if (IsKeyDown(KEY_UP)) {
                Moves.y -= rand(); // 1;
                heigh = (heigh >= 2) ? 2 : heigh + (5 * AnimationPad);
            } else if (IsKeyDown(KEY_DOWN)) {
                Moves.y += 1;
                heigh = (heigh <= 0) ? 0 : heigh - (5 * AnimationPad);
            }

            if (IsKeyPressed(KEY_C)) {
                anim[ind]->color_id =
                    anim[ind]->color_id == 4 ? 0 : anim[ind]->color_id + 1;
            }

            if (dir[ind]) { // 1 is the entity num representing the player seen
                            // here
                dir[ind].value().dir_X = Moves.x;
                dir[ind].value().dir_Y = Moves.y;
            }

            if (anim[ind])
                anim[ind]->count = heigh;
        }
    }
}

void handle_shoot_inputs(
    Registry &r, sparse_array<Player> &anim, sparse_array<Position> &pos,
    sparse_array<Size> &siz)
{
    for (auto &&[ind, anima, posi, sizo] : indexed_zipper(anim, pos, siz)) {
        if (!(anima && posi && sizo))
            continue;
        if (anim[ind]->id == ind) {
            if (IsKeyDown(KEY_SPACE)) {
                anim[ind]->IsShooting = true;
                anim[ind]->current_charge +=
                    (anim[ind]->current_charge >= 3) ? 0 : 5 * GetFrameTime();
            }
            if (IsKeyReleased(KEY_SPACE)) {
                anim[ind]->IsShooting = false;
                create_ammo(
                    r,
                    Position(
                        pos[ind]->pos_X + (float) sizo->size_X,
                        pos[ind]->pos_Y + (float) sizo->size_Y / 2),
                    anim[ind]->current_charge);
                anim[ind]->current_charge = 1.;
            }

            // if (anim[ind]->IsShooting) {
            //     Animation de charge du tir
            // }
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
