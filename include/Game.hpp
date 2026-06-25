#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Constants.hpp"
#include "Character.hpp"
#include "GameMap.hpp"

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void handleKeyPress(const sf::Event& e);
    void startDialog(const std::string& name, std::vector<std::string> lines);
    void startBattle(int mapEnemyIdx);
    void doBattleAction(int choice);

    void drawHUD();
    void drawCharPanel(Character& c, float x, float y, bool active);
    void drawBar(float x, float y, float w, float h, float ratio, sf::Color col);
    void drawText(const std::string& s, float x, float y, unsigned size, sf::Color c);

    sf::FloatRect feetBox(const sf::Vector2f& pos, int fw, int fh) const;
    bool collides(const sf::FloatRect& box, const std::vector<sf::FloatRect>& walls) const;

    sf::RenderWindow window;
    sf::View         view;
    sf::Font         font;
    bool             hasFont = false;
    bool             debugMode = false;

    std::vector<Character> party;
    int          activeIdx = 0;
    sf::Vector2f playerPos;
    float        animTimer = 0.f;

    std::vector<GameMap> maps;
    int currentMap = 0;
    State state = State::Explore;

    std::vector<std::string> dialogLines;
    int          dialogIndex = 0;
    std::string  dialogName;
    sf::FloatRect npcZone;

    static constexpr int MON_FRAME_W = 48;
    static constexpr int MON_FRAME_H = 48;
    static constexpr int MONSTER_COUNT = 16;

    struct MonsterDef {
        std::string name;
        int texID, gridX, gridY, hp, atk, exp;
    };
    static const MonsterDef MONSTER_TABLE[MONSTER_COUNT];

    struct Enemy {
        std::string name;
        int hp, maxHp, atk, exp, texID, gridX, gridY;
    } enemy{};

    sf::Texture  enemyTex[2], battleBgTex, playerTurnTex, enemyTurnTex;
    sf::Sprite   monsterSprite, battleBgSprite, playerTurnSprite, enemyTurnSprite;
    
    std::vector<std::string> battleLog;
    bool  playerTurn = true;
    float battleStep = 0.f, battleAnim = 0.f, bannerTimer = 0.f;
    int   currentBanner = 0, activeMapEnemyIdx = -1;
};