#include <cmath>
#include <iostream>
#include "raylib.h"
#include "Background.hpp"

int backgroundMaker()
{
    const int ScreenWidth = 1280;
    const int ScreenHeight = 720;
    const float Speed = (float)ScreenHeight/256;
    InitWindow(ScreenWidth, ScreenHeight, "R-Type");
    SetTargetFPS(144);
    Image bg = LoadImage("ressources/Backgrounds/Back.png");
    ImageResize(&bg, ScreenWidth, ScreenHeight);
    Texture2D bgText = LoadTextureFromImage(bg);

    Image spaceship = LoadImage("ressources/Sprites/ship.png");
    Texture2D ship = LoadTextureFromImage(spaceship);

    UnloadImage(bg);
    Vector2 ShipPos = {(float)ScreenWidth/2 - (float)ship.width/2, (float)ScreenHeight/2 - (float)ship.height/2};

    while (!WindowShouldClose()) {

        //Movement handling and clamping
        Vector2 Moves = {0., 0.};
        if (IsKeyDown(KEY_RIGHT)) Moves.x = (ShipPos.x + ship.width > ScreenWidth) ? 0 : Speed;
        if (IsKeyDown(KEY_LEFT)) Moves.x = ShipPos.x < 0 ? 0 : -1 * Speed;
        if (IsKeyDown(KEY_UP)) Moves.y = ShipPos.y < 0 ? 0 : -1 * Speed;
        if (IsKeyDown(KEY_DOWN)) Moves.y = ShipPos.y + ship.height > ScreenHeight ? 0 : Speed;
        if (Moves.x != 0. && Moves.y != 0.) {
            Moves.x *= (std::sqrt(2)/2);
            Moves.y *= (std::sqrt(2)/2);
        }
        ShipPos.x+=Moves.x;
        ShipPos.y+=Moves.y;

        ClearBackground(BLACK);
        BeginDrawing();
            DrawTexture(bgText, 0, 0, WHITE);
            DrawTexture(ship, ShipPos.x, ShipPos.y, WHITE);
        EndDrawing();
    }
    UnloadImage(spaceship);
    CloseWindow();
    
    return 0;
}