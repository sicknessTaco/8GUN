// main.cpp
#include <iostream>
#include <SDL2/SDL.h>
#include "Engine.hpp"
#include "LevelMenu.hpp"
#include "GameScene.hpp"

int main(int argc, char* argv[]) {
    Engine engine;

    if (!engine.init()) {
        std::cerr << "❌ No se pudo inicializar el motor." << std::endl;
        return -1;
    }

    LevelMenu menu(&engine);
    engine.setLevelMenu(&menu);

    std::cout << "✅ 8GUN Engine iniciado. Bienvenido al menú de zonas." << std::endl;

    SDL_Event event;
    Uint64 lastTicks = SDL_GetTicks();

    while (engine.isRunning()) {
        Uint64 currentTicks = SDL_GetTicks();
        float dt = (currentTicks - lastTicks) / 1000.0f;
        lastTicks = currentTicks;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                engine.quit();
            }
            if (event.type == SDL_KEYDOWN) {
                menu.handleInput(event.key.keysym.sym);
            }
        }

        if (engine.getScene() == 0) {
            menu.update(dt);
            menu.render();

            // ✅ DEBUG: Verifica si se solicitó iniciar juego
            if (menu.shouldStartGame()) {
                std::cout << "🎮 ¡Solicitando iniciar juego!" << std::endl;
                int zone = menu.getSelectedZone();
                int song = menu.getSelectedSong();
                std::cout << "   → Zona: " << zone << ", Canción: " << song << std::endl;

                GameScene* game = new GameScene(&engine, zone, song);
                engine.setGameScene(game);
                engine.setScene(1); // Cambia a escena de juego
                menu.resetStartGame(); // Reinicia flag
                std::cout << "🎮 ¡Juego iniciado!" << std::endl;
            }
        }

        if (engine.getScene() == 1) {
            GameScene* game = engine.getGameScene();
            if (game) {
                std::cout << "🎮 Actualizando juego..." << std::endl; // ✅ DEBUG
                game->update(dt);
                game->render();
                if (game->isComplete() || game->isDead()) {
                    std::cout << "🎮 Juego terminado, volviendo al menú" << std::endl; // ✅ DEBUG
                    delete game;
                    engine.setGameScene(nullptr);
                    engine.setScene(0); // Vuelve al menú
                }
            }
        }

        SDL_Delay(16);
    }

    std::cout << "👋 Gracias por jugar." << std::endl;
    return 0;
}