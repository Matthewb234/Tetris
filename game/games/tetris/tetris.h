#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#pragma once
#include <memory>
#include <random>
#include <SFML/Graphics/RenderWindow.hpp>

#include "drop_timer.h"
#include "piece_display.h"
#include "tetromino.h"
#include "../../components/gameboard/gameboard.h"
#include "../../components/game.h"

class Tetris : public Game {
private:
    std::vector<std::vector<sf::Color>> grid;
    sf::RenderTexture gridTexture;
    sf::Sprite gridSprite;
    
    std::random_device rd;
    std::mt19937 generator = std::mt19937(rd());
    std::uniform_int_distribution<> numGen = std::uniform_int_distribution<>(0, 6);

    std::vector<int> pieceBag = {0, 1, 2, 3, 4, 5, 6};
    std::unique_ptr<Tetromino> currentPiece;
    std::unique_ptr<Tetromino> storedPiece;
    std::unique_ptr<Tetromino> nextPiece;

    std::unique_ptr<PieceDisplay> storedPieceDisplay;
    std::unique_ptr<PieceDisplay> nextPieceDisplay;

    DropTimer dropTimer;

    bool invalidSpawn = false;

    // Helper methods for rendering with GameBoard
    void drawGrid();
    void drawCurrentPiece();
    void drawGameContent() override;

public:
    explicit Tetris();
    void initializeBoard();
    void spawnPiece();
    void movePiece(int, int);
    void rotatePiece(bool);
    void lockPiece();
    void pushDownPiece();
    void storePiece();
    void clearLines();
    void handleEvent(sf::Event event) override;
    bool update() override;
};
#endif // TETRIS_GAME_H