﻿#include "registry.hpp"
//#include <raylib.h>

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

void logging_system(Registry& r,
	sparse_array<Position> const& positions,
	sparse_array<Velocity> const& velocities) {
	for (size_t i = 0; i < positions.size() && i < velocities.size(); ++i) {
		auto const& pos = positions[i];
		auto const& vel = velocities[i];
		if (pos && vel) {
			std::cerr << i << ": Position = { " << pos.value().x << ", " << pos.value
			().y << " } , Velocity = { " << vel.value().vx << ", " << vel.value() .
				vy << " }" << std::endl;
		}
	}
}

void test_system(Registry &r)
{
	auto const& positions = r.get_components<Position>();
	auto const& velocities = r.get_components<Velocity>();
	std::cout << positions[0].has_value() << std::endl;
	for (size_t i = 0; i < positions.size() && i < velocities.size(); ++i) {
		auto const& pos = positions[i];
		auto const& vel = velocities[i];
		if (pos && vel) {
			std::cerr << i << ": Position = { " << pos.value().x << ", " << pos.value
			().y << " } , Velocity = { " << vel.value().vx << ", " << vel.value() .
				vy << " }" << std::endl;
		}
	}
}

int main()
{
	Registry reg;
	auto ent = reg.spawn_entity();
	Position pos = { 10.0, 15.0 };
	Velocity vel = { 1.0, 1.0 }; 
	Drawable drawable = { 255 };
	Controllable controllable = { { 0 } };

	reg.register_component<Position>();
	reg.register_component<Velocity>();
	reg.register_component<Drawable>();
	reg.register_component<Controllable>();
	reg.add_component<Position>(ent, std::move(pos));
	reg.add_component<Velocity>(ent, std::move(vel));
	reg.add_component<Drawable>(ent, std::move(drawable));
	reg.add_component<Controllable>(ent, std::move(controllable));
	auto ent2 = reg.spawn_entity();
	test_system(reg);
	reg.add_system<Position, Velocity>(logging_system);
	return 0;
}
