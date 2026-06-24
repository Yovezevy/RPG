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
    void startBattle();          // random encounter (pilih monster sesuai map/level)
    void startBattleWith(const MonsterDef& def);
    void doBattleAction(int choice);

    void drawHUD();
    void drawCharPanel(Character& c, float x, float y, bool active);
    void drawBar(float x, float y, float w, float h, float ratio, sf::Color col);
    void drawText(const std::string& s, float x, float y, unsigned size, sf::Color c);

    sf::FloatRect feetBox(const sf::Vector2f& pos, int fw, int fh) const;
    bool collides(const sf::FloatRect& box,
                  const std::vector<sf::FloatRect>& walls) const;

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

    // ---- Monster sprite sheet ----
    // Sheet: 576x384, frame 48x64, 12 col x 6 row
    // 4 monster per baris (tiap monster = 3 frame animasi), total 24 monster
    static constexpr int MON_FRAME_W  = 48;
    static constexpr int MON_FRAME_H  = 64;
    static constexpr int MON_PER_ROW  = 4;   // monster per baris sheet
    static constexpr int MON_ANIM_COL = 3;   // frame animasi per monster

    struct MonsterDef {
        std::string name;
        int sheetRow;       // baris di sheet (0-5)
        int sheetCol;       // indeks monster dalam baris (0-3) -> startX = sheetCol*3*48
        int hp, atk;
        int level;          // level minimum encounter
    };

    static const MonsterDef MONSTER_TABLE[];
    static constexpr int MONSTER_COUNT = 24;

    struct Enemy {
        std::string name;
        int hp, maxHp, atk;
        int sheetRow, sheetCol;  // posisi di sprite sheet
        int animFrame  = 0;
        float animTimer = 0.f;
    } enemy{};

    sf::Texture  monsterTex;
    sf::Sprite   monsterSprite;
    bool         monsterTexLoaded = false;

    std::vector<std::string> battleLog;
    bool  playerTurn  = true;
    float battleStep  = 0.f;
    float encounterTimer = 0.f;
    float battleAnim = 0.f;

    void updateMonsterFrame();
};
