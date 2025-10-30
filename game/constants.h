#ifndef TETRIS_CONSTANTS_H
#define TETRIS_CONSTANTS_H
#include <vector>
#include <SFML/Graphics/Color.hpp>

namespace Constants {
    constexpr int BLOCK_SIZE = 30;
    constexpr int GRID_WIDTH = 10;
    constexpr int GRID_HEIGHT = 20;
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 800;
    constexpr int GAME_WINDOW_WIDTH = GRID_WIDTH * BLOCK_SIZE;
    constexpr int GAME_WINDOW_HEIGHT = GRID_HEIGHT * BLOCK_SIZE;

    // Tetromino shapes (I, O, T, S, Z, J, L)
    const std::vector<std::vector<std::vector<int>>> SHAPES = {
        {{1,1,1,1},{0,0,0,0}}, // I
        {{1,1},{1,1}}, // O
        {{0,0,0},{1,1,1},{0,1,0}}, // T
        {{0,0,0},{0,1,1},{1,1,0}}, // S
        {{0,0,0},{1,1,0},{0,1,1}}, // Z
        {{0,0,0},{1,1,1},{0,0,1}}, // J
        {{0,0,0},{1,1,1},{1,0,0}}  // L
    };

    const std::vector<sf::Color> COLORS = {
        sf::Color::Cyan,
        sf::Color::Yellow,
        sf::Color::Magenta,
        sf::Color(0, 255, 0),   // Green
        sf::Color::Red,
        sf::Color::Blue,
        sf::Color(255, 165, 0)  // Orange
    };
};

#endif //TETRIS_CONSTANTS_H