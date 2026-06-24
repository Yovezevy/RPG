#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct SheetConfig {
    int frameW = 48, frameH = 48;
    int animCols = 3;
    int originCol = 0, originRow = 0;
};

struct Character {
    std::string name;
    sf::Texture walkTex, faceTex, battleTex;
    sf::Sprite  sprite, battleSprite;

    SheetConfig cfg;
    int frameW = 0, frameH = 0;
    int frame = 1, dirRow = 0;

    // REMAP ARAH: [bawah, kiri, kanan, atas] -> baris di sheet
    int dirToRow[4] = {0, 1, 2, 3};

    int hp=100,maxHp=100, mp=50,maxMp=50, atk=18, level=1, exp=0;

    bool load(const std::string& walkPath, const std::string& facePath,
              const std::string& charName, SheetConfig sheetCfg);
    void loadBattle(const std::string& path);
    void setFrame();
    bool alive() const { return hp > 0; }
};
