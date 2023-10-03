/*
** EPITECH PROJECT, 2023
** Raytracer
** File description:
** main.cpp
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <iostream>
#include "registry.hpp"
#include "zipper.hpp"
#include "sparse_array.hpp"

struct Position {
	float x, y;
};

struct Velocity {
	float vx, vy;
};

struct Drawable {
	int color;
};

struct Controllable {
	bool key_pressed[256];
};

// void redirect_all_stdout(void)
// {
//     cr_redirect_stdout();
//     cr_redirect_stderr();
// }

void print_position(Registry &r, sparse_array<Position> positions)
{
    for (auto pos : positions)
        std::cout << "Position = { " << pos->x << ", " << pos->y << " }" << std::endl;
}

void test_zipper()
{
    sparse_array<Position> positions(0);
    Zipper zipper(positions);
    Registry reg;

    reg.register_component<Position>(); 
}
