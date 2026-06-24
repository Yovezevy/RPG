#include "Game.hpp"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <algorithm>

// ==================== MONSTER TABLE ====================
// Format: {name, sheetRow, sheetCol, hp, atk, minLevel}
// Sheet: 576x384, 48x64 per frame, 4 monster per baris, 3 frame animasi
const Game::MonsterDef Game::MONSTER_TABLE[] = {
    // Baris 0
    {"Gadis Roh",       0, 0,  40,  8, 1},
    {"Goblin Hijau",    0, 1,  55, 12, 1},
    {"Serigala Abu",    0, 2,  70, 14, 2},
    {"Ksatria Gelap",   0, 3,  90, 18, 3},
    // Baris 1
    {"Harpy Merah",     1, 0,  60, 10, 1},
    {"Orc Besar",       1, 1,  80, 16, 2},
    {"Serigala Biru",   1, 2,  85, 15, 2},
    {"Iblis Petir",     1, 3, 110, 22, 4},
    // Baris 2
    {"Rubah Putih",     2, 0,  50,  9, 1},
    {"Shadown Monk",    2, 1,  75, 13, 2},
    {"Tengkorak Raja",  2, 2,  95, 20, 3},
    {"Iblis Merah",     2, 3, 130, 25, 5},
    // Baris 3
    {"Rubah Silver",    3, 0,  65, 11, 2},
    {"Goat Demon",      3, 1,  70, 14, 2},
    {"Armor Maut",      3, 2, 100, 21, 3},
    {"Naga Ungu",       3, 3, 150, 28, 6},
    // Baris 4
    {"Rubah Api",       4, 0,  55, 10, 1},
    {"Shadow Lord",     4, 1,  85, 17, 3},
    {"Yeti Biru",       4, 2,  90, 16, 3},
    {"Garuda Ungu",     4, 3, 140, 26, 5},
    // Baris 5
    {"Rubah Baja",      5, 0,  60, 11, 2},
    {"Titan Gelap",     5, 1,  95, 19, 3},
    {"Raksasa Beku",    5, 2, 115, 23, 4},
    {"Naga Agung",      5, 3, 180, 32, 7},
};

// ==================== KONSTRUKTOR ====================
Game::Game()
: window(sf::VideoMode(WIN_W, WIN_H), "Kastil Terkutuk - RPG"),
  view(sf::FloatRect(0, 0, WIN_W, WIN_H))
{
    window.setFramerateLimit(60);
    hasFont = font.loadFromFile("assets/font.ttf");
    std::srand((unsigned)time(nullptr));

    SheetConfig walkCfg;
    walkCfg.frameW = 48; walkCfg.frameH = 48;
    walkCfg.animCols = 3; walkCfg.originCol = 0; walkCfg.originRow = 0;

    party.resize(2);
    if (!party[0].load("assets/char1_walk.png", "assets/char1_face.png", "Reyn", walkCfg)) std::exit(1);
    if (!party[1].load("assets/char2_walk.png", "assets/char2_face.png", "Mira", walkCfg)) std::exit(1);
    party[0].loadBattle("assets/char1_battle.png");
    party[1].loadBattle("assets/char2_battle.png");

    // ====== PERBAIKI ARAH DI SINI (lihat catatan di bawah) ======
    // Kalau atas-bawah tertukar: {3,1,2,0}. Kalau kiri-kanan: {0,2,1,3}.
    // Kalau semua terbalik: {3,2,1,0}. Default: {0,1,2,3}.
    int remap[4] = {0, 1, 2, 3};
    for (int i=0;i<4;i++){ party[0].dirToRow[i]=remap[i]; party[1].dirToRow[i]=remap[i]; }

    party[0].maxHp=120; party[0].hp=120; party[0].maxMp=30; party[0].mp=30; party[0].atk=22;
    party[1].maxHp=80;  party[1].hp=80;  party[1].maxMp=80; party[1].mp=80; party[1].atk=14;

    // ====== MAP ======
    maps.resize(2);
    maps[0].load("assets/map_castle.png");
    maps[1].load("assets/map_dungeon.png");

    float cW = maps[0].tex.getSize().x, cH = maps[0].tex.getSize().y; // 1200x1200
    float dW = maps[1].tex.getSize().x, dH = maps[1].tex.getSize().y; // 1200x1200

    // ---- KASTIL ----
    maps[0].defaultSpawn = { cW*0.5f, cH*0.82f };   // tanah bawah
    maps[0].walls = {
        { 0, 0, cW, cH*0.60f },                     // bangunan kastil atas
        { 0, 0, 16, cH }, { cW-16, 0, 16, cH }      // tepi
    };
    // Portal kastil -> DUNGEON, mendarat di lantai aman (600,1080 dari screenshot)
    maps[0].portals.push_back({
        { cW*0.5f-30, cH*0.58f, 60, 34 }, 1, { 600.f, 1080.f } });

    // ---- DUNGEON (koordinat dari screenshot kamu) ----
    maps[1].defaultSpawn = { 600.f, 1080.f };       // <-- LANTAI AMAN (screenshot 3)
    maps[1].walls = {
        {0,0,dW,16}, {0,dH-16,dW,16}, {0,0,16,dH}, {dW-16,0,16,dH}
    };
    // Portal balik ke kastil (taruh dekat spawn dungeon)
    maps[1].portals.push_back({
        { 560.f, 1140.f, 80.f, 40.f }, 0, { cW*0.5f, cH*0.80f } });

    // ---- LUBANG MATI (perkiraan dari area hitam; verifikasi pakai kotak BIRU F1) ----
    maps[1].pits = {
        { 340.f,  80.f, 360.f, 130.f },   // area hitam atas-tengah (screenshot 1)
        {   0.f, 560.f, 110.f, 320.f },   // hitam tepi kiri
        { 760.f, 340.f, 200.f, 160.f }    // hitam kanan-tengah
    };

    playerPos = maps[0].defaultSpawn;
    npcZone = sf::FloatRect(maps[0].defaultSpawn.x-90, maps[0].defaultSpawn.y, 60, 60);

    // ====== MONSTER SPRITE SHEET ======
    monsterTexLoaded = monsterTex.loadFromFile("assets/monster.png");
    if (monsterTexLoaded) {
        monsterSprite.setTexture(monsterTex);
        monsterSprite.setOrigin(MON_FRAME_W / 2.f, MON_FRAME_H / 2.f);
    }
}

