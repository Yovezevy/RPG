#include "GameMap.hpp"
#include <iostream>

bool GameMap::load(const std::string& path) {
    if (!tex.loadFromFile(path)) {
        std::cerr << "GAGAL muat " << path << "\n";
        return false;
    }
    sprite.setTexture(tex);
    return true;
}
