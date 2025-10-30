#ifndef TETRIS_GAME_STATE_MANAGER_H
#define TETRIS_GAME_STATE_MANAGER_H
#include "tetris/tetris_game.h"
#include <memory>
#include <SFML/Graphics.hpp>

enum class GameState {
    PLAYING,
    PAUSED,
    GAME_OVER
};

class GameStateManager {
private:
    GameState state;
    std::unique_ptr<TetrisGame> currentGame;
    sf::RenderTexture gameTexture;
    sf::Sprite gameSprite;

public:
    GameStateManager() : state(GameState::PLAYING) {
        gameTexture.create(Constants::GAME_WINDOW_WIDTH, Constants::GAME_WINDOW_HEIGHT);
        currentGame = std::make_unique<TetrisGame>();
    }

    TetrisGame& getCurrentGame() { return *currentGame; }
    bool playing() { return state == GameState::PLAYING; }
    bool paused() { return state == GameState::PAUSED; }
    bool gameOver() { return state == GameState::GAME_OVER; }

    void startGame() { state = GameState::PLAYING; }

    void restartGame() {
        currentGame = std::make_unique<TetrisGame>();
        state = GameState::PLAYING;
    }

    void update() {
        if (state == GameState::PLAYING) {
            if (!currentGame->update()) {
                state = GameState::GAME_OVER;
            }
        }
    }

    void draw() {
        // Draw to the texture (internal game rendering)
        if (state == GameState::PLAYING || state == GameState::PAUSED) {
            gameTexture.clear(sf::Color::Black);
            currentGame->draw(gameTexture);  // Draw game to texture
            gameTexture.display();

            // Update sprite with the texture
            gameSprite.setTexture(gameTexture.getTexture());
        }
    }

    sf::Sprite& getSprite() {
        return gameSprite;
    }

    void pauseGame() {
        if (state == GameState::PLAYING) {
            state = GameState::PAUSED;
        }
    }

    void handleEvent(sf::Event event) {
        if (state == GameState::PAUSED) return;

        switch (event.key.code) {
            case sf::Keyboard::Escape:
                pauseGame();
                break;
            case sf::Keyboard::Left:
                currentGame->movePiece(-1, 0);
                break;
            case sf::Keyboard::Right:
                currentGame->movePiece(1, 0);
                break;
            case sf::Keyboard::Down:
                currentGame->movePiece(0, 1);
                break;
            case sf::Keyboard::Up:
            case sf::Keyboard::C:
                currentGame->rotatePiece(true);
                break;
            case sf::Keyboard::X:
                currentGame->rotatePiece(false);
                break;
            case sf::Keyboard::Z:
                currentGame->storePiece();
                break;
            case sf::Keyboard::R:
                restartGame();
                break;
            case sf::Keyboard::Space:
                currentGame->pushDownPiece();
                break;
        }
    }
};

#endif //TETRIS_GAME_STATE_MANAGER_H