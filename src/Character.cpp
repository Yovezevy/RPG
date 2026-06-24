#include "Character.hpp"
#include <iostream>

bool Character::load(const std::string& walkPath, const std::string& facePath,
                     const std::string& charName, SheetConfig sheetCfg) {
    name = charName;
    if (!walkTex.loadFromFile(walkPath)) {
        std::cerr << "GAGAL muat " << walkPath << "\n";
        return false;
    }
    faceTex.loadFromFile(facePath);
    cfg = sheetCfg;
    frameW = cfg.frameW; frameH = cfg.frameH;
    sprite.setTexture(walkTex);
    sprite.setOrigin(frameW / 2.f, frameH / 2.f);
    setFrame();
    return true;
}

void Character::loadBattle(const std::string& path) {
    if (battleTex.loadFromFile(path)) {
        battleSprite.setTexture(battleTex);
        battleSprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
        battleSprite.setOrigin(32, 32);
        battleSprite.setScale(-2.2f, 2.2f);   // <-- X NEGATIF = flip menghadap kanan
    }
}


void Character::setFrame() {
    int sx = (cfg.originCol + frame) * cfg.frameW;
    int sy = (cfg.originRow + dirToRow[dirRow]) * cfg.frameH; // pakai remap
    sprite.setTextureRect(sf::IntRect(sx, sy, cfg.frameW, cfg.frameH));
}
