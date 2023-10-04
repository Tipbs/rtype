#include <iostream>
#include "../include/Systems.hpp"

void move(Registry &r, 
sparse_array<Position> positions, 
sparse_array<Speed> speed, 
sparse_array<Direction> dir)
{
    for (auto ind : positions.size()) {
        std::cout << "pos : " << positions[ind] << "\nspeed : " << speed[ind] << "\ndir : " << dir[ind] << std::endl;
    }
}