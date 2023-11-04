#pragma once
#include <cstdint>
#include <mutex>
#include <boost/serialization/vector.hpp>
#include "Utils.hpp"

enum class EntityType {
    Player,
    Zorg,
    Boss,
    Projectile,
    Ammo,
    Enemy,
    None
};

class NetEnt {
  public:
    int id;
    EntityType type;
    Utils::Vec2 pos {};
    Utils::Vec2 dir {};
    uint8_t attacking = false;
    float attackState;

    NetEnt()
    {
        id = 0;
        type = EntityType::None;
        attackState = 0;
    }

    NetEnt &operator=(const NetEnt &) = default;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &id;
        ar &type;
        ar &pos;
        ar &attacking;
        ar &attackState;
    };
};

struct ThreadNetEnt {
    std::vector<NetEnt> ents;
    std::mutex mutex;
};