// ==================== LOOP ====================
void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        processEvents(); update(dt); render();
    }
}

// ==================== COLLISION ====================
sf::FloatRect Game::feetBox(const sf::Vector2f& pos, int fw, int fh) const {
    float w = fw*0.5f, h = fh*0.35f;
    return sf::FloatRect(pos.x - w/2.f, pos.y + fh/2.f - h, w, h);
}
bool Game::collides(const sf::FloatRect& box,
                    const std::vector<sf::FloatRect>& walls) const {
    for (auto& w : walls) if (box.intersects(w)) return true;
    return false;
}

// ==================== EVENT ====================
void Game::processEvents() {
    sf::Event e;
    while (window.pollEvent(e)) {
        if (e.type == sf::Event::Closed) window.close();
        if (e.type == sf::Event::KeyPressed) handleKeyPress(e);
    }
}
void Game::handleKeyPress(const sf::Event& e) {
    if (e.key.code == sf::Keyboard::Escape) window.close();
    if (e.key.code == sf::Keyboard::F1) debugMode = !debugMode;

    if (state == State::Explore && e.key.code == sf::Keyboard::Tab)
        activeIdx = (activeIdx + 1) % (int)party.size();

    if (state == State::Dialog &&
        (e.key.code == sf::Keyboard::Enter || e.key.code == sf::Keyboard::Space)) {
        if (++dialogIndex >= (int)dialogLines.size()) state = State::Explore;
    }
    if (state == State::Explore &&
        (e.key.code == sf::Keyboard::Enter || e.key.code == sf::Keyboard::Space)) {
        Character& me = party[activeIdx];
        if (currentMap == 0 &&
            feetBox(playerPos, me.frameW, me.frameH).intersects(npcZone)) {
            startDialog("Penjaga Tua", {
                "Di dalam dungeon ada lubang maut...",
                "Hindari area gelap, atau kau akan jatuh!",
                "Tekan [Tab] untuk berganti rekan."
            });
        }
    }
    if (state == State::Battle && playerTurn) {
        if (e.key.code == sf::Keyboard::Num1) doBattleAction(1);
        else if (e.key.code == sf::Keyboard::Num2) doBattleAction(2);
        else if (e.key.code == sf::Keyboard::Num3) doBattleAction(3);
    }
}
void Game::startDialog(const std::string& name, std::vector<std::string> lines) {
    dialogName = name; dialogLines = std::move(lines);
    dialogIndex = 0; state = State::Dialog;
}

