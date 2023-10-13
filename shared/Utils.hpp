#pragma once

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
}
