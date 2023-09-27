#include <cmath>
#include <iostream>
#include "Background.hpp"
#include "raylib.h"
#include "Ship.hpp"

int backgroundMaker()
{
    const int ScreenWidth = 1280;
    const int ScreenHeight = 720;
    InitWindow(ScreenWidth, ScreenHeight, "R-Type");
    SetTargetFPS(144);
    Image bg = LoadImage("ressources/Backgrounds/Back.png");
    ImageResize(&bg, ScreenWidth, ScreenHeight);
    Texture2D bgText = LoadTextureFromImage(bg);

    Ship play0(ScreenWidth, ScreenHeight, 0);
    Ship play1(ScreenWidth, ScreenHeight, 1);
    Ship play2(ScreenWidth, ScreenHeight, 2);
    Ship play3(ScreenWidth, ScreenHeight, 3);
    Ship play4(ScreenWidth, ScreenHeight, 4);

    UnloadImage(bg);

    while (!WindowShouldClose()) {

        play0.UpdateShip(ScreenWidth, ScreenHeight, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);
        play1.UpdateShip(ScreenWidth, ScreenHeight, KEY_W, KEY_S, KEY_A, KEY_D);
        play2.UpdateShip(ScreenWidth, ScreenHeight, KEY_DOWN, KEY_UP, KEY_RIGHT, KEY_LEFT);
        play3.UpdateShip(ScreenWidth, ScreenHeight, KEY_S, KEY_W, KEY_D, KEY_A);
        play4.UpdateShip(ScreenWidth, ScreenHeight, KEY_I, KEY_K, KEY_J, KEY_L);

        ClearBackground(BLACK);
        BeginDrawing();
            DrawTexture(bgText, 0, 0, WHITE);
            DrawTextureRec(play0.GetShip(), play0.GetRect(), play0.GetPos(), WHITE);
            DrawTextureRec(play1.GetShip(), play1.GetRect(), play1.GetPos(), WHITE);
            DrawTextureRec(play2.GetShip(), play2.GetRect(), play2.GetPos(), WHITE);
            DrawTextureRec(play3.GetShip(), play3.GetRect(), play3.GetPos(), WHITE);
            DrawTextureRec(play4.GetShip(), play4.GetRect(), play4.GetPos(), WHITE);
        EndDrawing();
    }
    UnloadTexture(bgText);
    play0.~Ship();
    play1.~Ship();
    play2.~Ship();
    play3.~Ship();
    play4.~Ship();

    CloseWindow();
    
    return 0;
}