// ==================== BATTLE ====================
void Game::startBattleWith(const MonsterDef& def) {
    enemy.name     = def.name;
    enemy.hp       = def.hp;
    enemy.maxHp    = def.hp;
    enemy.atk      = def.atk;
    enemy.sheetRow = def.sheetRow;
    enemy.sheetCol = def.sheetCol;
    enemy.animFrame  = 0;
    enemy.animTimer  = 0.f;

    // Set texture rect ke frame pertama monster
    if (monsterTexLoaded) {
        int sx = def.sheetCol * MON_ANIM_COL * MON_FRAME_W;
        int sy = def.sheetRow * MON_FRAME_H;
        monsterSprite.setTextureRect(sf::IntRect(sx, sy, MON_FRAME_W, MON_FRAME_H));
        // Scale: tampilkan lebih besar (2.5x), flip X agar menghadap kiri (ke player)
        monsterSprite.setScale(-2.5f, 2.5f);
    }

    battleLog.clear();
    battleLog.push_back("Seekor " + enemy.name + " muncul!");
    playerTurn = true; battleAnim = 0.f; state = State::Battle;
}

void Game::startBattle() {
    // Pilih monster acak sesuai map
    // Map 0 (kastil): level 1-3, Map 1 (dungeon): level 3-7
    int minLv = (currentMap == 0) ? 1 : 3;
    int maxLv = (currentMap == 0) ? 3 : 7;

    // Kumpulkan kandidat
    std::vector<int> candidates;
    for (int i = 0; i < MONSTER_COUNT; i++) {
        if (MONSTER_TABLE[i].level >= minLv && MONSTER_TABLE[i].level <= maxLv)
            candidates.push_back(i);
    }
    int pick = candidates.empty() ? 0 : candidates[rand() % candidates.size()];
    startBattleWith(MONSTER_TABLE[pick]);
}

void Game::updateMonsterFrame() {
    // Animasi idle monster: 3 frame, 4 FPS
    enemy.animTimer += 1.f / 60.f;
    if (enemy.animTimer >= 1.f / 4.f) {
        enemy.animTimer = 0.f;
        enemy.animFrame = (enemy.animFrame + 1) % MON_ANIM_COL;
        if (monsterTexLoaded) {
            int sx = (enemy.sheetCol * MON_ANIM_COL + enemy.animFrame) * MON_FRAME_W;
            int sy = enemy.sheetRow * MON_FRAME_H;
            monsterSprite.setTextureRect(sf::IntRect(sx, sy, MON_FRAME_W, MON_FRAME_H));
        }
    }
}
void Game::doBattleAction(int choice) {
    Character& me = party[activeIdx]; int dmg = 0;
    if (choice == 1) {
        dmg = me.atk + (rand()%6 - 2);
        battleLog.push_back(me.name + " menyerang! " + std::to_string(dmg) + " dmg.");
    } else if (choice == 2) {
        if (me.mp >= 10) { me.mp-=10; dmg = me.atk*2-4;
            battleLog.push_back(me.name + " memakai Skill! " + std::to_string(dmg) + " dmg.");
        } else { battleLog.push_back("MP tidak cukup!"); return; }
    } else if (choice == 3) {
        battleLog.push_back("Kamu kabur!"); state = State::Explore; return;
    }
    enemy.hp -= dmg; playerTurn = false; battleStep = 0.7f; battleAnim = 0.3f;
}

