#include "GraphicSystems.hpp"
#include <cstdlib>
#include <raylib.h>
#include "../../shared/Registry.hpp"
#include "../../shared/Sparse_array.hpp"
#include "GraphicComponents.hpp"
#include "../../shared/indexed_zipper.hpp"
#include "../../shared/zipper.hpp"

void display(
    Registry &r, sparse_array<Position> &positions, sparse_array<Size> &size,
    sparse_array<Sprite> &sprite, sparse_array<Player> &anim,
    sparse_array<Rectangle> &rectangles, sparse_array<InputField> &inputFields)
{
    BeginDrawing();
    for (auto &&[ind, pos, siz, spri] :
         indexed_zipper(positions, size, sprite)) {
        if (!(pos && siz && spri))
            continue;
        if (sprite[ind]->width_max == 9 && sprite[ind]->height_max == 1) {
            sprite[ind]->sprite.x =
                (sprite[ind]->sprite.x / sprite[ind]->width_padding ==
                         sprite[ind]->width_max
                     ? 0
                     : (sprite[ind]->sprite.x + sprite[ind]->width_padding));
        }
        if (anim[ind]) {
            sprite[ind]->sprite.y =
                anim[ind]->color_id * sprite[ind]->height_padding;
            if (anim[ind]->count >= 1.85)
                sprite[ind]->sprite.x = 4 * sprite[ind]->width_padding;
            else if (anim[ind]->count <= 0.15)
                sprite[ind]->sprite.x = 0 * sprite[ind]->width_padding;
            else if (anim[ind]->count <= 0.85)
                sprite[ind]->sprite.x = 1 * sprite[ind]->width_padding;
            else if (anim[ind]->count >= 1.15)
                sprite[ind]->sprite.x = 3 * sprite[ind]->width_padding;
            else
                sprite[ind]->sprite.x = 2 * sprite[ind]->width_padding;
        }

        Vector2 Rectpos = {
            (float) (positions[ind].value().pos_X),
            (float) positions[ind].value().pos_Y};
        DrawTextureRec(
            sprite[ind].value().spritesheet, sprite[ind].value().sprite,
            Rectpos, WHITE);

        // for (auto &&[inputField, rectangle]: zipper(inputFields, rectangles)) {
            // DrawText(inputField->field.c_str(), (int)rectangle->x + 5, (int)rectangle->y + 8, 40, MAROON);
        // }
    }
    EndDrawing();
}

void handle_dir_inputs(
    Registry &r, sparse_array<Direction> &dir, sparse_array<Player> &anim,
    sparse_array<Sprite> &sprite)
{
    for (auto &&[ind, diro, anima, sprit] : indexed_zipper(dir, anim, sprite)) {
        if (!(diro && anima && sprit))
            continue;
        const double AnimationPad = 0.02;
        double heigh = 1;
        if (anim[ind]->id == ind) {
            heigh = anim[ind]->count;
            Vector2 Moves = {0, 0};

            if (IsKeyDown(KEY_RIGHT))
                Moves.x += 1;
            if (IsKeyDown(KEY_LEFT))
                Moves.x -= 1;

            if (IsKeyDown(KEY_DOWN) == IsKeyDown(KEY_UP)) {
                heigh = (heigh < 1)    ? heigh + AnimationPad
                        : (heigh == 1) ? 1
                                       : heigh - AnimationPad;
            } else if (IsKeyDown(KEY_UP)) {
                Moves.y -= rand(); // 1;
                heigh = (heigh >= 2) ? 2 : heigh + (5 * AnimationPad);
            } else if (IsKeyDown(KEY_DOWN)) {
                Moves.y += 1;
                heigh = (heigh <= 0) ? 0 : heigh - (5 * AnimationPad);
            }

            if (IsKeyPressed(KEY_C)) {
                anim[ind]->color_id =
                    anim[ind]->color_id == 4 ? 0 : anim[ind]->color_id + 1;
            }

            if (dir[ind]) { // 1 is the entity num representing the player seen
                            // here
                dir[ind].value().dir_X = Moves.x;
                dir[ind].value().dir_Y = Moves.y;
            }

            if (anim[ind])
                anim[ind]->count = heigh;
        }
    }
}

void handle_shoot_inputs(
    Registry &r, sparse_array<Player> &anim, sparse_array<Position> &pos,
    sparse_array<Size> &siz)
{
    for (auto &&[ind, anima, posi, sizo] : indexed_zipper(anim, pos, siz)) {
        if (!(anima && posi && sizo))
            continue;
        if (anim[ind]->id == ind) {
            if (IsKeyDown(KEY_SPACE))
                anim[ind]->IsShooting = true;
            if (IsKeyReleased(KEY_SPACE))
                anim[ind]->IsShooting = false;

            if (anim[ind]->IsShooting) {
                std::cout << "Player id " << anim[ind]->color_id
                          << " just shoot with a " << anim[ind]->weapon.type
                          << " typed weapon, with an attack speed of "
                          << anim[ind]->weapon.attack_speed << std::endl;

                create_ammo(
                    r,
                    Position(
                        pos[ind]->pos_X + siz[ind]->size_X,
                        pos[ind]->pos_Y + (siz[ind]->size_Y / 2.) - 15),
                    anim[ind]->weapon);
            }
        }
    }
}

