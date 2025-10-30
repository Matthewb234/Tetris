#include "tetris_game.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include "tetromino.h"
#include "../constants.h"


TetrisGame::TetrisGame() : dropTimer(DropTimer(1)) {
    grid.resize(Constants::GRID_HEIGHT, std::vector<sf::Color>(Constants::GRID_WIDTH, sf::Color::Black));
    spawnPiece();
}

void TetrisGame::spawnPiece() {
    currentPiece = std::make_unique<Tetromino>(rand() % Constants::SHAPES.size());
    if (!currentPiece->isValidMove(0, 0, grid)) {
        invalidSpawn = true;
    }
}

void TetrisGame::movePiece(int dx, int dy) {
    if (currentPiece->isValidMove(dx, dy, grid)) {
        currentPiece->x += dx;
        currentPiece->y += dy;
    }
}

void TetrisGame::rotatePiece(bool rotateRight) {
    auto oldShape = currentPiece->shape;
    currentPiece->rotate(rotateRight);
    if (!currentPiece->isValidMove(0, 0, grid)) {
        currentPiece->shape = oldShape; // Revert rotation
    }
}

void TetrisGame::lockPiece() {
    int rows = currentPiece->shape.size();
    int cols = currentPiece->shape[0].size();
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (currentPiece->shape[i][j]) {
                int gridY = currentPiece->y + (i - (rows/2));
                int gridX = currentPiece->x + (j - (cols/2));
                if (gridY >= 0) {
                    grid[gridY][gridX] = currentPiece->color;
                }
            }
        }
    }
    clearLines();
    spawnPiece();
}

void TetrisGame::storePiece() {
    if (storedPiece == nullptr) {
        storedPiece = std::move(currentPiece);
        spawnPiece();
    } else {
        std::swap(currentPiece, storedPiece);
        currentPiece->zero();
    }
}

void TetrisGame::pushDownPiece() {
    if (currentPiece->ghostY < currentPiece->y) return;
    currentPiece->y = currentPiece->ghostY;
    lockPiece();
}


void TetrisGame::clearLines() {
    for (int i = Constants::GRID_HEIGHT - 1; i >= 0; i--) {
        bool fullLine = true;
        for (int j = 0; j < Constants::GRID_WIDTH; j++) {
            if (grid[i][j] == sf::Color::Black) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            grid.erase(grid.begin() + i);
            grid.insert(grid.begin(), std::vector<sf::Color>(Constants::GRID_WIDTH, sf::Color::Black));
            i++; // Check the same line again
        }
    }
}

bool TetrisGame::update() {
    if (invalidSpawn) return false;
    currentPiece->calcGhostPosition(grid);

    if (dropTimer.shouldDrop()) {
        if (currentPiece->isValidMove(0, 1, grid)) {
            currentPiece->y++;
        } else {
            lockPiece();
        }
    }
    return true;
}

void TetrisGame::draw(sf::RenderTarget& target) {
    // Draw grid
    for (int i = 0; i < Constants::GRID_HEIGHT; i++) {
        for (int j = 0; j < Constants::GRID_WIDTH; j++) {
            sf::RectangleShape block(sf::Vector2f(Constants::BLOCK_SIZE - 1, Constants::BLOCK_SIZE - 1));
            block.setPosition(j * Constants::BLOCK_SIZE, i * Constants::BLOCK_SIZE);
            block.setFillColor(grid[i][j]);
            target.draw(block);
        }
    }

    // Draw current piece
    currentPiece->drawPiece(target, false);
    currentPiece->drawPiece(target, true);
}