#pragma once

#include <map>
#include <vector>
#include <raylib.h>
#include "../../shared/Component.hpp"

enum MusicFx {
    Battle,
    Menu,
};

struct MusicComponent {
    std::map<MusicFx, Sound> musics;
    MusicFx context = Battle;

    MusicComponent(std::vector<std::pair<std::string, MusicFx>> paths)
    {
        for (auto &path : paths)
            musics[path.second] = LoadSound(path.first.c_str());
    }

    MusicComponent(std::string path, MusicFx type)
    {
        musics[type] = LoadSound(path.c_str());
    }
};

struct SoundManager {
    std::vector<Sound> sounds;
};

struct Sprite {
    Texture2D spritesheet;
    Rectangle sprite;
    double height_padding;
    double width_padding;
    double height_max;
    double width_max;
    int color_id = 0;
    std::chrono::duration<float> animation_delay =
        std::chrono::milliseconds(500);
    std::chrono::steady_clock::time_point time_since_last_anim =
        std::chrono::steady_clock::now();

    Sprite(const char *path)
        : spritesheet(LoadTextureFromImage(LoadImage(path)))
    {
        width_padding = spritesheet.width;
        height_padding = spritesheet.height;
        width_max = spritesheet.width;
        height_max = spritesheet.height;
        sprite.height = (spritesheet.height);
        sprite.width = (spritesheet.width);
        sprite.x = 0;
        sprite.y = 0;
    };

    Sprite(
        const char *path, int width, int height, double width_multiplier = 1,
        double height_multiplier = 1, int offset_x = 0, int offset_y = 0)
    {
        Image sprit = LoadImage(path);
        ImageResizeNN(
            &sprit, width * width_multiplier, height * height_multiplier);
        spritesheet = LoadTextureFromImage(sprit);
        width_padding = spritesheet.width / width_multiplier;
        height_padding = spritesheet.height / height_multiplier;
        width_max = width_multiplier;
        height_max = height_multiplier;
        sprite.height = (spritesheet.height) / height_multiplier;
        sprite.width = (spritesheet.width) / width_multiplier;
        sprite.x = offset_x * width;
        sprite.y = offset_y * width;
    };
};

struct InputField {
    std::string field;
};

struct Rect {
    bool draw_lines;
    Rectangle rect;

    Rect(bool draw_lines, Rectangle rect)
        : draw_lines(draw_lines), rect(rect) {};
};

struct Text {
    std::string text;
    int font_size = 0;

    Text(std::string s, int count) : text(s), font_size(count) {};
};

struct ScoreText {
    Entity from;

    ScoreText(Entity from) : from(from) {};
};

struct ChargeRect {
    Entity from;
    double maxWidth;

    ChargeRect(Entity from, double maxWidth)
        : from(from), maxWidth(maxWidth) {};
};
