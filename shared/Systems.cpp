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
        std::cout << "y = " << pos->pos_Y << " and x = " << pos->pos_X << std::endl;
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

void colision(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size,
    sparse_array<SpawnGrace> &grace)
{
    auto time = GetTimePoint();
    for (auto &&[ind, pos, siz] :
         indexed_zipper(positions, size)) {
        if (!(pos && siz))
            continue;
        if (grace[ind]
                    .value_or(SpawnGrace(std::chrono::seconds(0)))
                    .creation_time +
                grace[ind]
                    .value_or(SpawnGrace(std::chrono::seconds(0)))
                    .time >=
            time)
            continue;
        for (size_t ind2 = ind + 1; ind2 < positions.size(); ind2++) {
            if (grace[ind2]
                        .value_or(SpawnGrace(std::chrono::seconds(0)))
                        .creation_time +
                    grace[ind2]
                        .value_or(SpawnGrace(std::chrono::seconds(0)))
                        .time >=
                time)
                continue;
            if (positions[ind].value().pos_X >
                positions[ind2].value().pos_X + size[ind2].value().size_X)
                continue;
            else if (
                positions[ind].value().pos_Y >
                positions[ind2].value().pos_Y + size[ind2].value().size_Y)
                continue;
            else if (
                positions[ind2].value().pos_X >
                positions[ind].value().pos_X + size[ind].value().size_X)
                continue;
            else if (
                positions[ind2].value().pos_Y >
                positions[ind].value().pos_Y + size[ind].value().size_Y)
                continue;
            else {
            }
        }
    }
}
