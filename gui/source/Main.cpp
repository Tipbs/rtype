#include <chrono>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <semaphore>
#include <raylib.h>
#include "../../shared/Parser.hpp"
#include "../../shared/Registry.hpp"
#include "Client.hpp"
#include "GraphicComponents.hpp"
#include "GraphicSystems.hpp"

int main(int ac, char **av)
{
    Colision col(Tag::Player, Tag::Enemy);
    std::cout << col.to_string() << std::endl;
}
