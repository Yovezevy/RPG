#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

struct Portal {
    sf::FloatRect area;
    int           targetMap;
    sf::Vector2f  spawn;
};

struct GameMap {
    sf::Texture tex;
    sf::Sprite  sprite;
    std::vector<sf::FloatRect> walls;
    std::vector<sf::FloatRect> pits;     // lubang mematikan
    std::vector<Portal>        portals;
    sf::Vector2f               defaultSpawn;

    bool load(const std::string& path);
};
