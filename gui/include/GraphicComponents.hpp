#pragma once

#include <iostream>
#include <utility>
#include <raylib.h>
#include "../../shared/Component.hpp"

enum MusicFx {
    Battle,
    Menu
};

struct MusicComponent {
    Sound music;
    MusicFx type;

    MusicComponent(std::string path, MusicFx _type)
    {
        music = LoadSound(path.c_str());
        type = _type;
    }
};

struct SoundComponent {
    std::vector<std::tuple<Sound, bool &, bool>>
        sounds; // tuple(SOUND, STATE_PTR, LAST_STATE)

    /*SoundComponent(std::vector<std::pair<std::string, bool &>> paths) {
        for (auto &path: paths) {
            bool &test = std::ref(path.second);
            Sound sfx = LoadSound(path.first.c_str());
            std::cout << "path second = " << test << std::endl;
            sounds.push_back(std::make_tuple(sfx, path.second, false));
        }
        }*/
    SoundComponent(std::string path, bool &bo)
    {
        Sound sfx = LoadSound(path.c_str());
        sounds.emplace_back(sfx, bo, false);
    }
};

struct Sprite {
    Texture2D spritesheet;
    Rectangle sprite;
    double height_padding;
    double width_padding;
    double height_max;
    double width_max;
    int color_id = 0;

    Sprite(
        char *path, int width, int height, double width_multiplier,
        double height_multiplier)
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
        sprite.x = 0;
        sprite.y = 0;
    };

    Sprite(
        const char *path, int width, int height, double width_multiplier,
        double height_multiplier)
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
        sprite.x = 0;
        sprite.y = 0;
    };

    Sprite(char *path, int width, int height)
    {
        Image sprit = LoadImage(path);
        ImageResizeNN(&sprit, width, height);
        spritesheet = LoadTextureFromImage(sprit);
        width_padding = spritesheet.width;
        height_padding = spritesheet.height;
        width_max = width;
        height_max = height;
        sprite.height = (spritesheet.height);
        sprite.width = (spritesheet.width);
        sprite.x = 0;
        sprite.y = 0;
    };

    Sprite(const char *path, int width, int height)
    {
        Image sprit = LoadImage(path);
        ImageResizeNN(&sprit, width, height);
        spritesheet = LoadTextureFromImage(sprit);
        width_padding = spritesheet.width;
        height_padding = spritesheet.height;
        width_max = width;
        height_max = height;
        sprite.height = (spritesheet.height);
        sprite.width = (spritesheet.width);
        sprite.x = 0;
        sprite.y = 0;
    };

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

    Sprite(char *path) : spritesheet(LoadTextureFromImage(LoadImage(path)))
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
};

struct Animation {
    // Rectangle rect; //contains size as well
    Sprite sprite;
    Position *frames; // contains a list of position for each frames of an
                      // animated picture.
};

struct InputField {
    std::string field;
};
