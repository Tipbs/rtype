#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "../../shared/Registry.hpp"
#include "../../shared/zipper.hpp"
#include "../../shared/Sparse_array.hpp"
#include "../../shared/Systems.hpp"


void print_position(Registry &r, sparse_array<Position> positions)
{
    for (auto pos : positions)
        std::cout << "Position = { " << pos->pos_X << ", " << pos->pos_Y << " }" << std::endl;
}

TEST_CASE("Register", "ECS")
{
    Registry reg;
    sparse_array<Position> positions(0);
    Entity const entity = reg.spawn_entity();

    reg.register_component<Position>();
    reg.emplace_component<Position>(entity, 1, 2);
}

TEST_CASE("System", "ECS")
{
    sparse_array<Position> positions(0);
    Registry reg;
    Entity const entity = reg.spawn_entity();

    reg.register_component<Position>();
    reg.emplace_component<Position>(entity, 1, 2);
    reg.add_system<Position>(&print_position);
    reg.run_systems();
}

void print_position_zipper(Registry &r, sparse_array<Position> positions, sparse_array<Damages> damages)
{
    sparse_array<Position>::iterator pos = positions.begin();
    sparse_array<Damages>::iterator dam = damages.begin();
    std::tuple tup = std::make_tuple(pos, dam);

    zipper_iterator<sparse_array<Position>, sparse_array<Damages>> it(tup, 2);
    auto test = *it;
    auto &&[pos2, dam2] = test;
    pos2->pos_X = 5;
    pos2->pos_Y = 6;
    REQUIRE(positions[0]->pos_X == 5);
    REQUIRE(positions[0]->pos_Y == 6);
}

TEST_CASE("zipper_iterator", "ECS")
{
    sparse_array<Position> positions;
    sparse_array<Damages> damage;
    Registry reg;
    Entity const entity = reg.spawn_entity();
    Entity const entity2 = reg.spawn_entity();

    reg.register_component<Position>();
    reg.register_component<Damages>();
    reg.emplace_component<Position>(entity, 1, 2);
    reg.emplace_component<Position>(entity2, 3, 4);
    reg.emplace_component<Damages>(entity, 1);
    reg.emplace_component<Damages>(entity2, 2);
    reg.add_system<Position, Damages>(&print_position_zipper);
    reg.run_systems();
}
