#pragma once

#include <raylib.h>
#include "../../shared/Component.hpp"

struct Sprite {
	Texture2D sprite;

	Sprite(char *path, int w, int h) {
		Image sprit = LoadImage(path);
		ImageResize(&sprit, w, h);
		sprite = LoadTextureFromImage(sprit);
		};

	Sprite(const char *path, int w, int h) {
		Image sprit = LoadImage(path);
		ImageResize(&sprit, w, h);
		sprite = LoadTextureFromImage(sprit);
		};

	Sprite(const char *path): sprite(LoadTextureFromImage(LoadImage(path))) {};
	Sprite(char *path): sprite(LoadTextureFromImage(LoadImage(path))) {};
};

struct Animation {
	// Rectangle rect; //contains size as well
	Sprite sprite;
	Position* frames; // contains a list of position for each frames of an animated picture.
};
