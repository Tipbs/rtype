#pragma once

#include <boost/serialization/serialization.hpp>

namespace Utils {
    struct Vec2 {
        double x, y;

        Vec2 &operator=(const struct Vec2 &vec) = default;
        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar &x;
            ar &y;
        };
    };

    struct PlayerId {
        std::size_t id;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar &id;
        };
    };
};
