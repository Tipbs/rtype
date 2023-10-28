#pragma once

#include <string>
#include <utility>
#ifndef SERVER
#include <raylib.h>
#endif
#include "Registry.hpp"
#include "Component.hpp"

Entity create_player(Registry &reg, size_t id, Position pos);
Entity create_ammo(Registry &reg, Position pos, float damage_mult, int color_id);
