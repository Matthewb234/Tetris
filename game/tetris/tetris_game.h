#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H
#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

#include "drop_timer.h"
#include "tetromino.h"

class TetrisGame {
private:
    std::vector<std::vector<sf::Color>> grid;
    std::unique_ptr<Tetromino> currentPiece;
    std::unique_ptr<Tetromino> storedPiece;
    DropTimer dropTimer;
    bool invalidSpawn = false;

public:
    TetrisGame();
    void spawnPiece();
    void movePiece(int, int);
    void rotatePiece(bool);
    void lockPiece();
    void pushDownPiece();
    void storePiece();
    void clearLines();
    bool update();
    void draw(sf::RenderTarget&);
};
#endif // TETRIS_GAME_H
