#include <SFML/Graphics.hpp>
#include <iostream>

#include "application_context.h"
#include "constants.h"
#include "managers/game_state_manager.h"
#include "managers/page_state_manager.h"
#include "games/tetris/tetris.h"

ApplicationContext appCtx;

bool lostFocus = false;

// METHODS:
void manageEvent();

int main(int argc, char* argv[]) {
    Constants::initPaths(argc, argv);
    Constants::loadFont();
    auto& window = appCtx.getWindow();
    auto& gameManager = appCtx.getGameManager();
    auto& pageManager = appCtx.getPageManager();

    window.setFramerateLimit(60);



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
            gameManager.update(sf::Mouse::getPosition(appCtx.getWindow()));
            gameManager.draw();
            windowSprite = gameManager.getSprite();
        }


        // === Main Window Drawing ===
        window.clear(sf::Color(30, 30, 40));
        windowSprite.setPosition(Constants::WINDOW_WIDTH/2.f, Constants::WINDOW_HEIGHT/2.f);
        window.draw(windowSprite);

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
            if (!appCtx.getGameManager().playing() && event.type == sf::Event::MouseButtonPressed) {
                if (appCtx.getGameManager().handleMenuClick(sf::Mouse::getPosition(appCtx.getWindow()))) {
                    appCtx.getPageManager().switchState(PageState::MAIN_MENU);
                }
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