#pragma once
#include <cstdint>
#include <boost/serialization/vector.hpp>
#include <mutex>
#include "Utils.hpp"

class NetEnt {
public:
    int id;
    Utils::Vec2 pos;
    uint8_t states;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar &id;
        ar &pos;
        ar &states;
    };
};

struct ThreadNetEnt {
    std::vector<NetEnt> ents;
    std::mutex mutex;
};
