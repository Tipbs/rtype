#include <raylib.h>
#include "GraphicComponents.hpp"
#include "GraphicSystems.hpp"
#include "../../shared/Registry.hpp"
#include "../../shared/Sparse_array.hpp"

void display(Registry &r,
sparse_array<Position> &positions, 
sparse_array<Size> &size, 
sparse_array<Sprite> &sprite,
sparse_array<Player> &anim)
{
    BeginDrawing();
    for (size_t ind = 0; ind < sprite.size(); ind++) {
        auto &pos = positions[ind];
        auto &siz = size[ind];
        auto &spri = sprite[ind];
        if (!(pos && siz && spri))
            continue;
        if (sprite[ind]->width_max == 9 && sprite[ind]->height_max == 1) {
            sprite[ind]->sprite.x = (sprite[ind]->sprite.x / sprite[ind]->width_padding == sprite[ind]->width_max ? 0 : (sprite[ind]->sprite.x + sprite[ind]->width_padding));
        }
        if (anim[ind]) {
            sprite[ind]->sprite.y = anim[ind]->id * sprite[ind]->height_padding;
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
sparse_array<Player> &anim, 
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
    else if (IsKeyDown(KEY_UP)) {
        Moves.y -= 1;
        heigh = (heigh >= 2) ? 2 : heigh + (5 * AnimationPad);
    }
    else if (IsKeyDown(KEY_DOWN)) {
        Moves.y += 1;
        heigh = (heigh <= 0) ? 0 : heigh - (5 * AnimationPad);
    }

    if (IsKeyPressed(KEY_C)) {
        anim[1]->id = anim[1]->id == 4 ? 0 : anim[1]->id + 1;
    }


    if (dir[1]) { //1 is the entity num representing the player seen here
        dir[1].value().dir_X = Moves.x;
        dir[1].value().dir_Y = Moves.y;
    }

    if (anim[1]) {
        anim[1]->count = heigh;
    }
}

void handle_shoot_inputs(Registry &r, 
sparse_array<Player> &anim,
sparse_array<Position> &pos,
sparse_array<Size> &siz)
{
    if (IsKeyDown(KEY_SPACE)) {
        anim[1]->IsShooting = true;
    }
    if (IsKeyReleased(KEY_SPACE)) {
        anim[1]->IsShooting = false;
    }

    if (anim[1]->IsShooting) {
        std::cout << "Player id " << anim[1]->id << " just shoot with a " << anim[1]->weapon.type << " typed weapon, with an attack speed of " << anim[1]->weapon.attack_speed << std::endl;

        create_ammo(r, Position(pos[1]->pos_X + siz[1]->size_X, pos[1]->pos_Y + (siz[1]->size_Y / 2.) - 15), anim[1]->weapon);
    }
        
}

void make_infinite_background(Registry &r,
sparse_array<Position> &pos,
sparse_array<Size> &siz)
{
    if (pos[0] && siz[0]) {
        if (pos[0]->pos_X < -siz[0]->size_X)
            pos[0]->pos_X += siz[0]->size_X;
        if (pos[0]->pos_Y < -siz[0]->size_Y)
            pos[0]->pos_Y += siz[0]->size_Y;

        if (pos[0]->pos_X > 0)
            pos[0]->pos_X -= siz[0]->size_X;
        if (pos[0]->pos_Y > 0)
            pos[0]->pos_Y -= siz[0]->size_Y;
    }
} 