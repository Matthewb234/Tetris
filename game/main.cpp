#include <SFML/Graphics.hpp>
#include <ctime>
#include <iostream>

#include "constants.h"
#include "game_state_manager.h"
#include "components/pause_menu.h"
#include "tetris/tetris.h"
sf::RenderWindow window(sf::VideoMode(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT), "Tetris");
GameStateManager gameManager(&window);
PauseMenu pauseMenu;

sf::RectangleShape overlay(sf::Vector2f(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT));

// METHODS:
void drawFrame(const sf::Sprite&);
void manageEvent();

int main() {
    std::cout << "Tetris starting..." << std::endl;
    srand(time(nullptr));

    window.setFramerateLimit(60);

    pauseMenu.centerOn(Constants::WINDOW_WIDTH/2.f, Constants::WINDOW_HEIGHT/2.f);

    overlay.setFillColor(sf::Color(0, 0, 0, 180)); // Black with alpha (0-255)
    overlay.setPosition(0, 0);

    gameManager.loadGame(std::make_unique<Tetris>(&window)); // TEMP

    while (window.isOpen()) {
        manageEvent();

        gameManager.update();
        gameManager.draw();  // Draws to internal texture

        // === Main Window Drawing ===
        window.clear(sf::Color(30, 30, 40)); // Dark backdrop

        // Position the game sprite (the "inner window")
        sf::Sprite& gameSprite = gameManager.getSprite();
        gameSprite.setPosition(Constants::WINDOW_WIDTH/2.f, Constants::WINDOW_HEIGHT/2.f); // Position within the large window

        // Optional: Add a border/frame
        // drawFrame(gameSprite);

        // Draw the game
        window.draw(gameSprite);

        // Draw pause menu on top if needed
        if (gameManager.paused()) {
            window.draw(overlay);

            pauseMenu.update(sf::Mouse::getPosition(window));
            pauseMenu.draw();  // Renders to its own texture
            window.draw(pauseMenu.getSprite());  // Draw the menu sprite
        }

        // Add other UI elements here (score, next piece, etc.)

        window.display();
    }

    return 0;
}

void manageEvent() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        else if (gameManager.paused() && event.type == sf::Event::MouseButtonPressed) {
            int clicked = pauseMenu.handleClick(sf::Mouse::getPosition(window));
            if (clicked == 0) gameManager.startGame();
            else if (clicked == 1) gameManager.restartGame();
            else if (clicked == 2) window.close();
        } else if (event.type == sf::Event::KeyPressed)
            gameManager.handleEvent(event);


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