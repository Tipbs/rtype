#include <cmath>
#include <iostream>
#include "../include/Routine.hpp"
#include "raylib.h"
#include "../include/Ship.hpp"

int routineMaker()
{
    const int ScreenWidth = 1280;
    const int ScreenHeight = 720;
    InitWindow(ScreenWidth, ScreenHeight, "R-Type");
    SetTargetFPS(144);
    Image bg = LoadImage("ressources/Backgrounds/Back.png");
    ImageResize(&bg, ScreenWidth, ScreenHeight);
    Texture2D bgText = LoadTextureFromImage(bg);

    Ship play0(ScreenWidth, ScreenHeight, 0);
    // Ship play1(ScreenWidth, ScreenHeight, 1);
    // Ship play2(ScreenWidth, ScreenHeight, 2);
    // Ship play3(ScreenWidth, ScreenHeight, 3);
    // Ship play4(ScreenWidth, ScreenHeight, 9);

    UnloadImage(bg);

    while (!WindowShouldClose()) {

        play0.UpdateShip(ScreenWidth, ScreenHeight, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT);
        play0.UpdateShipShoot(KEY_SPACE);
        // play1.UpdateShip(ScreenWidth, ScreenHeight, KEY_W, KEY_S, KEY_A, KEY_D);
        // play1.UpdateShipShoot(KEY_SPACE);
        // play2.UpdateShip(ScreenWidth, ScreenHeight, KEY_T, KEY_G, KEY_F, KEY_H);
        // play2.UpdateShipShoot(KEY_SPACE);
        // play3.UpdateShip(ScreenWidth, ScreenHeight, KEY_SEMICOLON, KEY_X, KEY_Z, KEY_C);
        // play3.UpdateShipShoot(KEY_SPACE);
        // play4.UpdateShip(ScreenWidth, ScreenHeight, KEY_I, KEY_K, KEY_J, KEY_L);
        // play4.UpdateShipShoot(KEY_SPACE);

        ClearBackground(BLACK);
        BeginDrawing();
            DrawTexture(bgText, 0, 0, WHITE);
            DrawTextureRec(play0.GetShip(), play0.GetRect(), play0.GetPos(), WHITE);
            DrawTextureRec(play0.GetShoot(), play0.GetShootRect(), play0.GetShootPos(), WHITE);
            // DrawTextureRec(play1.GetShip(), play1.GetRect(), play1.GetPos(), WHITE);
            // DrawTextureRec(play1.GetShoot(), play1.GetShootRect(), play1.GetShootPos(), WHITE);
            // DrawTextureRec(play2.GetShip(), play2.GetRect(), play2.GetPos(), WHITE);
            // DrawTextureRec(play2.GetShoot(), play2.GetShootRect(), play2.GetShootPos(), WHITE);
            // DrawTextureRec(play3.GetShip(), play3.GetRect(), play3.GetPos(), WHITE);
            // DrawTextureRec(play3.GetShoot(), play3.GetShootRect(), play3.GetShootPos(), WHITE);
            // DrawTextureRec(play4.GetShip(), play4.GetRect(), play4.GetPos(), WHITE);
            // DrawTextureRec(play4.GetShoot(), play4.GetShootRect(), play4.GetShootPos(), WHITE);
        EndDrawing();
    }
    UnloadTexture(bgText);
    play0.~Ship();
    // play1.~Ship();
    // play2.~Ship();
    // play3.~Ship();
    // play4.~Ship();

    CloseWindow();
    
    return 0;
}

// Ship::UpdateShipShoot (add a shoot trigger right under the UpdateShip)
// class Ennemi to create an enemy on a pos withing [0, ScreenHeight] that will spawn on the right side