# Add system

# Description:
Add a system to the registry, it will be executed on each iteration of run_systems()

# Signature:
```c++
template <class ...Components, typename Function>
void add_system(Function&& f);
template <class ...Components, typename Function>
void add_system(Function const& f);
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

	reg.register_component<Position>();
	reg.register_component<Velocity>();
	reg.add_system<Position, Velocity>(logging_system);
}
