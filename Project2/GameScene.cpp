// GameScene.cpp
#include "GameScene.hpp"
#include "Engine.hpp"
#include "LevelMenu.hpp"
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <string>
#include <algorithm>

// Constructor de Enemy
Enemy::Enemy(float x, float y, int type)
    : x(x), y(y), type(type), shootTimer(0.0f), shootInterval(0.5f) {
}

GameScene::GameScene(Engine* engine, int zone, int song)
    : m_engine(engine), m_zone(zone), m_song(song), m_levelMenu(engine->getLevelMenu()) {

    m_playerX = m_engine->getLogicalWidth() / 2.0f;
    m_playerY = m_engine->getLogicalHeight() / 2.0f;

    loadAssets();

    spawnEnemies();

    // Cargar música: Zona 1, Canción 1 → "1/1.mp3", etc.
    std::string path = "assets/music/" + std::to_string(m_zone + 1) + "/" + std::to_string(m_song + 1) + ".mp3";
    Mix_Music* music = Mix_LoadMUS(path.c_str());
    if (music) {
        Mix_PlayMusic(music, -1);
        m_musicPlaying = true;
        std::cout << "🎵 Reproduciendo: " << path << std::endl;
    }
    else {
        std::cerr << "❌ No se pudo cargar música: " << Mix_GetError() << std::endl;
    }

    m_beatInterval = 0.5f; // 120 BPM
}

GameScene::~GameScene() {
    if (m_levelText) SDL_DestroyTexture(m_levelText);
}

void GameScene::loadAssets() {
    TTF_Font* font = TTF_OpenFont("assets/fonts/pkmn.ttf", 48); // ✅ Usa la misma fuente que LevelMenu
    if (!font) {
        std::cerr << "❌ No se pudo cargar fuente: " << TTF_GetError() << std::endl;
        return;
    }

    char levelText[64];
    sprintf_s(levelText, sizeof(levelText), "ZONA %d - CANCION %d", m_zone + 1, m_song + 1);

    SDL_Color color;
    if (m_zone == 0) color = { 0, 200, 80, 255 }; // Verde
    else if (m_zone == 1) color = { 255, 255, 0, 255 }; // Amarillo
    else if (m_zone == 2) color = { 0, 120, 255, 255 }; // Azul
    else color = { 255, 0, 0, 255 }; // Rojo

    SDL_Surface* surface = TTF_RenderText_Solid(font, levelText, color);
    if (surface) {
        m_levelText = SDL_CreateTextureFromSurface(m_engine->getRenderer(), surface);
        SDL_FreeSurface(surface);
    }
    TTF_CloseFont(font);
}

void GameScene::spawnEnemies() {
    m_enemies.clear();
    std::cout << "🎮 SpawnEnemies llamado: Zona=" << m_zone << ", Canción=" << m_song << std::endl;

    int numEnemies = 4;
    if (m_zone == 0 && m_song == 1) numEnemies = 6;
    if (m_zone == 0 && m_song == 2) numEnemies = 8;
    if (m_zone == 1 && m_song == 0) numEnemies = 6;
    if (m_zone == 1 && m_song == 1) numEnemies = 6;
    if (m_zone == 1 && m_song == 2) numEnemies = 8;

    std::cout << "🎮 Número de enemigos: " << numEnemies << std::endl;

    float spacing = m_engine->getLogicalWidth() / (numEnemies + 1);

    for (int i = 0; i < numEnemies; ++i) {
        float x = spacing * (i + 1);
        float y = 120;

        int type = 0;

        if (m_zone == 0) {
            type = 0;
        }
        else if (m_zone == 1) {
            if (m_song == 0) type = (i % 2 == 0) ? 0 : 1;
            else if (m_song == 1) type = 1;
            else if (m_song == 2) type = 1;
        }
        else if (m_zone == 2) {
            if (m_song == 0) type = (i % 3 == 0) ? 0 : (i % 3 == 1) ? 1 : 2;
            else if (m_song == 1) type = 2;
            else if (m_song == 2) type = 2;
        }
        else if (m_zone == 3) {
            type = (m_song == 0) ? (i % 4 == 0) ? 0 : (i % 4 == 1) ? 1 : (i % 4 == 2) ? 2 : 3 :
                (m_song == 1) ? (i % 3 == 0) ? 1 : (i % 3 == 1) ? 2 : 3 :
                (m_song == 2) ? 3 : 3;
        }

        m_enemies.emplace_back(x, y, type);
        std::cout << "🎮 Enemigo creado: Pos=(" << x << "," << y << "), Tipo=" << type << std::endl;
    }

    std::cout << "🎮 Total enemigos creados: " << m_enemies.size() << std::endl;
}
void GameScene::spawnBullets() {
    if (!m_isOnBeat) return;

    for (auto& enemy : m_enemies) {
        if (enemy.type == 0) {
            for (int i = 0; i < 5; ++i) {
                float angle = (i * 2.0f * M_PI) / 5.0f;
                float speed = 250.0f;
                float vx = cosf(angle) * speed;
                float vy = sinf(angle) * speed;
                if (m_bullets.size() < MAX_BULLETS) {
                    m_bullets.emplace_back(enemy.x, enemy.y, vx, vy);
                }
            }
        }

        if (enemy.type == 1) {
            for (int i = 0; i < 3; ++i) {
                float angle = (i * 2.0f * M_PI) / 3.0f + (m_currentMusicTime * 0.5f);
                float speed = 300.0f;
                float vx = cosf(angle) * speed;
                float vy = sinf(angle) * speed;
                if (m_bullets.size() < MAX_BULLETS) {
                    m_bullets.emplace_back(enemy.x, enemy.y, vx, vy);
                }
            }
        }

        if (enemy.type == 2) {
            for (int i = 0; i < 8; ++i) {
                float angle = (i * 2.0f * M_PI) / 8.0f + (m_currentMusicTime * 0.7f);
                float speed = 180.0f + (m_currentMusicTime * 10);
                float vx = cosf(angle) * speed;
                float vy = sinf(angle) * speed;
                if (m_bullets.size() < MAX_BULLETS) {
                    m_bullets.emplace_back(enemy.x, enemy.y, vx, vy);
                }
            }
        }

        if (enemy.type == 3) {
            for (int i = 0; i < 12; ++i) {
                float angle = (i * 2.0f * M_PI) / 12.0f + (m_currentMusicTime * 1.0f);
                float speed = 200.0f + (m_currentMusicTime * 5);
                float vx = cosf(angle) * speed;
                float vy = sinf(angle) * speed;
                if (m_bullets.size() < MAX_BULLETS) {
                    m_bullets.emplace_back(enemy.x, enemy.y, vx, vy);
                }
            }
        }
    }
}

