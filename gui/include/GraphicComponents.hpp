#pragma once

#include <raylib.h>
#include "../../shared/Component.hpp"

struct Sprite {
    Texture2D spritesheet;
    Rectangle sprite;
    double height_padding;
    double width_padding;
    double height_max;
    double width_max;

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
