# Add component

# Description:
Register an component for a specified entity

# Signature:
```c++
Entity spawn_entity();
```

# Usage:
```c++
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
}
```
