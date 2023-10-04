#include <iostream>
#include "raylib.h"

#ifndef SHIP_HPP
    #define SHIP_HPP

    class Ship {
        private:
            Rectangle ShipRec;
            Vector2 ShipPos;
            Texture2D ship;
            Texture2D shoot;
            Rectangle ShootRec;
            Vector2 ShootPos;
            float currentHeight;
            double lastPressedTime;
            double shootAnimCounter;
        public:
            Ship(int width, int height, int ShipNb);
            ~Ship();
            void UpdateShip(int width, int height, int Keyup, int Keydown, int Keyleft, int KeyRight);
            Texture2D GetShip();
            Texture2D GetShoot();
            Rectangle GetRect();
            Rectangle GetShootRect();
            Vector2 GetPos();
            Vector2 GetShootPos();
            void UpdateShipShoot(int KeyShoot);
    };


#endif