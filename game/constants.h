#ifndef TETRIS_CONSTANTS_H
#define TETRIS_CONSTANTS_H
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics.hpp>
#include <filesystem>


namespace Constants {
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 800;
    constexpr int GAME_POP_UP_DISPLAY_WIDTH = 250;
    constexpr int GAME_POP_UP_DISPLAY_HEIGHT = 350;
    inline sf::Font FONT;

    inline std::filesystem::path& _exeDir() {
        static std::filesystem::path dir;
        return dir;
    }

    inline void initPaths(int argc, char* argv[]) {
        auto exePath = std::filesystem::path(argv[0]).parent_path();
        if (std::filesystem::exists(exePath / "assets")) {
            _exeDir() = exePath;
        }
        else {
            _exeDir() = exePath.parent_path();
        }
    }

    inline std::string getAssetPath(const std::string& relativePath) {
        return (_exeDir() / relativePath).string();
    }

    inline void loadFont() {
        if (!FONT.loadFromFile(getAssetPath("assets\\fonts\\arial.ttf"))) {
            // Handle font loading error
        }
    }
};

namespace TetrisConstants {
    constexpr int BLOCK_SIZE = 30;
    constexpr int GRID_WIDTH_BLOCKS = 10;
    constexpr int GRID_HEIGHT_BLOCKS = 20;
    constexpr int DISPLAY_WIDTH_BLOCKS = 5;
    constexpr int DISPLAY_HEIGHT_BLOCKS = 4;
    constexpr int GRID_WIDTH = GRID_WIDTH_BLOCKS * BLOCK_SIZE;
    constexpr int GRID_HEIGHT = GRID_HEIGHT_BLOCKS * BLOCK_SIZE;
    constexpr int DISPLAY_WIDTH = DISPLAY_WIDTH_BLOCKS * BLOCK_SIZE;
    constexpr int DISPLAY_HEIGHT = DISPLAY_HEIGHT_BLOCKS * BLOCK_SIZE;

    // Tetromino shapes (I, O, T, S, Z, J, L)
    const std::vector<std::vector<std::vector<int>>>  SHAPES = {
        {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // I
        {{1,1},{1,1}}, // O
        {{0,1,0},{1,1,1},{0,0,0}}, // T
        {{0,1,1},{1,1,0},{0,0,0}}, // S
        {{1,1,0},{0,1,1},{0,0,0}}, // Z
        {{1,0,0},{1,1,1},{0,0,0}}, // J
        {{0,0,1},{1,1,1},{0,0,0}}  // L
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

    const std::vector<std::vector<sf::Vector2i>> WALL_KICKS_JLSTZ = {
        {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}}, // N
        {{0,0}, {1,0}, {1,-1}, {0,2}, {1,2}}, // E
        {{0,0}, {0,0}, {0,0}, {0,0}, {0,0}}, // S
        {{0,0}, {-1,0}, {-1,-1}, {0,2}, {-1,2}}, // W
    };

    const std::vector<std::vector<sf::Vector2i>> WALL_KICKS_I = {
        {{0,0}, {-1,0}, {2,0}, {-1,0}, {2,0}}, // N
        {{0,0}, {1,0}, {1,0}, {1,1}, {1,-2}}, // E
        {{0,0}, {2,0}, {-1,0}, {2,-1}, {-1,-1}}, // S
        {{0,0}, {0,0}, {0,0}, {0,-2}, {0,1}} // W
    };
};

#endif //TETRIS_CONSTANTS_H