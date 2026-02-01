#include "tetris.h"

#include <SFML/Graphics/RectangleShape.hpp>

#include "tetromino.h"
#include "../../application_context.h"
#include "../../constants.h"

Tetris::Tetris() : dropTimer(DropTimer()) {
    grid.resize(TetrisConstants::GRID_HEIGHT_BLOCKS, std::vector<sf::Color>(TetrisConstants::GRID_WIDTH_BLOCKS, sf::Color::Black));
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
        TetrisConstants::GRID_HEIGHT_BLOCKS * TetrisConstants::BLOCK_SIZE
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
    scoreDisplay = std::make_unique<ScoreDisplay>(
        gridSprite.getPosition().x - (gridTexture.getSize().x/2 + 3),
        gridSprite.getPosition().y + (gridTexture.getSize().y/2 + 3)
    );
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
    if (!currentPiece->isValidMove(0, 1, grid)) {
        invalidSpawn = true;
    }
    nextPieceDisplay->setTetromino(nextPiece.get());
    canStorePiece = true;
    dropTimer.resetTimers(false);
}

bool Tetris::movePiece(int dx, int dy) {
    if (currentPiece->isValidMove(dx, dy, grid)) {
        currentPiece->coords.x += dx;
        currentPiece->coords.y += dy;
        dropTimer.resetTimers(false);
        return true;
    }
    return false;
}

void Tetris::rotatePiece(bool rotateRight) {
    auto previousRotation = currentPiece->currentRotation;
    currentPiece->rotate(rotateRight);
    for (const auto& offset : currentPiece->getWallKicks(previousRotation)) {
        if (movePiece(offset.x, offset.y)) {
            checkTSpin();
            return;
        }
    }
    currentPiece->rotate(!rotateRight);
}

void Tetris::checkTSpin() {
    if (currentPiece->baseShape != TetrisConstants::SHAPES[2]) return;
    int x = currentPiece->coords.x;
    int y = currentPiece->coords.y;
    auto isBlocked = [&](int checkX, int checkY) {
        if (checkX < 0 || checkX >= TetrisConstants::GRID_WIDTH_BLOCKS ||
            checkY < 0 || checkY >= TetrisConstants::GRID_HEIGHT_BLOCKS) {
            return true;  // Out of bounds counts as filled
            }
        return grid[checkY][checkX] != sf::Color::Black;
    };

    bool topLeft = isBlocked(x - 1, y - 1);
    bool topRight = isBlocked(x + 1, y - 1);
    bool bottomLeft = isBlocked(x - 1, y + 1);
    bool bottomRight = isBlocked(x + 1, y + 1);

    int filledCorners = topLeft + topRight + bottomLeft + bottomRight;
    if (filledCorners < 3) return;

    RotationState rotation = currentPiece->currentRotation;
    bool isMini = false;

    if (rotation == N && bottomLeft && bottomRight && !topLeft && !topRight) {
        isMini = true;
    } else if (rotation == E && topLeft && bottomLeft && !topRight && !bottomRight) {
        isMini = true;
    } else if (rotation == S && topLeft && topRight && !bottomLeft && !bottomRight) {
        isMini = true;
    } else if (rotation == W && topRight && bottomRight && !topLeft && !bottomLeft) {
        isMini = true;
    }
    currentTSpinType = isMini ? TSpinType::MINI : TSpinType::FULL;
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

                if (gridY >= 0 && gridY < TetrisConstants::GRID_HEIGHT_BLOCKS &&
                    gridX >= 0 && gridX < TetrisConstants::GRID_WIDTH_BLOCKS) {
                    grid[gridY][gridX] = currentPiece->color;
                }
            }
        }
    }
    clearLines();
    currentTSpinType = TSpinType::NONE;
    spawnPiece();
}

void Tetris::storePiece() {
    if (!canStorePiece) return;
    if (storedPiece == nullptr) {
        storedPiece = std::move(currentPiece);
        spawnPiece();
    } else {
        std::swap(currentPiece, storedPiece);
        currentPiece->zero();
    }
    storedPieceDisplay->setTetromino(storedPiece.get());
    canStorePiece = false;
}

void Tetris::hardDrop() {
    if (currentPiece->coords.ghostY < currentPiece->coords.y) return;
    int delta = currentPiece->coords.ghostY - currentPiece->coords.y;
    currentPiece->coords.y = currentPiece->coords.ghostY;
    lockPiece();
    scoreDisplay->getScoreManager().addDropScore(delta, true);
}

void Tetris::softDrop() {
    isSoftDropping = true;
    if (currentPiece->isValidMove(0, 1, grid)) {
        currentPiece->coords.y++;
        scoreDisplay->getScoreManager().addDropScore(1, false);
    }
}

void Tetris::clearLines() {
    int linesCleared = 0;
    for (int i = TetrisConstants::GRID_HEIGHT_BLOCKS - 1; i >= 0; i--) {
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
            linesCleared++;
            i++;
        }
    }
    if (currentTSpinType != TSpinType::NONE) {
        scoreDisplay->getScoreManager().addTSpinScore(currentTSpinType == TSpinType::MINI, linesCleared);
    } else if (linesCleared != 0) {
        scoreDisplay->getScoreManager().incrementLines(linesCleared);
    }
    dropTimer.updateDropInterval(scoreDisplay->getScoreManager().getLevel());
}

void Tetris::drawGrid() {
    gridTexture.clear(sf::Color::Transparent);

    for (int i = 0; i < TetrisConstants::GRID_HEIGHT_BLOCKS; i++) {
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

void Tetris::drawGameContent() {
    drawGrid();
    board->drawToBoard(storedPieceDisplay->getSprite());
    board->drawToBoard(nextPieceDisplay->getSprite());
    board->drawToBoard(scoreDisplay->getSprite());
}

int Tetris::getScore() {
    return scoreDisplay->getScoreManager().getScore();
}

bool Tetris::update() {
    if (invalidSpawn) return false;
    currentPiece->calcGhostPosition(grid);
    scoreDisplay->updateDisplay();

    if (currentPiece->isValidMove(0, 1, grid)) {
        if (dropTimer.shouldDrop()) {
            currentPiece->coords.y++;
        }
    } else {
        if (dropTimer.shouldLock()) {
            lockPiece();
        }
    }

    return true;
}

void Tetris::activateInputContext() {
    auto& input = ApplicationContext::get().getInputManager();
    input.setActiveContext("tetris");
};

void Tetris::handleEvent(sf::Event event) {
    auto& input = ApplicationContext::get().getInputManager();
    if (event.type == sf::Event::KeyReleased) {
        if (input.isPressed("Move Down", event.key.code)) {
            isSoftDropping = false;
        } else {
            return;
        }
    }
    if (input.isPressed("Move Left", event.key.code)) {
        movePiece(-1, 0);
    } else if (input.isPressed("Move Right", event.key.code)) {
        movePiece(1, 0);
    } else if (input.isPressed("Move Down", event.key.code)) {
        softDrop();
    } else if (input.isPressed("Rotate CW", event.key.code)) {
        rotatePiece(true);
    } else if (input.isPressed("Rotate CCW", event.key.code)) {
        rotatePiece(false);
    } else if (input.isPressed("Store", event.key.code)) {
        storePiece();
    } else if (input.isPressed("Push Down", event.key.code)) {
        hardDrop();
    }
}