#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>

#include "application_context.h"
#include "constants.h"
#include "managers/game_state_manager.h"
#include "managers/page_state_manager.h"
#include "components/pause_display.h"
#include "games/tetris/tetris.h"
ApplicationContext appCtx;
PauseDisplay pauseDisplay;
sf::RectangleShape overlay(sf::Vector2f(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT));

bool lostFocus = false;

// METHODS:
void drawFrame(const sf::Sprite&);
void manageEvent();

int main() {
    std::cout << "Tetris starting..." << std::endl;

    auto& window = appCtx.getWindow();
    auto& gameManager = appCtx.getGameManager();
    auto& pageManager = appCtx.getPageManager();

    window.setFramerateLimit(60);

    pauseDisplay.centerOn(Constants::WINDOW_WIDTH/2.f, Constants::WINDOW_HEIGHT/2.f);

    overlay.setFillColor(sf::Color(0, 0, 0, 180)); // Black with alpha (0-255)
    overlay.setPosition(0, 0);

    gameManager.buildDefaultGame();
    while (window.isOpen()) {
        if (!window.hasFocus()) {
            lostFocus = true;
            appCtx.getGameManager().pauseGame();
        } else if (lostFocus) {
            appCtx.getGameManager().startGame();
            lostFocus = false;
        }

        manageEvent();
        sf::Sprite windowSprite;
        if (!pageManager.inGame()) {
            pageManager.update(sf::Mouse::getPosition(window));
            pageManager.draw();
            windowSprite = pageManager.getSprite();
        } else {
            gameManager.update();
            gameManager.draw();
            windowSprite = gameManager.getSprite();
        }


        // === Main Window Drawing ===
        window.clear(sf::Color(30, 30, 40));
        windowSprite.setPosition(Constants::WINDOW_WIDTH/2.f, Constants::WINDOW_HEIGHT/2.f);
        window.draw(windowSprite);

        if (gameManager.paused() && pageManager.inGame()) {
            window.draw(overlay);

            pauseDisplay.update(sf::Mouse::getPosition(window));
            pauseDisplay.draw(); // Renders to its own texture
            window.draw(pauseDisplay.getSprite()); // Draw the menu sprite
        }
        window.display();
    }

    return 0;
}

void manageEvent() {
    sf::Event event{};
    while (appCtx.getWindow().pollEvent(event)) {
        if (event.type == sf::Event::Closed){
            appCtx.getWindow().close();
        } else if (appCtx.getPageManager().inGame()) {
            if (appCtx.getGameManager().paused() && event.type == sf::Event::MouseButtonPressed) {
                pauseDisplay.handleClick(sf::Mouse::getPosition(appCtx.getWindow()));
            } else if (event.type == sf::Event::KeyPressed) {
                appCtx.getGameManager().handleEvent(event);
            }
        } else if (appCtx.getPageManager().settings()) {
            if (event.type == sf::Event::KeyPressed) {
                if (auto* settings = dynamic_cast<Settings*>(&appCtx.getPageManager().getCurrentPage())) {
                    settings->handleKeyPress(event.key.code);
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                appCtx.getPageManager().handleClick(sf::Mouse::getPosition(appCtx.getWindow()));
            }
        } else if (event.type == sf::Event::MouseButtonPressed) {
            appCtx.getPageManager().handleClick(sf::Mouse::getPosition(appCtx.getWindow()));
        }
    }
}