#include <iostream>
#include "raylib.h"
#include "Keybinds.hpp"

#ifndef SHIP_HPP
    #define SHIP_HPP

    class Ship {
        private:
            Rectangle ShipRec;
            Vector2 ShipPos;
            Texture2D ship;
        public:
            Ship(int width, int height, int ShipNb);
            ~Ship();
            void UpdateShip(int width, int height, int Keyup, int Keydown, int Keyleft, int KeyRight);
            Texture2D GetShip();
            Rectangle GetRect();
            Vector2 GetPos();
    };


#endif