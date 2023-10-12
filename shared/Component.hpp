#pragma once

#include <string>
#include <utility>

#ifdef SERVER
	float GetFrameTime();
	double GetTime();
	void ResetFrameTime();
#else
	#include <raylib.h>
#endif // !SERVER

struct Player {
	int id = 0;
	Player(): id(0) {};
	Player(int x): id(x) {};
};

struct Position {
	double pos_X = 0;
	double pos_Y = 0;
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
	double timer = 0;
	double creation_time = 0;
	SpawnGrace(double x, double y): timer(x), creation_time(y) {};
	SpawnGrace(double timer): timer(timer) {
		creation_time = GetTime();
	};
};