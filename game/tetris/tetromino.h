#ifndef TETROMINO_H
#define TETROMINO_H
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Tetromino {
public:
    std::vector<std::vector<int>> shape;
    sf::Color color;
    int x, y, ghostY;

    explicit Tetromino(int shapeIndex);
    void rotate(bool);
    void zero();
    void drawPiece(sf::RenderTarget&, bool);
    void calcGhostPosition(std::vector<std::vector<sf::Color>>);
    bool isValidMove(int, int, std::vector<std::vector<sf::Color>>);
};
#endif // TETROMINO_H
