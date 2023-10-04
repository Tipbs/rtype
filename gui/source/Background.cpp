// #include <iostream>
// #include "../include/Background.hpp"


// void Background::GetElementsRandom() {
//     std::size_t elementSize = 0;
//     for (std::size_t i = 0; i < 250; ++i) {
//         elementSize = std::rand() % 15 + 5;
//         _elementsVector.push_back({ elementSize, std::size_t(std::rand() % screenWidth),
//             std::size_t(std::rand() % screenHeight), BLUE, std::size_t(std::rand() % 3 + 0), true });
//     }
// }

// void Background::UpdateStarGlowing(Element &star) { //Element ?
//     if (star.isIncrement)
//         ++star.glow;
//     else
//         --star.glow;
//     if (star.glow == 4)
//         star.isIncrement = false;
//     if (star.glow == 0)
//         star.isIncrement = true;
// }

// void Background::UpdateStarsPosition(int screenWidth) {
//     for (auto &star : _elementsVector) {
//         if (star.posX <= 0) {
//             star.posX = screenWidth;
//             star.posY = std::rand() % 720;
//         } else {
//             star.posX -= 1;
//         }
//         if (std::rand() % 50 == 1)
//             UpdateStarGlowing(star);
//     }
// }

// void Background::Draw() {
//     for (auto i : _elementsVector) {
//         _bgElement.SetWidth(i.size);
//         _bgElement.SetHeight(i.size);
//         _bgElement.Draw(i.posX, i.posY, {
//             i.color.r, i.color.g, i.color.b, static_cast<unsigned char>(i.glow * 50) });
//     }
// }

// Background::Background(std::string bgElementTexturePath) {
//     GetElementsRandom();
//     _bgElement = raylib::Texture(bgElementTexturePath);
// };

// Background::~Background() {
//     _bgElement.Unload();
// };