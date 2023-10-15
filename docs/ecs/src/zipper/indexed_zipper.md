# Zipper

# Description:
Take many sparseArray and create a container that will contain a tuple of iterator, ignore index if one of entity doesn't have all the component.
The tuple will also have the index of iterator in it.

# Signature:
```c++
template <class ...Containers>
zipper_iterator(Containers &...cs);
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
	for (auto &&[i, pos, vel] : indexed_zipper(positions, velocities)) {
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

    reg.spawn_entity();
	reg.register_component<Position>();
	reg.register_component<Velocity>();
	reg.add_system<Position, Velocity>(logging_system);
    reg.run_systems();
}
