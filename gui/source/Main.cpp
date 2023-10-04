// #include <iostream>

// int main()
// {
//     std::cout << "Hello world :,D!" << std::endl;
//     return 0;
// }
#include <iostream>
#include "../include/Registry.hpp"
#include "../include/Component.hpp"
#include "../include/Sparse_array.hpp"
#include "../include/Zipper.hpp"

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
        std::cout << "Position = { " << pos->pos_X << ", " << pos->pos_Y << " }"
                  << std::endl;
}

int main()
{
    Registry reg;
    Entity const entity = reg.spawn_entity();
    Position pos(1, 2);

    reg.register_component<Position>();
    reg.emplace_component<Position>(entity, 1, 2);
    reg.add_system<Position>(&print_position);
    reg.run_systems();
}
