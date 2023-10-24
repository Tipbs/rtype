#pragma once

#include <string>
#include <utility>
#include <chrono>
#include "Utils.hpp"

#ifdef SERVER
	float GetFrameTime();
	void ResetFrameTime();
#else
	#include <raylib.h>
#endif // !SERVER

std::chrono::steady_clock::time_point GetTimePoint();

struct Current_Player {
	size_t id;
	Current_Player() {id = -1;};
	Current_Player(size_t x) {id = x;};
};
struct Position {
	double pos_X = 0;
	double pos_Y = 0;
	Position(double x, double y): pos_X(x), pos_Y(y) {};
    Position(Utils::Vec2 vec): pos_X(vec.x), pos_Y(vec.y) {};
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
struct EnemyCount {
    int leftToSpawn = 0;
    int leftAlive = 0;
    float timeSinceLastSpawn = 0;
    float spawnFrequency = 2;
    EnemyCount(int enemiesNb, int spawnFrequency = 2): leftToSpawn(enemiesNb), spawnFrequency(spawnFrequency) {};
};
