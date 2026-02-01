//
// Created by Matthew on 1/26/2026.
//

#ifndef TETRIS_APPLICATIONCONTEXT_H
#define TETRIS_APPLICATIONCONTEXT_H

#include <SFML/Graphics.hpp>

#include "managers/game_state_manager.h"
#include "managers/page_state_manager.h"
#include "managers/input_manager.h"

class ApplicationContext {
    inline static ApplicationContext* instance = nullptr;

    sf::RenderWindow window;
    InputManager inputManager;
    GameStateManager gameManager;
    PageStateManager pageManager;

public:
    ApplicationContext()
        : window(sf::VideoMode(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT), "Tetris"),
          inputManager(),
          gameManager(),
          pageManager() {
        instance = this;
        window.setFramerateLimit(60);
    }

    static ApplicationContext& get() { return *instance; }

    sf::RenderWindow& getWindow() { return window; }
    GameStateManager& getGameManager() { return gameManager; }
    PageStateManager& getPageManager() { return pageManager; }
    InputManager& getInputManager() { return inputManager; }
};
#endif //TETRIS_APPLICATIONCONTEXT_H