// ==================== UPDATE ====================
void Game::update(float dt) {
    if (state == State::Explore) {
        Character& me = party[activeIdx];
        sf::Vector2f move(0,0); bool moving = false;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))       { move.x=-1; me.dirRow=1; moving=true; }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { move.x= 1; me.dirRow=2; moving=true; }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    { move.y=-1; me.dirRow=3; moving=true; }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  { move.y= 1; me.dirRow=0; moving=true; }

        if (moving) {
            sf::Vector2f next = playerPos + move * PLAYER_SPEED * dt;
            sf::Vector2f tx = {next.x, playerPos.y};
            if (!collides(feetBox(tx, me.frameW, me.frameH), maps[currentMap].walls))
                playerPos.x = next.x;
            sf::Vector2f ty = {playerPos.x, next.y};
            if (!collides(feetBox(ty, me.frameW, me.frameH), maps[currentMap].walls))
                playerPos.y = next.y;

            animTimer += dt;
            if (animTimer >= 1.f/ANIM_FPS) { animTimer=0; me.frame=(me.frame+1)%me.cfg.animCols; }

            // Portal
            for (auto& p : maps[currentMap].portals)
                if (feetBox(playerPos, me.frameW, me.frameH).intersects(p.area)) {
                    currentMap = p.targetMap; playerPos = p.spawn; break;
                }
            // Lubang mematikan
            for (auto& pit : maps[currentMap].pits)
                if (feetBox(playerPos, me.frameW, me.frameH).intersects(pit)) {
                    me.hp = me.maxHp; currentMap = 0;
                    playerPos = maps[0].defaultSpawn; break;
                }
            // Random encounter di dungeon
            if (currentMap == 1) {
                encounterTimer += dt;
                if (encounterTimer > 2.f && (rand()%100) < 2) { encounterTimer=0; startBattle(); }
            }
        } else me.frame = 1;
        me.setFrame();
    }
    else if (state == State::Battle) {
        if (battleAnim > 0) battleAnim -= dt;
        updateMonsterFrame();   // animasi idle monster tiap frame
        if (!playerTurn) {
            battleStep -= dt;
            if (battleStep <= 0) {
                Character& me = party[activeIdx];
                if (enemy.hp <= 0) {
                    battleLog.push_back(enemy.name + " dikalahkan!");
                    me.exp += 20;
                    if (me.exp >= 50) { me.exp=0; me.level++; me.maxHp+=20; me.hp=me.maxHp; me.atk+=4;
                        battleLog.push_back(me.name + " LEVEL UP! Lv." + std::to_string(me.level)); }
                    state = State::Explore;
                } else {
                    int dmg = enemy.atk + (rand()%5 - 2);
                    me.hp -= dmg; battleAnim = 0.3f;
                    battleLog.push_back(enemy.name + " menyerang! " + std::to_string(dmg) + " dmg.");
                    if (me.hp <= 0) { me.hp=me.maxHp; currentMap=0;
                        playerPos=maps[0].defaultSpawn; state=State::Explore; }
                    playerTurn = true;
                }
            }
        }
        if (battleLog.size() > 5) battleLog.erase(battleLog.begin());
    }

    // Kamera
    sf::Vector2u ms = maps[currentMap].tex.getSize();
    float cx = std::max(WIN_W/2.f, std::min(playerPos.x, ms.x - WIN_W/2.f));
    float cy = std::max(WIN_H/2.f, std::min(playerPos.y, ms.y - WIN_H/2.f));
    if (ms.x < (unsigned)WIN_W) cx = ms.x/2.f;
    if (ms.y < (unsigned)WIN_H) cy = ms.y/2.f;
    view.setCenter(cx, cy);
}

// ==================== RENDER ====================
void Game::render() {
    window.clear(sf::Color(10,10,15));
    window.setView(view);
    window.draw(maps[currentMap].sprite);

    if (debugMode) {
        for (auto& w : maps[currentMap].walls) {
            sf::RectangleShape r({w.width,w.height}); r.setPosition(w.left,w.top);
            r.setFillColor(sf::Color(255,0,0,70)); window.draw(r);
        }
        for (auto& p : maps[currentMap].portals) {
            sf::RectangleShape r({p.area.width,p.area.height}); r.setPosition(p.area.left,p.area.top);
            r.setFillColor(sf::Color(0,255,0,120)); window.draw(r);
        }
        for (auto& pit : maps[currentMap].pits) {
            sf::RectangleShape r({pit.width,pit.height}); r.setPosition(pit.left,pit.top);
            r.setFillColor(sf::Color(0,120,255,130)); window.draw(r);
        }
    }

    Character& me = party[activeIdx];
    me.sprite.setPosition(playerPos);
    window.draw(me.sprite);

    window.setView(window.getDefaultView());
    drawHUD();

    if (state == State::Dialog) {
        sf::RectangleShape box({(float)WIN_W-40,130}); box.setPosition(20, WIN_H-150);
        box.setFillColor(sf::Color(15,20,40,235));
        box.setOutlineColor(sf::Color(126,200,255)); box.setOutlineThickness(2); window.draw(box);
        drawText(dialogName, 40, WIN_H-140, 18, sf::Color(126,200,255));
        if (dialogIndex < (int)dialogLines.size())
            drawText(dialogLines[dialogIndex], 40, WIN_H-105, 16, sf::Color::White);
        drawText("[Enter] lanjut", WIN_W-160, WIN_H-45, 13, sf::Color(150,150,150));
    }
    else if (state == State::Battle) {
        sf::RectangleShape bg({(float)WIN_W,(float)WIN_H});
        bg.setFillColor(sf::Color(20,10,30,235)); window.draw(bg);

        // ---- Monster ----
        if (monsterTexLoaded) {
            float shakeX = (!playerTurn && battleAnim > 0) ? (float)(rand()%6-3) : 0.f;
            float lungeX = (playerTurn  && battleAnim > 0) ? -30.f : 0.f;
            monsterSprite.setPosition(WIN_W * 0.68f + shakeX + lungeX, WIN_H * 0.38f);
            window.draw(monsterSprite);
        } else {
            // Fallback: kotak ungu jika texture gagal muat
            sf::RectangleShape foe({110,110}); foe.setPosition(WIN_W*0.62f, 140);
            foe.setFillColor(sf::Color(120,40,160));
            foe.setOutlineColor(sf::Color(200,120,255)); foe.setOutlineThickness(2);
            if (!playerTurn && battleAnim > 0) foe.move((rand()%6-3), 0);
            window.draw(foe);
        }

        if (me.battleTex.getSize().x > 0) {
            float lunge = (!playerTurn && battleAnim>0) ? 40.f : 0.f;
            float shake = (playerTurn && battleAnim>0) ? (rand()%6-3) : 0.f;
            me.battleSprite.setPosition(WIN_W*0.28f + lunge + shake, WIN_H*0.62f);
            window.draw(me.battleSprite);
        }
        drawText(enemy.name, WIN_W*0.52f, 55, 22, sf::Color(200,120,255));
        drawBar(WIN_W*0.52f, 85, 220, 16,
                (float)std::max(0,enemy.hp)/enemy.maxHp, sf::Color(155,89,182));
        drawText("HP: " + std::to_string(std::max(0,enemy.hp)) + "/" + std::to_string(enemy.maxHp),
                 WIN_W*0.52f, 105, 13, sf::Color(200,200,200));

        sf::RectangleShape lb({(float)WIN_W-80,140}); lb.setPosition(40, WIN_H-220);
        lb.setFillColor(sf::Color(0,0,0,160)); window.draw(lb);
        float ly = WIN_H-210;
        for (auto& l : battleLog) { drawText(l, 55, ly, 16, sf::Color::White); ly += 24; }
        if (playerTurn)
            drawText("[1] Serang   [2] Skill (-10 MP)   [3] Lari",
                     55, WIN_H-55, 18, sf::Color(126,200,255));
    }

    if (debugMode) {
        char buf[160];
        snprintf(buf, sizeof(buf), "POS x=%.0f y=%.0f | MAP %dx%d | F1=debug",
                 playerPos.x, playerPos.y,
                 maps[currentMap].tex.getSize().x, maps[currentMap].tex.getSize().y);
        drawText(buf, 10, WIN_H-25, 16, sf::Color::Yellow);
    }
    window.display();
}

