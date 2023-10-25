#include "Component.hpp"
#include "Systems.hpp"
#include "indexed_zipper.hpp"
#include "zipper.hpp"
#include <syncstream>


/**
 * @brief Add a new component with its erase and insert function.
 *
 * @tparam  Component   The Component to insert.
 * @return  The sparse_array of Component newly created.
 */
void move(Registry &r, 
sparse_array<Position> &positions,
sparse_array<Speed> &speed, 
sparse_array<Direction> &dir)
{
    for (auto &&[pos, spe, diro]: zipper(positions, speed, dir)) {
        //std::osyncstream(std::cout) << "y = " << pos->pos_Y << "  x = " << pos->pos_X << std::endl;
		pos->pos_X += spe->speed * diro->dir_X * GetFrameTime();
		pos->pos_Y += spe->speed * diro->dir_Y * GetFrameTime();
        #ifdef SERVER
        diro->dir_X = 0;
        diro->dir_Y = 0;
        #endif
    }
}

void damages(Registry &r,
sparse_array<Health> &healt,
sparse_array<Damages> &dama, 
size_t i1, size_t i2)
{
    healt[i1]->health -= dama[i2]->damages;
    std::osyncstream(std::cout) << "User " << i1 << " has taken " << dama[i2]->damages << " damages. He now have " << healt[i1]->health << " HP." << std::endl;
    healt[i2]->health -= dama[i1]->damages;
    std::osyncstream(std::cout) << "User " << i2 << " has taken " << dama[i1]->damages << " damages. He now have " << healt[i2]->health << " HP." << std::endl;
    if (healt[i1]->health <= 0)
        r.kill_entity(r.entity_from_index(i1));
    if (healt[i2]->health <= 0)
        r.kill_entity(r.entity_from_index(i2));
}

void colision(Registry &r,
sparse_array<Position> &positions, 
sparse_array<Size> &size, 
sparse_array<SpawnGrace> &grace, 
sparse_array<Damages> &dam, 
sparse_array<Health> &helth)
{
    auto time = GetTimePoint();
    for (auto &&[ind, pos, siz, dama, halth]: indexed_zipper(positions, size, dam, helth)) {
        if (!(pos && siz && dama && halth))
            continue;
        if (grace[ind].value_or(SpawnGrace(std::chrono::seconds(0))).creation_time + grace[ind].value_or(SpawnGrace(std::chrono::seconds(0))).time >= time)
                continue;
        for (size_t ind2 = ind + 1; ind2 < positions.size(); ind2++) {
            if (grace[ind2].value_or(SpawnGrace(std::chrono::seconds(0))).creation_time + grace[ind2].value_or(SpawnGrace(std::chrono::seconds(0))).time >= time)
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
