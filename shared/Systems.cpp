#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include "Systems.hpp"
#include <chrono>

#ifdef SERVER
	std::mutex mutex;
	static auto time_since_last_tick = std::chrono::high_resolution_clock::now(); // voir si raylib utilise même chose
	float GetFrameTime()
	{
		std::scoped_lock(mutex);
		const auto now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration<double>(time_since_last_tick - now).count();
	}

    void ResetFrameTime()
	{
		std::scoped_lock(mutex);
		time_since_last_tick = std::chrono::high_resolution_clock::now();
	}
#else
	#include <raylib.h>
#endif // !SERVER

std::chrono::steady_clock::time_point GetTimePoint()
{
	return std::chrono::steady_clock::now();
}

void move(Registry &r, 
sparse_array<Position> &positions,
sparse_array<Speed> &speed, 
sparse_array<Direction> &dir)
{
    for (size_t ind = 0; ind < positions.size(); ind++) {
        auto &pos = positions[ind];
        auto &spe = speed[ind];
        auto &diro = dir[ind];
        if (!(pos && spe && diro))
            continue;
        //std::cout << "BEFORE : pos : " << positions[ind].value().pos_X << ", " << positions[ind].value().pos_Y << "\nspeed : " << speed[ind].value().speed << "\ndir : " << dir[ind].value().dir_X << ", " << dir[ind].value().dir_Y << std::endl << std::endl;
        double magnitude = std::sqrt((dir[ind].value().dir_X * dir[ind].value().dir_X) + (dir[ind].value().dir_Y * dir[ind].value().dir_Y));
        //std::cout << "magnitude " << magnitude << std::endl;
        if (magnitude > 0.1) { //Added a magnitude threshold to avoid going straight to INT_MIN and INT_MAX when having a really low direction move
            positions[ind].value().pos_X += (speed[ind].value().speed * (dir[ind].value().dir_X / magnitude)) * GetFrameTime();
            positions[ind].value().pos_Y += (speed[ind].value().speed * (dir[ind].value().dir_Y / magnitude)) * GetFrameTime();
        }
        //std::cout << "AFTER : pos : " << positions[ind].value().pos_X << ", " << positions[ind].value().pos_Y << "\nspeed : " << speed[ind].value().speed << "\ndir : " << dir[ind].value().dir_X << ", " << dir[ind].value().dir_Y << std::endl << std::endl;
    }
}

void colision(Registry &r,
sparse_array<Position> &positions, 
sparse_array<Size> &size, 
sparse_array<SpawnGrace> &grace)
{
    std::chrono::steady_clock::time_point time = GetTimePoint();
    for (size_t ind = 0; ind < positions.size(); ind++) {
        auto &pos = positions[ind];
        auto &siz = size[ind];
        if (!(pos && siz))
            continue;
        //std::cout << "temps d'origine : "
        //    << grace[ind].value_or(SpawnGrace(std::chrono::seconds(0))).creation_time.time_since_epoch()
        //          << std::endl;
        //std::cout << "temps de grace : "
        //    << grace[ind].value_or(SpawnGrace(std::chrono::seconds(0))).timer
        //          << std::endl;
        //std::cout << "temps actuel : " << time.time_since_epoch() << std::endl;
        if (grace[ind].value_or(SpawnGrace(std::chrono::seconds(0))).creation_time + grace[ind].value_or(SpawnGrace(std::chrono::seconds(0))).timer >=
            time)
            continue;
        for (size_t ind2 = ind + 1; ind2 < positions.size(); ind2++) {
            if (grace[ind2].value().creation_time +
                    grace[ind2].value().timer >=
                time)
                continue;
            if (positions[ind].value().pos_X > positions[ind2].value().pos_X + size[ind2].value().size_X)
                continue;
            else if (positions[ind].value().pos_Y > positions[ind2].value().pos_Y + size[ind2].value().size_Y)
                continue;
            else if (positions[ind2].value().pos_X > positions[ind].value().pos_X + size[ind].value().size_X)
                continue;
            else if (positions[ind2].value().pos_Y > positions[ind].value().pos_Y + size[ind].value().size_Y)
                continue;
            else
                std::cout << "y'a collision\n";
        }
    }
}