// ==================== UI HELPER ====================
void Game::drawBar(float x, float y, float w, float h, float ratio, sf::Color col) {
    sf::RectangleShape bg({w,h}); bg.setPosition(x,y);
    bg.setFillColor(sf::Color(40,40,40)); window.draw(bg);
    sf::RectangleShape fg({w*std::max(0.f,ratio),h}); fg.setPosition(x,y);
    fg.setFillColor(col); window.draw(fg);
}
void Game::drawText(const std::string& s, float x, float y, unsigned size, sf::Color c) {
    if (!hasFont) return;
    sf::Text t(s, font, size); t.setPosition(x,y); t.setFillColor(c); window.draw(t);
}
void Game::drawCharPanel(Character& c, float x, float y, bool active) {
    sf::RectangleShape panel({190,70}); panel.setPosition(x,y);
    panel.setFillColor(sf::Color(15,20,40,220));
    panel.setOutlineThickness(active?3.f:1.f);
    panel.setOutlineColor(active?sf::Color(255,215,0):sf::Color(90,90,110));
    window.draw(panel);
    if (c.faceTex.getSize().x > 0) {
        sf::Sprite face(c.faceTex);
        int fs = std::min(c.faceTex.getSize().x, c.faceTex.getSize().y);
        face.setTextureRect(sf::IntRect(0,0,fs,fs));
        float sc = 60.f/fs; face.setScale(sc,sc); face.setPosition(x+5,y+5);
        window.draw(face);
    }
    float bx = x+72;
    drawText(c.name, bx, y+2, 14, sf::Color::White);
    drawBar(bx, y+24, 110, 12, (float)c.hp/c.maxHp, sf::Color(220,60,60));
    drawText(std::to_string(c.hp)+"/"+std::to_string(c.maxHp), bx+4, y+22, 10, sf::Color::White);
    drawBar(bx, y+42, 110, 12, (float)c.mp/c.maxMp, sf::Color(70,130,230));
    drawText(std::to_string(c.mp)+"/"+std::to_string(c.maxMp), bx+4, y+40, 10, sf::Color::White);
}
void Game::drawHUD() {
    drawCharPanel(party[0], 10, 10, activeIdx == 0);
    drawCharPanel(party[1], 10, 88, activeIdx == 1);
    drawText("[Tab]Ganti [Enter]Bicara [F1]Debug", 10, 165, 13, sf::Color(180,180,180));
}
