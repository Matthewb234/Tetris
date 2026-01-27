#include "gameboard.h"

GameBoard::GameBoard(float width, float height)
    : boardPosition(0.f, 0.f), boardSize(width, height) {

    renderTexture.create(static_cast<unsigned int>(width),
                        static_cast<unsigned int>(height));
}

void GameBoard::setPosition(float x, float y) {
    boardPosition = sf::Vector2f(x, y);
}

void GameBoard::setPosition(const sf::Vector2f& position) {
    boardPosition = position;
}

void GameBoard::clear(const sf::Color& color) {
    renderTexture.clear(color);
}

void GameBoard::drawToBoard(const sf::Drawable& drawable) {
    renderTexture.draw(drawable);
}

void GameBoard::display() {
    renderTexture.display();
}