void GameScene::updateBullets(float dt) {
    for (auto& bullet : m_bullets) {
        if (!bullet.active) continue;
        bullet.x += bullet.vx * dt; // ✅ dt aquí
        bullet.y += bullet.vy * dt; // ✅ dt aquí

        // Eliminar balas fuera de pantalla
        if (bullet.x < -10 || bullet.x > m_engine->getLogicalWidth() + 10 ||
            bullet.y < -10 || bullet.y > m_engine->getLogicalHeight() + 10) {
            bullet.active = false;
        }
    }

    // Limpiar balas inactivas
    m_bullets.erase(
        std::remove_if(m_bullets.begin(), m_bullets.end(),
            [](const Bullet& b) { return !b.active; }),
        m_bullets.end()
    );
}

void GameScene::checkCollisions() {
    if (m_playerDead || m_invincibilityTimer > 0) return;

    for (auto& bullet : m_bullets) {
        if (!bullet.active) continue;

        float dx = bullet.x - m_playerX;
        float dy = bullet.y - m_playerY;
        float distance = sqrtf(dx * dx + dy * dy);

        if (distance < 32.0f) {
            m_playerLives--;
            m_invincibilityTimer = 1.5f;
            bullet.active = false;

            if (m_playerLives <= 0) {
                m_playerDead = true;
                std::cout << "💀 Jugador muerto." << std::endl;
            }
            break;
        }
    }
}

void GameScene::checkBeat() {
    if (!m_musicPlaying) return;

    m_currentMusicTime = Mix_GetMusicPosition(m_engine->getMusic()) / 1000.0f;

    if (m_currentMusicTime - m_lastBeatTime >= m_beatInterval) {
        m_isOnBeat = true;
        m_lastBeatTime = m_currentMusicTime;
        spawnBullets();
    }
    else {
        m_isOnBeat = false;
    }
}

void GameScene::updateInvincibility(float dt) {
    if (m_invincibilityTimer > 0) {
        m_invincibilityTimer -= dt; // ✅ dt aquí
        if (m_invincibilityTimer < 0) m_invincibilityTimer = 0;
    }
}

void GameScene::update(float dt) { // ✅ dt aquí
    if (m_complete || m_playerDead) return;

    handleInput(dt); // ✅ Pasa dt para movimiento
    updateBullets(dt);       // ✅ dt aquí
    checkCollisions();       // ✅ NO usa dt
    updateInvincibility(dt); // ✅ dt aquí
    checkBeat();             // ✅ NO usa dt

    // Si la canción termina (60 segundos), completa
    if (m_currentMusicTime > 60.0f) {
        m_complete = true;
        std::cout << "🎉 Zona " << (m_zone + 1) << ", Canción " << (m_song + 1) << " completada!" << std::endl;
        completeSong();
    }
}

void GameScene::handleInput(float dt) {
    const Uint8* state = SDL_GetKeyboardState(nullptr);

    float dx = 0, dy = 0;
    if (state[SDL_SCANCODE_W] || state[SDL_SCANCODE_UP]) dy -= 1;
    if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_DOWN]) dy += 1;
    if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT]) dx -= 1;
    if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT]) dx += 1;

    if (dx != 0 || dy != 0) {
        float len = sqrtf(dx * dx + dy * dy);
        if (len > 0) {
            dx /= len;
            dy /= len;
        }
        m_playerX += dx * m_playerSpeed * dt;
        m_playerY += dy * m_playerSpeed * dt;
    }

    // ✅ Disparo del jugador
    if (state[SDL_SCANCODE_SPACE]) {
        m_playerShotTimer += dt;
        if (m_playerShotTimer >= m_playerShotInterval) {
            shootPlayer();
            m_playerShotTimer = 0.0f;
        }
    }

    // Límites del jugador
    m_playerX = fmaxf(32, fminf(m_engine->getLogicalWidth() - 32, m_playerX));
    m_playerY = fmaxf(32, fminf(m_engine->getLogicalHeight() - 32, m_playerY));
}

