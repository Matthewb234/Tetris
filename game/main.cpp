#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>

#include "constants.h"
#include "game_state_manager.h"
#include "page_state_manager.h"
#include "components/pause_display.h"
#include "games/tetris/tetris.h"
sf::RenderWindow window(sf::VideoMode(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT), "Tetris");
GameStateManager gameManager(&window);
PageStateManager pageManager(&window);
PauseDisplay pauseDisplay;

sf::RectangleShape overlay(sf::Vector2f(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT));

// METHODS:
void drawFrame(const sf::Sprite&);
void manageEvent();

int main() {
    std::cout << "Tetris starting..." << std::endl;
    srand(time(nullptr));

    window.setFramerateLimit(60);

    pauseDisplay.centerOn(Constants::WINDOW_WIDTH/2.f, Constants::WINDOW_HEIGHT/2.f);

    overlay.setFillColor(sf::Color(0, 0, 0, 180)); // Black with alpha (0-255)
    overlay.setPosition(0, 0);

    while (window.isOpen()) {
        manageEvent();
        sf::Sprite windowSprite;

        if (!pageManager.inGame()) {
            pageManager.update(sf::Mouse::getPosition(window));
            pageManager.draw();
            windowSprite = pageManager.getSprite();
        } else {
            gameManager.update();
            gameManager.draw();  // Draws to internal texture
            windowSprite = gameManager.getSprite();
        }


        // === Main Window Drawing ===
        window.clear(sf::Color(30, 30, 40)); // Dark backdrop

        // Position the game sprite (the "inner window")
        windowSprite.setPosition(Constants::WINDOW_WIDTH/2.f, Constants::WINDOW_HEIGHT/2.f); // Position within the large window

        // Optional: Add a border/frame
        // drawFrame(windowSprite);

        // Draw the game
        window.draw(windowSprite);

        // Draw pause menu on top if needed
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
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed){
            window.close();
        } else if (pageManager.inGame()) {
            if (gameManager.paused() && event.type == sf::Event::MouseButtonPressed) {
                int clicked = pauseDisplay.handleClick(sf::Mouse::getPosition(window));
                if (clicked == 0) gameManager.startGame();
                else if (clicked == 1) gameManager.restartGame();
                else if (clicked == 2) {
                    gameManager.pauseGame();
                    pageManager.switchState(PageState::MAIN_MENU);
                }
            } else if (event.type == sf::Event::KeyPressed) {
                gameManager.handleEvent(event);
            }
        } else if (event.type == sf::Event::MouseButtonPressed) {
            int clicked = pageManager.handleClick(sf::Mouse::getPosition(window));
            if (clicked == 0) {
                pageManager.switchState(PageState::GAME);
                gameManager.loadGame(std::make_unique<Tetris>());
            }
            else if (clicked == 1);
            else if (clicked == 2) window.close();
        }
    }
}

void drawFrame(const sf::Sprite& framedSprite) {
    sf::RectangleShape frame;
    frame.setSize(sf::Vector2f(framedSprite.getTexture()->getSize()));
    frame.setOrigin(frame.getSize()/2.f);
    frame.setPosition(sf::Vector2f(window.getSize())/2.f);
    frame.setPosition(Constants::WINDOW_WIDTH/2.f, Constants::WINDOW_HEIGHT/2.f);
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineColor(sf::Color::White);
    frame.setOutlineThickness(3);
    window.draw(frame);
}