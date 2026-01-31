#include "tetromino.h"

#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>

#include "../../constants.h"


Tetromino::Tetromino(int shapeIndex) {
    shape = TetrisConstants::SHAPES[shapeIndex];
    baseShape = shape;
    color = TetrisConstants::COLORS[shapeIndex];
    zero();
    createBlockTexture();
    createSprite();
}

void Tetromino::createBlockTexture() {
    sf::Image blockImage;
    blockImage.create(TetrisConstants::BLOCK_SIZE - 1, TetrisConstants::BLOCK_SIZE - 1, sf::Color::White);
    blockTexture.loadFromImage(blockImage);
}

void Tetromino::createSprite() {
    int pieceSize = shape[0].size() * TetrisConstants::BLOCK_SIZE;
    pieceTexture.create(pieceSize, pieceSize);
    pieceTexture.clear(sf::Color::Transparent);

    int rows = shape.size();
    int cols = shape[0].size();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!shape[i][j]) continue;
            sf::RectangleShape block(sf::Vector2f(TetrisConstants::BLOCK_SIZE - 1, TetrisConstants::BLOCK_SIZE - 1));
            block.setTexture(&blockTexture);
            block.setPosition(j * TetrisConstants::BLOCK_SIZE, i * TetrisConstants::BLOCK_SIZE);
            block.setFillColor(color);
            pieceTexture.draw(block);
        }
    }
    pieceTexture.display();

    pieceSprite.setTexture(pieceTexture.getTexture());
    int origin = (shape[0].size() / 2) * TetrisConstants::BLOCK_SIZE;
    pieceSprite.setOrigin(origin, origin);
    ghostSprite = pieceSprite;
    ghostSprite.setColor(sf::Color(255, 255, 255, 100));
}

sf::Vector2f Tetromino::position(bool isGhost) {
    return {coords.boardX(), coords.boardY(isGhost)};
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
    if (rotateRight) {
        currentRotation = static_cast<RotationState>((static_cast<int>(currentRotation) + 1) % 4);
    } else {
        currentRotation = static_cast<RotationState>((static_cast<int>(currentRotation) + 3) % 4);
    }
    createSprite();
}

std::vector<sf::Vector2i> Tetromino::getWallKicks(RotationState previous) {
    std::vector<sf::Vector2i> kicks;
    auto& offsetTable = (baseShape != TetrisConstants::SHAPES[0])
        ? TetrisConstants::WALL_KICKS_JLSTZ
        : TetrisConstants::WALL_KICKS_I;

    for (int i = 0; i < 5; i++) {
        kicks.push_back({
            offsetTable[previous][i].x - offsetTable[currentRotation][i].x,
            offsetTable[previous][i].y - offsetTable[currentRotation][i].y
        });
    }
    return kicks;
}

void Tetromino::zero() {
    shape = baseShape;
    coords.x = TetrisConstants::GRID_WIDTH_BLOCKS / 2;
    coords.y = shape == TetrisConstants::SHAPES[0] ? 0 : -1;

    createSprite();
}

void Tetromino::calcGhostPosition(const std::vector<std::vector<sf::Color>>& grid) {
    coords.ghostY = coords.y;
    while (isValidMove(0, coords.ghostY - coords.y + 1, grid)) {
        coords.ghostY++;
    }
}

bool Tetromino::isValidMove(int dx, int dy, std::vector<std::vector<sf::Color>> grid) {
    int rows = shape.size();
    int cols = shape[0].size();

    int newX = coords.x + dx;
    int newY = coords.y + dy;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (shape[i][j]) {
                int gridX = newX + (j - (cols/2));
                int gridY = newY + (i - (rows/2));

                if (gridX < 0 || gridX >= TetrisConstants::GRID_WIDTH_BLOCKS || gridY >= TetrisConstants::GRID_HEIGHT_BlOCKS)
                    return false;
                if (gridY >= 0 && grid[gridY][gridX] != sf::Color::Black)
                    return false;
            }
        }
    }
    return true;
}
