#include "tetromino.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include "../constants.h"


Tetromino::Tetromino(int shapeIndex) {
    shape = Constants::SHAPES[shapeIndex];
    color = Constants::COLORS[shapeIndex];
    x = Constants::GRID_WIDTH / 2 - shape[0].size() / 2;
    y = 0;
}

void Tetromino::rotate(bool rotateRight) {
    int rows = shape.size();
    int cols = shape[0].size();
    std::vector rotated(cols, std::vector<int>(rows));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (rotateRight) {
                rotated[j][rows - 1 - i] = shape[i][j];
            } else {
                rotated[cols - 1 - j][i] = shape[i][j];
            }
        }
    }
    shape = rotated;
}

void Tetromino::zero() {
    x = Constants::GRID_WIDTH / 2 - shape[0].size() / 2;
    y = 0;
}

void Tetromino::drawPiece(sf::RenderTarget& target, bool isGhost) {
    if (isGhost && (ghostY < y)) return;
    int yPos = isGhost ? ghostY : y;

    int rows = shape.size();
    int cols = shape[0].size();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (shape[i][j]) {
                sf::RectangleShape block(sf::Vector2f(Constants::BLOCK_SIZE - 1, Constants::BLOCK_SIZE - 1));
                block.setPosition((x + (j - (cols/2))) * Constants::BLOCK_SIZE,
                                (yPos + (i - (rows/2))) * Constants::BLOCK_SIZE);
                block.setFillColor(sf::Color(color.r, color.g, color.b, isGhost ? 180 : 255));
                target.draw(block);
            }
        }
    }
}

void Tetromino::calcGhostPosition(std::vector<std::vector<sf::Color>> grid) {
    ghostY = 0;
    while (isValidMove(0, ghostY - y + 1, grid)) {
        ghostY++;
    }
}

bool Tetromino::isValidMove(int dx, int dy, std::vector<std::vector<sf::Color>> grid) {
    int newX = x + dx;
    int newY = y + dy;

    int rows = shape.size();
    int cols = shape[0].size();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (shape[i][j]) {
                int gridX = newX + (j - (cols/2));
                int gridY = newY + (i - (rows/2));

                if (gridX < 0 || gridX >= Constants::GRID_WIDTH || gridY >= Constants::GRID_HEIGHT)
                    return false;
                if (gridY >= 0 && grid[gridY][gridX] != sf::Color::Black)
                    return false;
            }
        }
    }
    return true;
}
