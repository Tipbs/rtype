#include "GraphicComponents.hpp"
#include "../../shared/Registry.hpp"

void display(Registry &r,
sparse_array<Position> &positions, 
sparse_array<Size> &size, 
sparse_array<Sprite> &sprite)
{
    BeginDrawing();
    for (size_t ind = 0; ind < sprite.size(); ind++) {
        auto &pos = positions[ind];
        auto &siz = size[ind];
        auto &spri = sprite[ind];
        if (!(pos && siz && spri))
            continue;
        DrawTexture(sprite[ind].value().sprite, positions[ind].value().pos_X, positions[ind].value().pos_Y, WHITE);
    }
    EndDrawing();
}
