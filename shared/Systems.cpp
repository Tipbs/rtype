#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <ostream>
#include "Component.hpp"
#include "Systems.hpp"
#include "Registry.hpp"
#include "indexed_zipper.hpp"

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

void damages(Registry &r,
sparse_array<Health> &healt,
sparse_array<Damages> &dama, 
size_t i1, size_t i2)
{
    healt[i1]->health -= dama[i2]->damages;
    std::cout << "User " << i1 << " has taken " << dama[i2]->damages << " damages. He now have " << healt[i1]->health << " HP." << std::endl;
    healt[i2]->health -= dama[i1]->damages;
    std::cout << "User " << i2 << " has taken " << dama[i1]->damages << " damages. He now have " << healt[i2]->health << " HP." << std::endl;
    if (healt[i1]->health <= 0)
        r.kill_entity(i1);
    if (healt[i2]->health <= 0)
        r.kill_entity(i2);
        

}

void colision(Registry &r,
sparse_array<Position> &positions, 
sparse_array<Size> &size, 
sparse_array<SpawnGrace> &grace, 
sparse_array<Damages> &dam, 
sparse_array<Health> &helth)
{
    double time = GetTime();
    for (auto &&[ind, pos, siz, dama, halth]: indexed_zipper(positions, size, dam, helth)) {
        if (!(pos && siz && dama && halth))
            continue;
        if (grace[ind].value_or(SpawnGrace(0, 0)).creation_time + grace[ind].value_or(SpawnGrace(0, 0)).timer >= time)
                continue;
        for (size_t ind2 = ind + 1; ind2 < positions.size(); ind2++) {
            if (grace[ind2].value_or(SpawnGrace(0, 0)).creation_time + grace[ind2].value_or(SpawnGrace(0, 0)).timer >= time)
                continue;
            if (positions[ind].value().pos_X > positions[ind2].value().pos_X + size[ind2].value().size_X)
                continue;
            else if (positions[ind].value().pos_Y > positions[ind2].value().pos_Y + size[ind2].value().size_Y)
                continue;
            else if (positions[ind2].value().pos_X > positions[ind].value().pos_X + size[ind].value().size_X)
                continue;
            else if (positions[ind2].value().pos_Y > positions[ind].value().pos_Y + size[ind].value().size_Y)
                continue;
            else {
                damages(r, helth, dam, ind, ind2);
            }
        }
    }
}
