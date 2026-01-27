#ifndef TETRIS_GAMEBOARD_H
#define TETRIS_GAMEBOARD_H

#pragma once
#include <SFML/Graphics.hpp>

#include "../../constants.h"

class GameBoard {
private:
    sf::RenderTexture renderTexture;
    sf::Vector2f boardPosition;
    sf::Vector2f boardSize;

public:
    GameBoard(float width = Constants::WINDOW_WIDTH,
        float height = Constants::WINDOW_HEIGHT);

    // Set the position of the entire game board
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& position);

    // Get position and size info
    sf::Vector2f getPosition() const { return boardPosition; }
    sf::Vector2f getSize() const { return boardSize; }

    // Clear the board for new frame
    void clear(const sf::Color& color = sf::Color::Transparent);

    // Draw a sprite onto the board at a relative position
    void drawToBoard(const sf::Drawable& drawable);

    // Finalize the board (call after all drawToBoard calls)
    void display();

    // Get the render texture for more advanced usage
    sf::RenderTexture& getRenderTexture() { return renderTexture; }
};
#endif //TETRIS_GAMEBOARD_H