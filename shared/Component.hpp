#pragma once

#include <string>
#include <utility>
#include <chrono>
#include "Entity.hpp"
#include "Utils.hpp"

#ifdef SERVER
	float GetFrameTime();
	void ResetFrameTime();
#else
	#include <raylib.h>
#endif // !SERVER

std::chrono::steady_clock::time_point GetTimePoint();

struct Current_Player {};

struct Player {
    int color_id = -1;
};

struct NetworkedEntity {
    int id;

    NetworkedEntity(int id) : id(id) {}
};

struct Position {
	double pos_X = 0;
	double pos_Y = 0;
	Position(double x = 0, double y = 0): pos_X(x), pos_Y(y) {};
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

struct Weapon {
    int type = 1;
    Size size = Size(1,1);
    double attack_speed = 1.;
    Entity owner_id;
    bool IsShooting = false;
    float current_charge = 0.;

    Weapon(Entity owner_id, int type = 1, int ammo_per_sec = 1): type(type), owner_id(owner_id) {
        size = Size(1,1);
        attack_speed = 1. / ammo_per_sec;
    };
};

struct Couleur {
    int color_id = 0;

    Couleur(int count) : color_id(count) {};
};

struct Animation {
    Animation() {};
};

struct Backgrounds {
    Backgrounds()
    {};
};

struct EnemyCount {
    int leftToSpawn = 0;
    int leftAlive = 0;
    float timeSinceLastSpawn = 0;
    float spawnFrequency = 2;
    EnemyCount(int enemiesNb, int spawnFrequency = 2): leftToSpawn(enemiesNb), spawnFrequency(spawnFrequency) {};
};

// struct HUD {
// 	double charge = 0;
// 	int score = 0;
//     int color_id = 0;
// 	HUD() {charge = 0; score = 0; color_id = 0;};
// };
