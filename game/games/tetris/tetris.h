#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#pragma once
#include <memory>
#include <random>

#include "drop_timer.h"
#include "piece_display.h"
#include "score_manager.h"
#include "tetromino.h"
#include "../../components/game.h"

enum class TSpinType {
    NONE,
    MINI,
    FULL
};

class Tetris : public Game {
private:
    std::vector<std::vector<sf::Color>> grid;
    sf::RenderTexture gridTexture;
    sf::Sprite gridSprite;
    
    std::random_device rd;
    std::mt19937 generator = std::mt19937(rd());
    std::vector<int> pieceBag = {0, 1, 2, 3, 4, 5, 6};
    std::unique_ptr<Tetromino> currentPiece;
    std::unique_ptr<Tetromino> storedPiece;
    std::unique_ptr<Tetromino> nextPiece;

    std::unique_ptr<PieceDisplay> storedPieceDisplay;
    std::unique_ptr<PieceDisplay> nextPieceDisplay;
    std::unique_ptr<ScoreDisplay> scoreDisplay;

    DropTimer dropTimer;

    TSpinType currentTSpinType = TSpinType::NONE;

    bool invalidSpawn = false;
    bool canStorePiece = true;
    bool isSoftDropping = false;

    // Helper methods for rendering with GameBoard
    void drawGrid();
    void drawCurrentPiece();
    void drawGameContent() override;

public:
    explicit Tetris();
    void initializeBoard();
    void spawnPiece();
    bool movePiece(int, int);
    void rotatePiece(bool);
    void lockPiece();
    void softDrop();
    void hardDrop();
    void storePiece();
    void clearLines();
    void checkTSpin();
    int getScore() override;
    void handleEvent(sf::Event event) override;
    bool update() override;
    void activateInputContext() override;
};
#endif // TETRIS_GAME_H