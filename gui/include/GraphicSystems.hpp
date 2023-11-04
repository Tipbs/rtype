#pragma once
#include "../../shared/Sparse_array.hpp"
#include "../../shared/Systems.hpp"
#include "GraphicComponents.hpp"

void display(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size,
    sparse_array<Sprite> &sprite, sparse_array<Player> &anim,
    sparse_array<Rectangle> &rectangles, sparse_array<InputField> &inputFields,
    sparse_array<Rect> &rect, sparse_array<Color> &col,
    sparse_array<Text> &text, sparse_array<MenuFields> &menuFields, sparse_array<CustomText> &texts,
    sparse_array<CanBeSelected> &selectables);
void handle_dir_inputs(
    Registry &r, sparse_array<Direction> &dir,
    sparse_array<Current_Player> &players, sparse_array<Sprite> &sprite,
    sparse_array<Speed> &speeds, sparse_array<Couleur> &colors);
void hadle_text_inputs(
    Registry &r, sparse_array<InputField> &inputFields,
    sparse_array<Rectangle> &rectangles);
void handle_shoot_inputs(
    Registry &r, sparse_array<Couleur> &players, sparse_array<Size> &sizes,
    sparse_array<Weapon> &weapons, sparse_array<Position> &positions);
void updateWithSnapshots(
    Registry &r, sparse_array<Position> &positions,
    sparse_array<NetworkedEntity> &entities, sparse_array<Speed> &speeds,
    sparse_array<Current_Player> &currents, sparse_array<Size> &sizes,
    sparse_array<Player> &players);
void do_animation(
    Registry &r, sparse_array<Sprite> &sprites, sparse_array<Couleur> &colors);
void do_ship_animation(
    Registry &r, sparse_array<Sprite> &sprites, sparse_array<Couleur> &couleurs,
    sparse_array<Weapon> &weapons, sparse_array<Current_Player> &cur_player);
void make_infinite_background(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &sizes,
    sparse_array<Backgrounds> &bg);
void handle_music(Registry &r, sparse_array<MusicComponent> &sounds);
void play_sound(Registry &r, sparse_array<SoundManager> &sounds);
void update_score_text(
    Registry &r, sparse_array<Score> &scores,
    sparse_array<ScoreText> &scoreTexts, sparse_array<Text> &texts);
void update_charge_rect(
    Registry &r, sparse_array<Weapon> &weapons,
    sparse_array<ChargeRect> &chargeRects, sparse_array<Rect> &rects);
void handle_menu_inputs(
    Registry &r, sparse_array<MenuFields> &menuFields,
    sparse_array<Rectangle> &rectangles, sparse_array<CustomText> &texts);
void selectable_text(
    Registry &r, sparse_array<CustomText> &texts, sparse_array<Position> &positions,
    sparse_array<CanBeSelected> &selectables);
