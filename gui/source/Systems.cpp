#include <iostream>
#include "../include/Systems.hpp"

void move(Registry &r, 
sparse_array<Position> positions, 
sparse_array<Speed> speed, 
sparse_array<Direction> dir)
{
    for (int ind = 0; ind < positions.size(); ind++) {
        std::cout << "pos : " << positions[ind]->pos_X << ", " << positions[ind]->pos_Y << "\nspeed : " << speed[ind]->speed << "\ndir : " << dir[ind]->dir_X << ", " << dir[ind]->dir_Y << std::endl;
    }
}
