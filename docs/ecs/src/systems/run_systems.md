# Run systems

# Description:
Run all systems of the registry one time

# Signature:
```c++
void run_systems();
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

	reg.add_system<Position, Velocity>(logging_system);
	reg.run_systems(); // will run the loggin_system system.
}
```
