#ifndef TETRIS_GAME_STATE_MANAGER_H
#define TETRIS_GAME_STATE_MANAGER_H
#include <memory>
#include <SFML/Graphics.hpp>

#include "../components/game.h"
#include "../games/tetris/tetris.h"

enum class GameState {
    LOADING,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class GameStateManager {
private:
    GameState state;
    std::unique_ptr<Game> currentGame;
    sf::RenderTexture gameTexture;
    sf::Sprite gameSprite;

public:
    explicit GameStateManager() : state(GameState::LOADING) {}

    Game& getCurrentGame() { return *currentGame; }
    bool hasGameLoaded() {return currentGame != nullptr;}
    bool loading() { return state == GameState::LOADING; }
    bool playing() { return state == GameState::PLAYING; }
    bool paused() { return state == GameState::PAUSED; }
    bool gameOver() { return state == GameState::GAME_OVER; }

    void buildDefaultGame() {
        currentGame = std::make_unique<Tetris>();
        gameTexture.create(currentGame->textureSize().x, currentGame->textureSize().y);
        currentGame->activateInputContext();
    }

    void loadGame(std::unique_ptr<Game> game) {
        currentGame = std::move(game);
        gameTexture.create(currentGame->textureSize().x, currentGame->textureSize().y);
        currentGame->activateInputContext();
    }

    void startGame() { state = GameState::PLAYING; }

    void restartGame() {
        currentGame = std::make_unique<Tetris>();
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
            gameTexture.clear(sf::Color::Transparent);
            currentGame->draw(gameTexture);  // Draw game to texture
            gameTexture.display();

            // Update sprite with the texture
            gameSprite.setTexture(gameTexture.getTexture());
            gameSprite.setOrigin(currentGame->textureSize()/2.f);
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
        switch (event.key.code) {
            case sf::Keyboard::Escape:
                pauseGame();
                break;
            case sf::Keyboard::R:
                restartGame();
                break;
            default:
                currentGame->handleEvent(event);
                break;
        }
    }
};

#endif //TETRIS_GAME_STATE_MANAGER_H