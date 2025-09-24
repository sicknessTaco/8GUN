// LevelMenu.cpp
#include "LevelMenu.hpp"
#include "Engine.hpp"
#include <iostream>
#include <sstream>

const LevelMenu::Zone LevelMenu::ZONES[NUM_ZONES] = {
    {"ZONA 1", 1, {0, 200, 80, 255}},      // Verde
    {"ZONA 2", 2, {255, 255, 0, 255}},     // Amarillo
    {"ZONA 3", 3, {0, 120, 255, 255}},     // Azul
    {"ZONA 4", 4, {255, 0, 0, 255}}        // Rojo
};

LevelMenu::LevelMenu(Engine* engine)
    : m_engine(engine), m_font(nullptr) {

    m_font = TTF_OpenFont("assets/fonts/pkmn.ttf", 32);
    if (!m_font) {
        std::cerr << "❌ Failed to load font: " << TTF_GetError() << std::endl;
    }

    std::string path = "assets/music/mainmenu/1.mp3";
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (music) {
        Mix_PlayMusic(music, -1);
        std::cout << "🎵 Música de menú cargada: " << path << std::endl;
    }
    else {
        std::cout << "⚠️ No se pudo cargar música de menú: " << Mix_GetError() << std::endl;
    }
}

LevelMenu::~LevelMenu() {
    if (m_font) TTF_CloseFont(m_font);
}

void LevelMenu::update(float deltaTime) {}

