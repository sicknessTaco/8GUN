// GameScene.hpp
#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>

class Engine;
class LevelMenu;

struct Bullet {
    float x, y;
    float vx, vy;
    bool active;
    Bullet(float x, float y, float vx, float vy) : x(x), y(y), vx(vx), vy(vy), active(true) {}
};

struct Enemy {
    float x, y;
    int type; // 0 = verde, 1 = amarillo, 2 = azul, 3 = rojo
    float shootTimer;
    float shootInterval;
    Enemy(float x, float y, int type);
};

class GameScene {
public:
    GameScene(Engine* engine, int zone, int song);
    ~GameScene();

    void update(float dt); // ✅ dt aquí
    void render();
    void handleInput(float dt); // ✅ Recibe dt

    bool isComplete() const { return m_complete; }
    bool isDead() const { return m_playerDead; }

    void completeSong();
    void shootPlayer();
private:
    Engine* m_engine;
    LevelMenu* m_levelMenu;
    int m_zone;
    int m_song;

    float m_playerX, m_playerY;
    float m_playerSpeed = 400.0f;
    int m_playerLives = 3;
    bool m_playerDead = false;
    float m_invincibilityTimer = 0.0f;
    float m_playerShotTimer = 0.0f;
    float m_playerShotInterval = 0.2f;

    std::vector<Bullet> m_bullets;
    const int MAX_BULLETS = 200;

    std::vector<Enemy> m_enemies;

    float m_lastBeatTime = 0.0f;
    float m_currentMusicTime = 0.0f;
    float m_beatInterval = 0.5f;
    bool m_isOnBeat = false;

    bool m_complete = false;
    bool m_musicPlaying = false;

    SDL_Texture* m_levelText = nullptr;

    void loadAssets();
    void spawnEnemies();
    void spawnBullets();
    void updateBullets(float dt);     // ✅ dt aquí
    void checkCollisions();
    void checkBeat();
    void updateInvincibility(float dt); // ✅ dt aquí
    void renderText(const char* text, int x, int y, SDL_Color color);
};