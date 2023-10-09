#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include "../include/Systems.hpp"

void move(Registry &r, 
sparse_array<Position> positions, 
sparse_array<Speed> speed, 
sparse_array<Direction> dir)
{
    for (int ind = 0; ind < positions.size(); ind++) {
        std::cout << "pos : " << positions[ind]->pos_X << ", " << positions[ind]->pos_Y << "\nspeed : " << speed[ind]->speed << "\ndir : " << dir[ind]->dir_X << ", " << dir[ind]->dir_Y << std::endl;
        float magnitude = std::sqrt((dir[ind]->dir_X * dir[ind]->dir_X) + (dir[ind]->dir_Y * dir[ind]->dir_Y));
        std::cout << "magnitude " << magnitude << std::endl;
        if (magnitude > 0.1) { //Added a magnitude threshold to avoid going straight to INT_MIN and INT_MAX when having a really low direction move
            positions[ind]->pos_X += (speed[ind]->speed * (dir[ind]->dir_X / magnitude));
            positions[ind]->pos_Y += (speed[ind]->speed * (dir[ind]->dir_Y / magnitude));
        }
        std::cout << "pos : " << positions[ind]->pos_X << ", " << positions[ind]->pos_Y << "\nspeed : " << speed[ind]->speed << "\ndir : " << dir[ind]->dir_X << ", " << dir[ind]->dir_Y << std::endl;
    }
}

void display(Registry &r,
sparse_array<Position> positions, 
sparse_array<Size> size, 
sparse_array<Sprite> sprite)
{
    for (int ind = 0; ind < sprite.size(); ind++) {
        ClearBackground(BLACK);
        BeginDrawing();
            DrawTexture(sprite[ind]->sprite, positions[ind]->pos_X, positions[ind]->pos_Y, WHITE);
        EndDrawing();
    }
}