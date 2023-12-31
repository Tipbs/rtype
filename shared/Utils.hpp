#pragma once

#include <boost/serialization/serialization.hpp>

namespace Utils {

template<typename T>
constexpr auto get(T value)
{
    return static_cast<std::underlying_type_t<T>>(value);
}

struct Vec2 {
    float x, y;

    Vec2 &operator=(const struct Vec2 &vec) = default;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &x;
        ar &y;
    };
};

struct PlayerId {
    std::size_t id;
    Vec2 pos;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &id;
        ar &pos;
    };
};
}; // namespace Utils
