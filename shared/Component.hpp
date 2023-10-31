#pragma once

#include <string>
#include <utility>
#include <chrono>

#ifdef SERVER
	float GetFrameTime();
	void ResetFrameTime();
#else
	#include <raylib.h>
#endif // !SERVER

std::chrono::steady_clock::time_point GetTimePoint();

struct Current_Player {};

struct NetworkedEntity {
    int id;

    NetworkedEntity(int id) : id(id) {}
};

struct Position {
	double pos_X = 0;
	double pos_Y = 0;
    Position operator+(Position &pos)
	{
		return Position(pos_X + pos.pos_X, pos_Y + pos.pos_Y);
	};
	Position(double x, double y): pos_X(x), pos_Y(y) {};
};

struct Damages {
	int damages = 0;
	Damages(int x): damages(x) {};
};

struct Size {
	int size_X = 1;
	int size_Y = 1;
	Size(int x, int y): size_X(x), size_Y(y) {};
};

struct Health {
	int health = 1;
	Health(int x): health(x) {};
};

struct Speed {
	double speed = 0;
	Speed(double x): speed(x) {};
};

struct Direction {
	double dir_X = 0;
	double dir_Y = 0;
	Direction(double x, double y): dir_X(x), dir_Y(y) {};
};

struct SpawnGrace {
	std::chrono::duration<float> time;
	std::chrono::steady_clock::time_point creation_time;
	SpawnGrace(std::chrono::duration<float> x, std::chrono::steady_clock::time_point y): time(x), creation_time(y) {};
	SpawnGrace(std::chrono::duration<float> timer): time(timer) {
		creation_time = GetTimePoint();
	};
};

struct AlwaysShoot {
	std::chrono::duration<float> delay;
	std::chrono::steady_clock::time_point last_shoot;

	AlwaysShoot(std::chrono::duration<float> duration)
		: delay(duration), last_shoot(std::chrono::steady_clock::now()) { }
};

struct ProjectileInfo {
	Position offset;
	Direction dir;
};

struct ProjectileShooter {
	std::vector<ProjectileInfo> infos;
	std::chrono::duration<float> delay;
	std::chrono::steady_clock::time_point lastShot;
	int shotCount = 0;

	ProjectileShooter(std::chrono::duration<float> del) : delay(del), lastShot(std::chrono::steady_clock::now())
	{}
};
