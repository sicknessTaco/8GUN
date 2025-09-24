// Engine.cpp
#include "Engine.hpp"
#include <iostream>

Engine::~Engine() {
    quit();
}

bool Engine::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "❌ SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
        std::cerr << "❌ Mix_OpenAudio failed: " << Mix_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "❌ TTF_Init failed: " << TTF_GetError() << std::endl;
        return false;
    }
    else {
        std::cout << "✅ SDL_ttf inicializado correctamente" << std::endl;
    }

    // 🚀 NUEVA RESOLUCIÓN: 1280x720
    int winW = LOGICAL_WIDTH;
    int winH = LOGICAL_HEIGHT;

    m_window = SDL_CreateWindow("RHYTHM GUNGEON", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        winW, winH, SDL_WINDOW_SHOWN);
    if (!m_window) {
        std::cerr << "❌ Window failed: " << SDL_GetError() << std::endl;
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer) {
        std::cerr << "❌ Renderer failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // ✅ ¡IMPORTANTE! Activar escala pixel-perfect (nearest neighbor)
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

    // ✅ ¡NO usamos textura lógica! Renderizamos directamente.
    // Pero si quieres usar textura lógica (para futura escala), puedes mantenerla.
    // Por ahora, renderizamos directamente a la pantalla.
    // (No necesitamos m_targetTexture si renderizamos directamente)
    // Pero lo mantenemos por compatibilidad con LevelMenu.
    m_targetTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        LOGICAL_WIDTH, LOGICAL_HEIGHT);
    if (!m_targetTexture) {
        std::cerr << "❌ Target texture failed: " << SDL_GetError() << std::endl;
        return false;
    }

    m_running = true;
    m_lastTicks = SDL_GetTicks();

    std::cout << "✅ 8GUN Engine iniciado (1280x720 HD, pixel art)" << std::endl;
    return true;
}

void Engine::quit() {
    if (m_music) Mix_FreeMusic(m_music);
    if (m_targetTexture) SDL_DestroyTexture(m_targetTexture);
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window) SDL_DestroyWindow(m_window);
    TTF_Quit();
    Mix_CloseAudio();
    SDL_Quit();
    m_running = false;
}

void Engine::render() {
    // ✅ Renderizamos directamente la textura lógica a la pantalla
    // (Esto permite que si en el futuro queremos escalar, sea fácil)
    SDL_SetRenderTarget(m_renderer, nullptr);
    SDL_RenderCopy(m_renderer, m_targetTexture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}

void Engine::update(float deltaTime) {}

void Engine::setScene(int sceneId) {
    m_currentScene = sceneId;
    std::cout << "🔄 Cambiando a escena: " << sceneId << std::endl;
}

// ✅ AÑADE ESTAS DOS FUNCIONES
void Engine::setLevelMenu(LevelMenu* menu) {
    m_levelMenu = menu;
}

void Engine::setGameScene(GameScene* scene) {
    m_gameScene = scene;
}