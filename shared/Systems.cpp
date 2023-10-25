#include "Component.hpp"
#include "Systems.hpp"
#include "indexed_zipper.hpp"
#include "zipper.hpp"
#include <syncstream>

void move(Registry &r, 
sparse_array<Position> &positions,
sparse_array<Speed> &speed, 
sparse_array<Direction> &dir)
{
    for (auto &&[pos, spe, diro]: zipper(positions, speed, dir)) {
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

void update_grace(Registry &r,
sparse_array<SpawnGrace> &graces)
{
    auto time = GetTimePoint();

    for (auto &&[ind, grace]: indexed_zipper(graces)) {
        if (grace->creation_time + grace->time >= time) {
            r.remove_component<SpawnGrace>(ind);
        }
    }
}

void colision(Registry &r,
sparse_array<Position> &positions, 
sparse_array<Size> &size, 
sparse_array<SpawnGrace> &grace, 
sparse_array<Damages> &dam, 
sparse_array<Health> &helth)
{
    for (auto &&[ind, pos, siz, dama, halth]: indexed_zipper(positions, size, dam, helth)) {
        if (grace[ind].has_value()) {
            continue;
        }
        for (auto &&[ind2, pos2, siz2, dama2, halth2]: indexed_zipper(positions, size, dam, helth)) {
            if (ind2 <= ind || grace[ind2].has_value()) {
                continue;
            }
            if (pos->pos_X > pos2->pos_X + siz2->size_X)
                continue;
            else if (pos->pos_Y > pos2->pos_Y + siz2->size_Y)
                continue;
            else if (pos2->pos_X > pos->pos_X + siz->size_X)
                continue;
            else if (pos2->pos_Y > pos->pos_Y + siz->size_Y)
                continue;
            else {
                damages(r, helth, dam, ind, ind2);
            }
        }
    }
}
