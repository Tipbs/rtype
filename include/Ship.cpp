#include <cmath>
#include <iostream>
#include "Ship.hpp"
#include "raylib.h"

Ship::Ship(int width, int height, int ShipNb)
{
    Image spaceship = LoadImage("ressources/Sprites/r-typesheet42.gif");
    Rectangle shipRec;
    ImageResizeNN(&spaceship, width / 3, height / 3);
    Texture2D ship = LoadTextureFromImage(spaceship);
    shipRec.width = (float)ship.width / 5;
    shipRec.height = (float)ship.height / 5;
    shipRec.x = 2 * (float)ship.width / 5;
    shipRec.y = ShipNb * (float)ship.height / 5;

    Vector2 ShipPos = {(float)width/2 - (float)shipRec.width/2, (float)height/2 - (float)shipRec.height/2};

    this->ship = ship;
    this->ShipPos = ShipPos;
    this->ShipRec = shipRec;
    UnloadImage(spaceship);
}

void Ship::UpdateShip(int width, int height, int Keyup, int Keydown, int Keyleft, int KeyRight)
{
    const float Speed = (float)height/200;
    const int ShipBigUp = 4 * (float)this->ship.width / 5;
    const int ShipUp = 3 * (float)this->ship.width / 5;
    const int ShipMid = 2 * (float)this->ship.width / 5;
    const int ShipDown = 1 * (float)this->ship.width / 5;
    const int ShipBigDown = 0 * (float)this->ship.width / 5;

    //Movement handling and clamping
    Vector2 Moves = {0., 0.};

    if (IsKeyDown(KeyRight)) Moves.x = (this->ShipPos.x + this->ShipRec.width > width) ? 0 : Speed;
    if (IsKeyDown(Keyleft)) Moves.x = this->ShipPos.x < 0 ? 0 : -1 * Speed;
    if (IsKeyDown(Keyup)) {
        Moves.y = this->ShipPos.y < 0 ? 0 : -1 * Speed;
        this->ShipRec.x = ShipBigUp;
    }
    if (IsKeyDown(Keydown)) {
        Moves.y = this->ShipPos.y + this->ShipRec.height > height ? 0 : Speed;
        this->ShipRec.x = ShipBigDown;
    }
    if (IsKeyDown(Keydown) == IsKeyDown(Keyup)) this->ShipRec.x = ShipMid;

    if (Moves.x != 0. && Moves.y != 0.) {
        Moves.x *= (std::sqrt(2)/2);
        Moves.y *= (std::sqrt(2)/2);
    }
    this->ShipPos.x+=Moves.x;
    this->ShipPos.y+=Moves.y;
}

Ship::~Ship()
{
    UnloadTexture(this->ship);
}

Texture2D Ship::GetShip()
{
    return this->ship;
}

Rectangle Ship::GetRect()
{
    return this->ShipRec;
}

Vector2 Ship::GetPos()
{
    return this->ShipPos;
}