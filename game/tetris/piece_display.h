#ifndef TETRIS_STORED_PIECE_DISPLAY_H
#define TETRIS_STORED_PIECE_DISPLAY_H
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include "../constants.h"
#include "tetromino.h"

class PieceDisplay {
private:
    sf::Sprite pieceSprite;

    sf::RenderTexture displayTexture;
    sf::Sprite displaySprite;

    sf::Font font;
    sf::Text label;
    sf::Vector2f position;

public:

    PieceDisplay() = default;
    PieceDisplay(float x, float y, sf::Vector2f origin, const std::string& labelText)
        : position(x, y) {
        displayTexture.create(TetrisConstants::DISPLAY_WIDTH, TetrisConstants::DISPLAY_HEIGHT);

        displaySprite.setTexture(displayTexture.getTexture());
        displaySprite.setScale(1.f, -1.f);
        displaySprite.setOrigin(origin.x, origin.y + displayTexture.getSize().y);
        displaySprite.setPosition(position);

        if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            // Handle font loading error
        }

        label.setFont(font);
        label.setString(labelText);
        label.setCharacterSize(16);
        label.setFillColor(sf::Color::Black);
        label.setPosition(10, displayTexture.getSize().y * .04);  // Top-left when flipped

        setTetrimino(nullptr);
    }

    void setTetrimino(Tetromino* tetromino) {
        displayTexture.clear(sf::Color::Transparent);

        float quarterHeight = displayTexture.getSize().y / 4.f;

        // Draw white top quarter (for label)
        sf::RectangleShape topSection(sf::Vector2f(
            displayTexture.getSize().x,
            quarterHeight));
        topSection.setFillColor(sf::Color::White);
        displayTexture.draw(topSection);

        // Draw label
        displayTexture.draw(label);

        // Draw piece
        if (tetromino != nullptr) {
            tetromino->zero();
            // Center the piece below the label
            pieceSprite = tetromino->pieceSprite;
            int origin = (tetromino->shape[0].size() / 2.f) * TetrisConstants::BLOCK_SIZE;
            pieceSprite.setOrigin(origin, origin);
            pieceSprite.setPosition(sf::Vector2f(
                TetrisConstants::DISPLAY_WIDTH/2,
                TetrisConstants::DISPLAY_HEIGHT * (5/8.f)));
            displayTexture.draw(pieceSprite);
        }

        // Draw border frame
        sf::RectangleShape frame(static_cast<sf::Vector2f>(displayTexture.getSize()));
        frame.setFillColor(sf::Color::Transparent);
        frame.setOutlineColor(sf::Color::White);
        frame.setOutlineThickness(-3);
        displayTexture.draw(frame);
    }

    sf::Sprite& getSprite() {return displaySprite;}
};
#endif //TETRIS_STORED_PIECE_DISPLAY_H