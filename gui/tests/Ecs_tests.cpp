#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <iostream>
#include "Registry.hpp"
#include "Zipper.hpp"
#include "Sparse_array.hpp"

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
        std::cout << "Position = { " << pos->pos_X << ", " << pos->pos_Y << " }" << std::endl;
}

Test(Ecs, create_comomponent)
{
    sparse_array<Position> positions(0);
    //Zipper zipper(positions);
    Registry reg;
    Entity const entity = reg.spawn_entity();

    reg.register_component<Position>();
    reg.emplace_component<Position>(entity);
    reg.add_system<Position>(&print_position);
    reg.run_systems();
}
