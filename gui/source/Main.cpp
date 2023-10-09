#include <iostream>
#include <semaphore>
#include <format>
#include "Registry.hpp"
#include "Sparse_array.hpp"
#include "Zipper.hpp"

struct Position {
    float x, y;
}

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
    for (auto &pos : positions)
        std::cout << "Position = { " << pos->x << ", " << pos->y << " }"
                  << std::endl;
}

int main()
{
    Registry reg;
    Entity const entity = reg.spawn_entity();
    Position pos(1, 2);
    float x = 2.0, y = 1.0;
    std::cout << x << y << std::endl;
    std::cout << std::format("Hello {} !\n", "world") << std::endl;
    reg.register_component<Position>();
    std::optional<Position> &temp = reg.emplace_component<Position>(entity, x, y);
    reg.add_system<Position>(&print_position);
    reg.run_systems();
}
