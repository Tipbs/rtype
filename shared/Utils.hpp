#pragma once

namespace Utils {
    struct Vec2 {
        double x, y;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar &x;
            ar &y;
        };
    };
}
