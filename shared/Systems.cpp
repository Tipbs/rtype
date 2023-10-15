#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include "Systems.hpp"
#include <chrono>
#include "indexed_zipper.hpp"
#include "zipper.hpp"

#ifdef SERVER
	std::mutex mutex;
	static auto time_since_last_tick = std::chrono::high_resolution_clock::now(); // voir si raylib utilise m�me chose
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
    for (auto &&[ind, pos, spe, diro]: indexed_zipper(positions, speed, dir)) {
        if (!(pos && spe && diro))
            continue;
        double magnitude = std::sqrt(
            (dir[ind].value().dir_X * 
            dir[ind].value().dir_X) + 
            (dir[ind].value().dir_Y * 
            dir[ind].value().dir_Y));
        if (magnitude > 0.1) { //Added a magnitude threshold to avoid going straight to INT_MIN and INT_MAX when having a really low direction move
            positions[ind].value().pos_X += 
                (speed[ind].value().speed * 
                (dir[ind].value().dir_X / magnitude)) * 
                GetFrameTime();
            positions[ind].value().pos_Y += 
                (speed[ind].value().speed * 
                (dir[ind].value().dir_Y / magnitude)) * 
                GetFrameTime();
        }
    }
}

void colision(Registry &r,
sparse_array<Position> &positions, 
sparse_array<Size> &size, 
sparse_array<SpawnGrace> &grace)
{
    std::chrono::steady_clock::time_point time = GetTimePoint();
    for (auto &&[ind, pos, siz]: zipper(positions, size)) {
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
            if (pos.value().pos_X > pos2.value().pos_X + siz2.value().size_X)
                continue;
            else if (pos.value().pos_Y > pos2.value().pos_Y + siz2.value().size_Y)
                continue;
            else if (pos2.value().pos_X > pos.value().pos_X + siz.value().size_X)
                continue;
            else if (pos2.value().pos_Y > pos.value().pos_Y + siz.value().size_Y)
                continue;
            else {  }
                //TODO
                // std::cout << "y'a collision\n";
        }
    }
}
