// Engine.hpp
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

// ✅ DECLARACIONES ANTECEDENTES (forward declarations)
class LevelMenu;
class GameScene;

// ✅ Definiciones de tamaño lógico (8-bit)
constexpr int LOGICAL_WIDTH = 1280;
constexpr int LOGICAL_HEIGHT = 720;

class Engine {
public:
    Engine() = default;
    ~Engine();

    bool init();
    void quit();
    bool isRunning() const { return m_running; }

    SDL_Renderer* getRenderer() const { return m_renderer; }
    SDL_Texture* getTargetTexture() const { return m_targetTexture; }
    int getLogicalWidth() const { return LOGICAL_WIDTH; }
    int getLogicalHeight() const { return LOGICAL_HEIGHT; }

    void setScene(int sceneId);
    int getScene() const { return m_currentScene; }

    // ✅ MÉTODOS DE ACCESO A ESCENAS (funcionan ahora)
    Mix_Music* getMusic() const { return m_music; }

    void setLevelMenu(LevelMenu* menu);
    LevelMenu* getLevelMenu() const { return m_levelMenu; }

    void setGameScene(GameScene* scene);
    GameScene* getGameScene() const { return m_gameScene; }

private:
    void render();
    void update(float dt);

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture* m_targetTexture = nullptr;

    Mix_Music* m_music = nullptr;

    bool m_running = false;
    int m_currentScene = 0;

    LevelMenu* m_levelMenu = nullptr;
    GameScene* m_gameScene = nullptr;

    Uint64 m_lastTicks = 0;
};