void LevelMenu::render() {
    auto renderer = m_engine->getRenderer();
    SDL_SetRenderTarget(renderer, m_engine->getTargetTexture());
    SDL_SetRenderDrawColor(renderer, 20, 10, 30, 255);
    SDL_RenderClear(renderer);

    renderText("RHYTHM GUNGEON", m_engine->getLogicalWidth() / 2, m_engine->getLogicalHeight() / 6, { 255, 255, 100, 255 });

    if (!m_selectingSong) {
        // Menú de zonas
        for (int i = 0; i < NUM_ZONES; ++i) {
            int y = m_engine->getLogicalHeight() / 3 + i * 48;
            SDL_Color color;

            if (isLevelUnlocked(i, 0)) { // Desbloqueado si completaste jefe de zona anterior
                color = ZONES[i].color;
            }
            else {
                color = { 100, 100, 100, 255 }; // Gris bloqueado
            }

            renderText(ZONES[i].name, m_engine->getLogicalWidth() / 2, y, color);

            // 🔒 Candado si está bloqueado
            if (!isLevelUnlocked(i, 0)) {
                renderText("🔒", m_engine->getLogicalWidth() / 2 + 120, y, { 255, 255, 255, 255 });
            }
        }

        // Cursor
        if (m_selectedZone < NUM_ZONES && isLevelUnlocked(m_selectedZone, 0)) {
            renderText(">", m_engine->getLogicalWidth() / 2 - 160,
                m_engine->getLogicalHeight() / 3 + m_selectedZone * 48, { 255, 255, 255, 255 });
        }
        else {
            renderText("🔒", m_engine->getLogicalWidth() / 2 - 160,
                m_engine->getLogicalHeight() / 3 + m_selectedZone * 48, { 255, 0, 0, 255 });
        }

        renderText("ESPACIO: SELECCIONAR | ESC: SALIR", m_engine->getLogicalWidth() / 2,
            m_engine->getLogicalHeight() - 40, { 180, 180, 180, 255 });
    }
    else {
        // Menú de canciones dentro de la zona seleccionada
        renderText("SELECCIONA CANCION", m_engine->getLogicalWidth() / 2, m_engine->getLogicalHeight() / 5, { 255, 255, 255, 255 });

        for (int i = 0; i < NUM_SONGS; ++i) {
            int y = m_engine->getLogicalHeight() / 3 + i * 48;
            SDL_Color color;

            // Si no está desbloqueada la canción
            if (!isLevelUnlocked(m_selectedZone, i)) {
                color = { 100, 100, 100, 255 }; // Gris
            }
            else {
                // Si ya se completó
                if (isSongCompleted(m_selectedZone, i)) {
                    color = { 0, 255, 255, 255 }; // Cian
                }
                else {
                    // Si es jefe (canción 4)
                    if (i == 3) {
                        color = ZONES[m_selectedZone].color;
                    }
                    else {
                        color = { 255, 255, 255, 255 }; // Blanco
                    }
                }
            }

            // ✅ CORREGIDO: Guarda en std::string temporal
            std::string temp = (i == 3) ? "JEFE!" : "CANCION " + std::to_string(i + 1);
            renderText(temp.c_str(), m_engine->getLogicalWidth() / 2, y, color);
        }

        // Cursor
        if (isLevelUnlocked(m_selectedZone, m_selectedSong)) {
            renderText(">", m_engine->getLogicalWidth() / 2 - 160,
                m_engine->getLogicalHeight() / 3 + m_selectedSong * 48, { 255, 255, 255, 255 });
        }
        else {
            renderText("🔒", m_engine->getLogicalWidth() / 2 - 160,
                m_engine->getLogicalHeight() / 3 + m_selectedSong * 48, { 255, 0, 0, 255 });
        }

        renderText("ESPACIO: INICIAR | ESC: ATRAS", m_engine->getLogicalWidth() / 2,
            m_engine->getLogicalHeight() - 40, { 180, 180, 180, 255 });
    }

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, m_engine->getTargetTexture(), nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void LevelMenu::handleInput(SDL_Keycode key) {
    if (m_selectingSong) {
        switch (key) {
        case SDLK_UP: case SDLK_w:
            m_selectedSong = (m_selectedSong - 1 + NUM_SONGS) % NUM_SONGS;
            break;
        case SDLK_DOWN: case SDLK_s:
            m_selectedSong = (m_selectedSong + 1) % NUM_SONGS;
            break;
        case SDLK_SPACE: case SDLK_RETURN:
            if (isLevelUnlocked(m_selectedZone, m_selectedSong)) {
                std::cout << "▶️ Iniciando Zona " << (m_selectedZone + 1) << ", Canción " << (m_selectedSong + 1) << std::endl;
            }
            else {
                std::cout << "🔒 Canción bloqueada. Completa la anterior." << std::endl;
            }
            break;
        case SDLK_ESCAPE:
            m_selectingSong = false;
            break;
        }
    }
    else {
        switch (key) {
        case SDLK_UP: case SDLK_w:
            m_selectedZone = (m_selectedZone - 1 + NUM_ZONES) % NUM_ZONES;
            break;
        case SDLK_DOWN: case SDLK_s:
            m_selectedZone = (m_selectedZone + 1) % NUM_ZONES;
            break;
        case SDLK_SPACE: case SDLK_RETURN:
            if (isLevelUnlocked(m_selectedZone, m_selectedSong)) {
                std::cout << "▶️ Iniciando Zona " << (m_selectedZone + 1) << ", Canción " << (m_selectedSong + 1) << std::endl;
                m_shouldStartGame = true; // ✅ Marca para iniciar juego
            }
            else {
                std::cout << "🔒 Canción bloqueada. Completa la anterior." << std::endl;
            }
            break;
        case SDLK_ESCAPE:
            m_exitRequested = true;
            break;
        }
    }
}

// ✅ ¿Está desbloqueada la canción?
bool LevelMenu::isLevelUnlocked(int zone, int song) const {
    // Zona 1, Canción 1 siempre desbloqueada
    if (zone == 0 && song == 0) return true;

    // Si es jefe (canción 4), desbloquea solo si completaste canción 3 de la misma zona
    if (song == 3) {
        return isSongCompleted(zone, 2); // Necesitas completar canción 3
    }

    // Si es canción 1 de zona > 1, desbloquea solo si completaste jefe de zona anterior
    if (song == 0 && zone > 0) {
        return isSongCompleted(zone - 1, 3); // Necesitas completar jefe de zona anterior
    }

    // Para canciones 1,2,3 en zona > 0: desbloqueadas si la zona está desbloqueada
    if (song < 3 && zone > 0) {
        return isLevelUnlocked(zone, 0); // Si la zona está desbloqueada, todas las canciones anteriores también
    }

    return false;
}

// ✅ ¿Se completó esta canción?
bool LevelMenu::isSongCompleted(int zone, int song) const {
    if (zone < 0 || zone >= NUM_ZONES) return false;
    if (song < 0 || song >= NUM_SONGS) return false;
    return m_completedSongs[zone][song];
}

// ✅ Marca una canción como completada
void LevelMenu::completeSong(int zone, int song) {
    if (zone < 0 || zone >= NUM_ZONES) return;
    if (song < 0 || song >= NUM_SONGS) return;
    m_completedSongs[zone][song] = true;
    std::cout << "🎉 Completado: Zona " << (zone + 1) << ", Canción " << (song + 1) << std::endl;
}

void LevelMenu::renderText(const char* text, int x, int y, SDL_Color color) {
    TTF_Font* font = TTF_OpenFont("assets/fonts/pkmn.ttf", 32);
    if (!font) {
        std::cerr << "❌ LevelMenu: No se pudo cargar fuente: " << TTF_GetError() << std::endl;
        return;
    }

    // ✅ Verifica que el texto no esté vacío
    if (!text || strlen(text) == 0) {
        std::cerr << "⚠️ LevelMenu: Texto vacío en renderText" << std::endl;
        TTF_CloseFont(font);
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        std::cerr << "❌ LevelMenu: RenderText failed: " << TTF_GetError() << std::endl;
        std::cerr << "   Texto: '" << text << "'" << std::endl;
        TTF_CloseFont(font);
        return;
    }

    // ✅ Verifica que el ancho no sea cero
    if (surface->w == 0) {
        std::cerr << "⚠️ LevelMenu: Texto sin ancho: '" << text << "'" << std::endl;
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_engine->getRenderer(), surface);
    SDL_FreeSurface(surface);

    if (texture) {
        SDL_Rect rect;
        SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
        rect.x = x - rect.w / 2;
        rect.y = y - rect.h / 2;
        SDL_RenderCopy(m_engine->getRenderer(), texture, nullptr, &rect);
        SDL_DestroyTexture(texture);
    }
    else {
        std::cerr << "❌ LevelMenu: No se pudo crear textura: " << SDL_GetError() << std::endl;
    }

    TTF_CloseFont(font);
}