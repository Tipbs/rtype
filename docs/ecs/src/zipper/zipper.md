# Zipper

# Description:
Take many sparseArray and create a container that will contain a tuple of iterator, ignore index if one of entity doesn't have all the component.

# Signature:
```c++
template <class ...Containers>
zipper(Containers &...cs);
```

# Usage:
```c++
struct Position {
	float x, y;
};

struct Velocity {
	float vx, vy;
};

void logging_system(Registry& r,
	sparse_array<Position> const& positions,
	sparse_array<Velocity> const& velocities) {
	for (auto &&[pos, vel] : zipper(positions, velocities)) {
		if (pos && vel) {
			std::cerr << "Position = { " << pos.value().x << ", " << pos.value
			().y << " } , Velocity = { " << vel.value().vx << ", " << vel.value() .
				vy << " }" << std::endl;
		}
	}
}

int main()
{
	Registry reg;

    reg.spawn_entity();
	reg.register_component<Position>();
	reg.register_component<Velocity>();
	reg.add_system<Position, Velocity>(logging_system);
    reg.run_systems();
}
