#ifndef TETROMINO_H
#define TETROMINO_H
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "../../constants.h"

struct Coords {
    int x, y, ghostY;

    Coords() : x(0), y(0), ghostY(0) {}
    Coords(int x, int y) : x(x), y(y), ghostY(0) {}
    float boardX() {return x * TetrisConstants::BLOCK_SIZE;}
    float boardY(bool isGhost) {
        if (isGhost) return ghostY * TetrisConstants::BLOCK_SIZE;
        return y * TetrisConstants::BLOCK_SIZE;
    }
};

enum RotationState {
    N,
    E,
    S,
    W
};

class Tetromino {
private:
    sf::RenderTexture pieceTexture;
    sf::RenderTexture ghostTexture;
    sf::Texture blockTexture;

public:
    std::vector<std::vector<int>> baseShape;
    std::vector<std::vector<int>> shape;
    RotationState currentRotation = N;

    sf::Sprite pieceSprite;
    sf::Sprite ghostSprite;

    sf::Color color;
    Coords coords;

    explicit Tetromino(int shapeIndex);
    void createBlockTexture();
    void createSprite();
    sf::Vector2f position(bool);
    void rotate(bool);
    std::vector<sf::Vector2i> getWallKicks(RotationState);
    void zero();
    void calcGhostPosition(const std::vector<std::vector<sf::Color>>&);
    bool isValidMove(int, int, const std::vector<std::vector<sf::Color>>&);
};
#endif // TETROMINO_H
