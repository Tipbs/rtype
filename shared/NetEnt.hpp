#pragma once
#include <cstdint>
#include <boost/serialization/vector.hpp>
#include <mutex>
#include "Utils.hpp"

enum class EntityType {
    Player
};

class NetEnt {
public:
    int id;
	EntityType type;
    Utils::Vec2 pos;
    uint8_t states;
    double speed;

    NetEnt &operator=(const NetEnt &) = default;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar &id;
        ar &type;
        ar &pos;
        ar &states;
    };
};

struct ThreadNetEnt {
    std::vector<NetEnt> ents;
    std::mutex mutex;
};
