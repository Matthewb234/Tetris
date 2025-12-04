#include "tetris.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include "tetromino.h"
#include "../constants.h"

Tetris::Tetris(sf::RenderWindow* win)
    : window(win), dropTimer(DropTimer(1)) {
    grid.resize(TetrisConstants::GRID_HEIGHT_BlOCKS, std::vector<sf::Color>(TetrisConstants::GRID_WIDTH_BLOCKS, sf::Color::Black));
    initializeBoard();
    nextPiece = std::make_unique<Tetromino>(numGen(generator));
    spawnPiece();
}


void Tetris::initializeBoard() {
    float boardX = 0.f;
    float boardY = 0.f;

    board = std::make_unique<GameBoard>();
    board->setPosition(boardX, boardY);

    gridTexture.create(
        TetrisConstants::GRID_WIDTH_BLOCKS * TetrisConstants::BLOCK_SIZE,
        TetrisConstants::GRID_HEIGHT_BlOCKS * TetrisConstants::BLOCK_SIZE
    );
    gridSprite.setTexture(gridTexture.getTexture());
    gridSprite.setOrigin(TetrisConstants::GRID_WIDTH/2.f, TetrisConstants::GRID_HEIGHT/2.f);
    gridSprite.setPosition(board->getSize()/2.f);

    storedPieceDisplay = std::make_unique<PieceDisplay>(
        gridSprite.getPosition().x - (gridTexture.getSize().x/2),
        gridSprite.getPosition().y - (gridTexture.getSize().y/2 + 3),
        sf::Vector2f(TetrisConstants::DISPLAY_WIDTH,0), "HOLD");
    nextPieceDisplay = std::make_unique<PieceDisplay>(
    gridSprite.getPosition().x + (gridTexture.getSize().x/2),
    gridSprite.getPosition().y - (gridTexture.getSize().y/2 + 3),
    sf::Vector2f(0, 0), "NEXT");
}

void Tetris::spawnPiece() {
    int num = numGen(generator);
    currentPiece = std::move(nextPiece);
    nextPiece = std::make_unique<Tetromino>(num);
    if (!currentPiece->isValidMove(0, 0, grid)) {
        invalidSpawn = true;
    }
    nextPieceDisplay->setTetrimino(nextPiece.get());
}

void Tetris::movePiece(int dx, int dy) {
    if (currentPiece->isValidMove(dx, dy, grid)) {
        currentPiece->coords.x += dx;
        currentPiece->coords.y += dy;
    }
}

void Tetris::rotatePiece(bool rotateRight) {
    auto oldShape = currentPiece->shape;
    currentPiece->rotate(rotateRight);
    if (!currentPiece->isValidMove(0, 0, grid)) {
        currentPiece->shape = oldShape;
        currentPiece->createSprite();
    }
}

void Tetris::lockPiece() {
    int rows = currentPiece->shape.size();
    int cols = currentPiece->shape[0].size();

    int topLeftX = currentPiece->coords.x - (cols / 2);
    int topLeftY = currentPiece->coords.y - (rows / 2);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (currentPiece->shape[i][j]) {
                int gridX = topLeftX + j;
                int gridY = topLeftY + i;

                if (gridY >= 0 && gridY < TetrisConstants::GRID_HEIGHT_BlOCKS &&
                    gridX >= 0 && gridX < TetrisConstants::GRID_WIDTH_BLOCKS) {
                    grid[gridY][gridX] = currentPiece->color;
                    }
            }
        }
    }
    markGridDirty();
    clearLines();
    spawnPiece();
}

void Tetris::storePiece() {
    if (storedPiece == nullptr) {
        storedPiece = std::move(currentPiece);
        spawnPiece();
    } else {
        std::swap(currentPiece, storedPiece);
        currentPiece->zero();
    }
    storedPieceDisplay->setTetrimino(storedPiece.get());
}

void Tetris::pushDownPiece() {
    if (currentPiece->coords.ghostY < currentPiece->coords.y) return;
    currentPiece->coords.y = currentPiece->coords.ghostY;
    lockPiece();
}

void Tetris::clearLines() {
    for (int i = TetrisConstants::GRID_HEIGHT_BlOCKS - 1; i >= 0; i--) {
        bool fullLine = true;
        for (int j = 0; j < TetrisConstants::GRID_WIDTH_BLOCKS; j++) {
            if (grid[i][j] == sf::Color::Black) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            grid.erase(grid.begin() + i);
            grid.insert(grid.begin(), std::vector<sf::Color>(TetrisConstants::GRID_WIDTH_BLOCKS, sf::Color::Black));
            i++; // Check the same line again
            markGridDirty();
        }
    }
}

void Tetris::drawGrid() {
    if (gridNeedsUpdate) {
        gridTexture.clear(sf::Color::Transparent);

        for (int i = 0; i < TetrisConstants::GRID_HEIGHT_BlOCKS; i++) {
            for (int j = 0; j < TetrisConstants::GRID_WIDTH_BLOCKS; j++) {
                sf::RectangleShape block(sf::Vector2f(
                    TetrisConstants::BLOCK_SIZE - 1,
                    TetrisConstants::BLOCK_SIZE - 1
                ));
                block.setPosition(
                    j * TetrisConstants::BLOCK_SIZE,
                    i * TetrisConstants::BLOCK_SIZE
                );
                block.setFillColor(grid[i][j]);

                gridTexture.draw(block);
            }
        }

        gridTexture.display();
        gridNeedsUpdate = false;
    }
    board->drawToBoard(gridSprite);

    sf::RectangleShape frame(static_cast<sf::Vector2f>(gridTexture.getSize()));
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineColor(sf::Color::White);
    frame.setOutlineThickness(3);
    frame.setOrigin(gridSprite.getOrigin());
    frame.setPosition(gridSprite.getPosition());
    board->drawToBoard(frame);
}

void Tetris::drawCurrentPiece() {
    if (currentPiece->coords.y < 1) return; //TODO Fix this to show partial shapes
    sf::Vector2f baseCoords = gridSprite.getPosition() - gridSprite.getOrigin();
    currentPiece->ghostSprite.setPosition(currentPiece->position(true) + baseCoords);
    currentPiece->pieceSprite.setPosition(currentPiece->position(false) + baseCoords);
    board->drawToBoard(currentPiece->pieceSprite);
    board->drawToBoard(currentPiece->ghostSprite);
}

void Tetris::markGridDirty() {
    gridNeedsUpdate = true;
}

void Tetris::handleEvent(sf::Event event) {
    switch (event.key.code) {
        case sf::Keyboard::Left:
            movePiece(-1, 0);
            break;
        case sf::Keyboard::Right:
            movePiece(1, 0);
            break;
        case sf::Keyboard::Down:
            movePiece(0, 1);
            break;
        case sf::Keyboard::Up:
        case sf::Keyboard::C:
            rotatePiece(true);
            break;
        case sf::Keyboard::X:
            rotatePiece(false);
            break;
        case sf::Keyboard::Z:
            storePiece();
            break;
        case sf::Keyboard::Space:
            pushDownPiece();
            break;
    }
}

void Tetris::drawGameContent() {
    drawGrid();
    drawCurrentPiece();
    board->drawToBoard(storedPieceDisplay->getSprite());
    board->drawToBoard(nextPieceDisplay->getSprite());
}

bool Tetris::update() {
    if (invalidSpawn) return false;
    currentPiece->calcGhostPosition(grid);

    if (dropTimer.shouldDrop()) {
        if (currentPiece->isValidMove(0, 1, grid)) {
            currentPiece->coords.y++;
        } else {
            lockPiece();
        }
    }

    return true;
}