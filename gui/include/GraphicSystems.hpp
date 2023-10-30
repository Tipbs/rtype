#pragma once
#include "../../shared/Sparse_array.hpp"
#include "../../shared/Systems.hpp"
#include "GraphicComponents.hpp"

void display(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size,
    sparse_array<Sprite> &sprite, sparse_array<Player> &anim,
    sparse_array<Rectangle> &rectangles, sparse_array<InputField> &inputFields);
void do_animation(
    Registry &r, sparse_array<Sprite> &sprites,
    sparse_array<Animation> &animations, sparse_array<Weapon> &weapons);
void handle_dir_inputs(
    Registry &r, sparse_array<Direction> &dir, sparse_array<Player> &players,
    sparse_array<Sprite> &sprite, sparse_array<Speed> &speeds,
    sparse_array<Animation> &animations);
void hadle_text_inputs(
    Registry &r, sparse_array<InputField> &inputFields,
    sparse_array<Rectangle> &rectangles);
void handle_shoot_inputs(
    Registry &r, sparse_array<Animation> &players, sparse_array<Size> &sizes,
    sparse_array<Weapon> &weapons, sparse_array<Position> &positions);
void updateWithSnapshots(
    Registry &r, sparse_array<Position> &positions,
    sparse_array<NetworkedEntity> &entities, sparse_array<Speed> &speeds,
    sparse_array<Current_Player> &currents, sparse_array<Size> &sizes,
    sparse_array<Player> &players);
void do_animation(
    Registry &r, sparse_array<Sprite> &sprites,
    sparse_array<Animation> &animations, sparse_array<Weapon> &weapons);
void make_infinite_background(
    Registry &r, sparse_array<Position> &positions,
    sparse_array<Size> &sizes, sparse_array<Backgrounds> &bg);
