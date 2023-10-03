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
    shipRec.y = (ShipNb % 5) * (float)ship.height / 5;

    Vector2 ShipPos = {(float)width/2 - (float)shipRec.width/2, (float)height/2 - (float)shipRec.height/3};

    Image spaceshoot = LoadImage("ressources/Sprites/shoot_charge.png");
    Rectangle shootRec;
    ImageResizeNN(&spaceshoot, height/4*3, height/12);
    Texture2D shoot = LoadTextureFromImage(spaceshoot);
    shootRec.width = (float)shoot.width / 9;
    shootRec.height = (float)shoot.height;
    shootRec.x = 0 * (float)shoot.width /9;
    shootRec.y = (float)shoot.height;

    Vector2 ShootPos = {(float)width/2 + (float)shipRec.width/2 - (float)shipRec.width/6, (float)height/2 - (float)shipRec.height/3};

    this->ship = ship;
    this->ShipPos = ShipPos;
    this->ShipRec = shipRec;
    this->currentHeight = 1;
    this->lastPressedTime = 0;
    this->shoot = shoot;
    this->ShootRec = shootRec;
    this->ShootPos = ShootPos;
    this->shootAnimCounter = 8;
    UnloadImage(spaceship);
}

void Ship::UpdateShip(int width, int height, int Keyup, int Keydown, int Keyleft, int KeyRight)
{
    const float AnimationPad = 0.02;
    const float Speed = (float)height/200;
    const int ShipBigUp = 4 * (float)this->ship.width / 5;
    const int ShipUp = 3 * (float)this->ship.width / 5;
    const int ShipMid = 2 * (float)this->ship.width / 5;
    const int ShipDown = 1 * (float)this->ship.width / 5;
    const int ShipBigDown = 0 * (float)this->ship.width / 5;

    //Movement handling and clamping
    Vector2 Moves = {0., 0.};

    if (IsKeyDown(KeyRight)) Moves.x += (this->ShipPos.x + this->ShipRec.width > width) ? 0 : Speed;
    if (IsKeyDown(Keyleft)) Moves.x += this->ShipPos.x < 0 ? 0 : -1 * Speed;
    if (IsKeyDown(Keyup)) {
        Moves.y += this->ShipPos.y < 0 ? 0 : -1 * Speed;
        this->currentHeight = (this->currentHeight >= 2) ? 2 : this->currentHeight + (5 * AnimationPad);
    }
    if (IsKeyDown(Keydown)) {
        Moves.y += this->ShipPos.y + this->ShipRec.height > height ? 0 : Speed;
        this->currentHeight = (this->currentHeight <= 0) ? 0 : this->currentHeight - (5 * AnimationPad);
    }
    if (IsKeyDown(Keydown) == IsKeyDown(Keyup)) {
        this->currentHeight = (this->currentHeight < 1) ? this->currentHeight + AnimationPad : (this->currentHeight == 1) ? 1 : this->currentHeight - AnimationPad;
    }

    if (Moves.x != 0. && Moves.y != 0.) {
        Moves.x *= (std::sqrt(2)/2);
        Moves.y *= (std::sqrt(2)/2);
    }

    // Handle the graphical tilt of the ship, depending on a value made in range [0,2]

    if (this->currentHeight >= 1.85) {
        this->ShipRec.x = ShipBigUp;
    } else if (this->currentHeight <= 0.15) {
        this->ShipRec.x = ShipBigDown;
    } else if (this->currentHeight <= 0.85) {
        this->ShipRec.x = ShipDown;
    } else if (this->currentHeight >= 1.15) {
        this->ShipRec.x = ShipUp;
    } else {
        this->ShipRec.x = ShipMid;
    }

    this->ShipPos.x+=Moves.x;
    this->ShipPos.y+=Moves.y;
    
    this->ShootPos.x+=Moves.x;
    this->ShootPos.y+=Moves.y;
}

void Ship::UpdateShipShoot(int KeyShoot)
{
    if (IsKeyReleased(KeyShoot)) {
        std::cout << "Time held pressed : " << (GetTime() - this->lastPressedTime) << " seconds \n";
        //Anim post shoot + envoi packet
    }
    if (!IsKeyDown(KeyShoot)) {
        this->lastPressedTime = GetTime();
        this->shootAnimCounter = 8;
        this->ShootRec.x = (this->shootAnimCounter  * (float)shoot.width / 9);
    } else {
        if (this->shootAnimCounter >= 7.9) {this->shootAnimCounter = 0;}
        else {this->shootAnimCounter += 0.1;}
        this->ShootRec.x = ((int)this->shootAnimCounter  * (float)shoot.width / 9);
    }
}

Ship::~Ship()
{
    UnloadTexture(this->ship);
    UnloadTexture(this->shoot);
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

Texture2D Ship::GetShoot()
{
    return this->shoot;
}

Rectangle Ship::GetShootRect()
{
    return this->ShootRec;
}

Vector2 Ship::GetShootPos()
{
    return this->ShootPos;
}