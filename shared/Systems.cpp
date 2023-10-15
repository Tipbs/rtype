#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include "Systems.hpp"
#include "indexed_zipper.hpp"
#include "zipper.hpp"

#ifdef SERVER
    #include <chrono>

	static auto time_since_last_tick = std::chrono::high_resolution_clock::now(); // voir si raylib utilise m�me chose
	float GetFrameTime()
	{
		const auto now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration<double>(time_since_last_tick - now).count();
	}

    double GetTime()
    {
        return 1.0;
    }

    void ResetFrameTime()
	{
		time_since_last_tick = std::chrono::high_resolution_clock::now();
	}
#else
	#include <raylib.h>
#endif // !SERVER

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
    double time = GetTime();
    for (auto &&[pos, siz, gra]: zipper(positions, size, grace)) {
        if (gra.value_or(SpawnGrace(0, 0)).creation_time + gra.value_or(SpawnGrace(0, 0)).timer >= time)
                continue;
        for (auto &&[pos2, siz2, gra2]: zipper(positions, size, grace)) {
            if (gra2.value_or(SpawnGrace(0, 0)).creation_time + gra2.value_or(SpawnGrace(0, 0)).timer >= time)
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
