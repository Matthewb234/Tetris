#include "tetris.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include "tetromino.h"
#include "../../constants.h"

Tetris::Tetris()
    : dropTimer(DropTimer(1)) {
    grid.resize(TetrisConstants::GRID_HEIGHT_BlOCKS, std::vector<sf::Color>(TetrisConstants::GRID_WIDTH_BLOCKS, sf::Color::Black));
    initializeBoard();

    std::shuffle(pieceBag.begin(), pieceBag.end(), generator);
    nextPiece = std::make_unique<Tetromino>(pieceBag.back());
    pieceBag.pop_back();
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
    if (pieceBag.empty()) {
        pieceBag = {0, 1, 2, 3, 4, 5, 6};
        std::shuffle(pieceBag.begin(), pieceBag.end(), generator);
    }
    int num = pieceBag.back();
    pieceBag.pop_back();
    currentPiece = std::move(nextPiece);
    nextPiece = std::make_unique<Tetromino>(num);
    if (!currentPiece->isValidMove(0, 0, grid)) {
        invalidSpawn = true;
    }
    nextPieceDisplay->setTetromino(nextPiece.get());
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
    storedPieceDisplay->setTetromino(storedPiece.get());
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
        }
    }
}

void Tetris::drawGrid() {
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

    drawCurrentPiece();
    gridTexture.display();
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
    currentPiece->ghostSprite.setPosition(currentPiece->position(true));
    currentPiece->pieceSprite.setPosition(currentPiece->position(false));
    gridTexture.draw(currentPiece->ghostSprite);
    gridTexture.draw(currentPiece->pieceSprite);
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