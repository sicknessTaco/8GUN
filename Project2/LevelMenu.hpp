// LevelMenu.hpp
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class Engine; // ✅ Solo declaración, no definición

class LevelMenu {
public:
    LevelMenu(Engine* engine);
    ~LevelMenu();

    void update(float dt); // ✅ Usa dt, no deltaTime
    void render();
    void handleInput(SDL_Keycode key);

    bool isExiting() const { return m_exitRequested; }
    bool isSelectingSong() const { return m_selectingSong; }
    int getSelectedZone() const { return m_selectedZone; }
    int getSelectedSong() const { return m_selectedSong; }

    bool isLevelUnlocked(int zone, int song) const;
    bool isSongCompleted(int zone, int song) const;
    void completeSong(int zone, int song);
    bool shouldStartGame() const { return m_shouldStartGame; }
    void resetStartGame() { m_shouldStartGame = false; }

private:
    // ... otros campos ...

    // ✅ AÑADE ESTE CAMPO
    bool m_shouldStartGame = false;

private:
    Engine* m_engine;
    TTF_Font* m_font;

    int m_selectedZone = 0;
    int m_selectedSong = 0;
    bool m_exitRequested = false;
    bool m_selectingSong = false;

    enum { NUM_ZONES = 4 };
    enum { NUM_SONGS = 4 };

    struct Zone {
        const char* name;
        int zoneIndex;
        SDL_Color color;
    };
    static const Zone ZONES[NUM_ZONES];

    bool m_completedSongs[4][4] = { false };

    void renderText(const char* text, int x, int y, SDL_Color color);
};