void hadle_text_inputs(
    Registry &r, sparse_array<InputField> &inputFields,
    sparse_array<Rectangle> &rectangles)
{
    for (auto &&[inputField, rectangle]: zipper(inputFields, rectangles)) {
        if (CheckCollisionPointRec(GetMousePosition(), rectangle.value())) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            int key = GetCharPressed();
            int letterCount = 0;

            // Check if more characters have been pressed on the same frame
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (letterCount < 16)) {
                    inputField->field[letterCount] = (char)key;
                    inputField->field[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                    letterCount++;
                }
                key = GetCharPressed();  // Check next character in the queue
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                letterCount--;
                if (letterCount < 0) {
                    letterCount = 0;
                }
                inputField->field[letterCount] = '\0';
            }
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
    }
}

void make_infinite_background(
    Registry &r, sparse_array<Position> &pos, sparse_array<Size> &siz)
{
    if (pos[0] && siz[0]) {
        if (pos[0]->pos_X < -siz[0]->size_X)
            pos[0]->pos_X += siz[0]->size_X;
        if (pos[0]->pos_Y < -siz[0]->size_Y)
            pos[0]->pos_Y += siz[0]->size_Y;

        if (pos[0]->pos_X > 0)
            pos[0]->pos_X -= siz[0]->size_X;
        if (pos[0]->pos_Y > 0)
            pos[0]->pos_Y -= siz[0]->size_Y;
    }
}


// /*******************************************************************************************
// *
// *   raylib [text] example - Input Box
// *
// *   Example originally created with raylib 1.7, last time updated with raylib 3.5
// *
// *   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
// *   BSD-like license that allows static linking with closed source software
// *
// *   Copyright (c) 2017-2023 Ramon Santamaria (@raysan5)
// *
// ********************************************************************************************/

// #include "raylib.h"

// #define MAX_INPUT_CHARS     9

// //------------------------------------------------------------------------------------
// // Program main entry point
// //------------------------------------------------------------------------------------
// int main(void)
// {
//     // Initialization
//     //--------------------------------------------------------------------------------------
//     char name[MAX_INPUT_CHARS + 1] = "\0";      // NOTE: One extra space required for null terminator char '\0'
//     int letterCount = 0;

//     Rectangle textBox = { screenWidth/2.0f - 100, 180, 225, 50 };
//     bool mouseOnText = false;

//     int framesCounter = 0;

//     SetTargetFPS(10);               // Set our game to run at 10 frames-per-second
//     //--------------------------------------------------------------------------------------

//     // Main game loop
//     while (!WindowShouldClose())    // Detect window close button or ESC key
//     {
//         // Update
//         //----------------------------------------------------------------------------------
//         if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
//         else mouseOnText = false;

//         if (mouseOnText)
//         {
//             // Set the window's cursor to the I-Beam
//             SetMouseCursor(MOUSE_CURSOR_IBEAM);

//             // Get char pressed (unicode character) on the queue
//             int key = GetCharPressed();

//             // Check if more characters have been pressed on the same frame
//             while (key > 0)
//             {
//                 // NOTE: Only allow keys in range [32..125]
//                 if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
//                 {
//                     name[letterCount] = (char)key;
//                     name[letterCount+1] = '\0'; // Add null terminator at the end of the string.
//                     letterCount++;
//                 }

//                 key = GetCharPressed();  // Check next character in the queue
//             }

//             if (IsKeyPressed(KEY_BACKSPACE))
//             {
//                 letterCount--;
//                 if (letterCount < 0) letterCount = 0;
//                 name[letterCount] = '\0';
//             }
//         }
//         else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

//         if (mouseOnText) framesCounter++;
//         else framesCounter = 0;
//         //----------------------------------------------------------------------------------

//         // Draw
//         //----------------------------------------------------------------------------------
//         BeginDrawing();

//             ClearBackground(RAYWHITE);

//             DrawText("PLACE MOUSE OVER INPUT BOX!", 240, 140, 20, GRAY);

//             DrawRectangleRec(textBox, LIGHTGRAY);
//             if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
//             else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);

//             DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

//             DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, MAX_INPUT_CHARS), 315, 250, 20, DARKGRAY);

//             if (mouseOnText)
//             {
//                 if (letterCount < MAX_INPUT_CHARS)
//                 {
//                     // Draw blinking underscore char
//                     if (((framesCounter/20)%2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
//                 }
//                 else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
//             }

//         EndDrawing();
//         //----------------------------------------------------------------------------------
//     }

//     // De-Initialization
//     //--------------------------------------------------------------------------------------
//     CloseWindow();        // Close window and OpenGL context
//     //--------------------------------------------------------------------------------------

//     return 0;
// }

// // Check if any key is pressed
// // NOTE: We limit keys check to keys between 32 (KEY_SPACE) and 126
// bool IsAnyKeyPressed()
// {
//     bool keyPressed = false;
//     int key = GetKeyPressed();

//     if ((key >= 32) && (key <= 126)) keyPressed = true;

//     return keyPressed;
// }