void GameScene::shootPlayer() {
    // Disparar hacia arriba
    float bulletSpeed = 500.0f;
    float bulletX = m_playerX;
    float bulletY = m_playerY - 32; // Disparar desde arriba del jugador
    float bulletVX = 0.0f;
    float bulletVY = -bulletSpeed;

    if (m_bullets.size() < MAX_BULLETS) {
        m_bullets.emplace_back(bulletX, bulletY, bulletVX, bulletVY);
        std::cout << "🔫 Jugador disparó!" << std::endl;
    }
}
void GameScene::render() {
    auto renderer = m_engine->getRenderer();
    SDL_SetRenderTarget(renderer, m_engine->getTargetTexture());
    SDL_SetRenderDrawColor(renderer, 20, 10, 30, 255);
    SDL_RenderClear(renderer);

    // Jugador
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    if (m_invincibilityTimer > 0 && static_cast<int>(m_invincibilityTimer * 10) % 2 == 0) {
        // Parpadeo
    }
    else {
        SDL_Rect playerRect = { static_cast<int>(m_playerX - 32), static_cast<int>(m_playerY - 32), 64, 64 };
        SDL_RenderFillRect(renderer, &playerRect);
    }

    // Enemigos
    for (const auto& enemy : m_enemies) {
        SDL_Color color = (enemy.type == 0) ? SDL_Color{ 0, 200, 80, 255 } :
            (enemy.type == 1) ? SDL_Color{ 255, 255, 0, 255 } :
            (enemy.type == 2) ? SDL_Color{ 0, 120, 255, 255 } :
            SDL_Color{ 255, 0, 0, 255 };
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_Rect enemyRect = { static_cast<int>(enemy.x - 16), static_cast<int>(enemy.y - 16), 32, 32 };
        SDL_RenderFillRect(renderer, &enemyRect);
    }

    // Balas
    for (const auto& bullet : m_bullets) {
        if (bullet.active) {
            SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
            SDL_Rect bulletRect = { static_cast<int>(bullet.x - 4), static_cast<int>(bullet.y - 4), 8, 8 };
            SDL_RenderFillRect(renderer, &bulletRect);
        }
    }

    // Texto de nivel
    if (m_levelText) {
        SDL_Rect textRect;
        SDL_QueryTexture(m_levelText, nullptr, nullptr, &textRect.w, &textRect.h);
        textRect.x = m_engine->getLogicalWidth() / 2 - textRect.w / 2;
        textRect.y = 20;
        SDL_RenderCopy(renderer, m_levelText, nullptr, &textRect);
    }

    // Vidas
    for (int i = 0; i < m_playerLives; ++i) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect lifeRect = { 20 + i * 25, m_engine->getLogicalHeight() - 40, 20, 20 };
        SDL_RenderFillRect(renderer, &lifeRect);
    }

    // Estado
    if (m_playerDead) {
        renderText("PERDISTE", m_engine->getLogicalWidth() / 2, m_engine->getLogicalHeight() / 2, { 255, 0, 0, 255 });
    }
    else if (m_complete) {
        renderText("¡GANASTE!", m_engine->getLogicalWidth() / 2, m_engine->getLogicalHeight() / 2, { 0, 255, 255, 255 });
    }

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopy(renderer, m_engine->getTargetTexture(), nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void GameScene::renderText(const char* text, int x, int y, SDL_Color color) {
    TTF_Font* font = TTF_OpenFont("assets/fonts/pkmn.ttf", 64); // ✅ Usa la misma fuente que LevelMenu
    if (!font) {
        std::cerr << "❌ No se pudo cargar fuente: " << TTF_GetError() << std::endl;
        return;
    }

    // ✅ Verifica que el texto no esté vacío
    if (!text || strlen(text) == 0) {
        std::cerr << "⚠️ Texto vacío en renderText" << std::endl;
        TTF_CloseFont(font);
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        std::cerr << "❌ RenderText failed: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    // ✅ Verifica que el ancho no sea cero
    if (surface->w == 0) {
        std::cerr << "⚠️ Texto sin ancho: '" << text << "'" << std::endl;
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
        std::cerr << "❌ No se pudo crear textura: " << SDL_GetError() << std::endl;
    }

    TTF_CloseFont(font);
}

void GameScene::completeSong() {
    if (m_levelMenu) {
        m_levelMenu->completeSong(m_zone, m_song);
        std::cout << "🎉 Zona " << (m_zone + 1) << ", Canción " << (m_song + 1) << " completada y desbloqueada!" << std::endl;
    }
}