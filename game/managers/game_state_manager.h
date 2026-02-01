#ifndef TETRIS_GAME_STATE_MANAGER_H
#define TETRIS_GAME_STATE_MANAGER_H
#include <memory>
#include <SFML/Graphics.hpp>

#include "../components/game.h"
#include "../components/popUp/game_over_pop_up.h"
#include "../components/popUp/pause_pop_up.h"
#include "../games/tetris/tetris.h"

enum class GameState {
    LOADING,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class GameStateManager {
    GameState state;
    std::unique_ptr<Game> currentGame;

    sf::RenderTexture gameTexture;
    sf::Sprite gameSprite;

    sf::RectangleShape overlay;
    PausePopUp pausePopUp;
    GameOverPopUp gameOverPopUp;
public:
    explicit GameStateManager() : state(GameState::LOADING) {
        overlay.setSize(sf::Vector2f(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT));
        overlay.setFillColor(sf::Color(0, 0, 0, 180));
        overlay.setPosition(0, 0);

        pausePopUp.centerOn(Constants::WINDOW_WIDTH/2.f, Constants::WINDOW_HEIGHT/2.f);
        gameOverPopUp.centerOn(Constants::WINDOW_WIDTH/2.f, Constants::WINDOW_HEIGHT/2.f);
    }

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

    void update(sf::Vector2i mousePos) {
        if (state == GameState::PLAYING) {
            if (!currentGame->update()) {
                state = GameState::GAME_OVER;
                gameOverPopUp.updateScore(currentGame->getScore());
            }
        } else if (state == GameState::GAME_OVER) {
            gameOverPopUp.update(mousePos);
        } else if (state == GameState::PAUSED) {
            pausePopUp.update(mousePos);
        }
    }

    void draw() {
        if (state == GameState::LOADING) return;
        gameTexture.clear(sf::Color::Transparent);
        currentGame->draw(gameTexture);
        gameTexture.display();

        if (state == GameState::PAUSED) {
            gameTexture.draw(overlay);

            pausePopUp.draw();
            gameTexture.draw(pausePopUp.getSprite());
        } else if (state == GameState::GAME_OVER) {
            gameTexture.draw(overlay);

            gameOverPopUp.draw();
            gameTexture.draw(gameOverPopUp.getSprite());
        }

        gameSprite.setTexture(gameTexture.getTexture());
        gameSprite.setOrigin(currentGame->textureSize()/2.f);
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
                if (state == GameState::PLAYING) currentGame->handleEvent(event);
                break;
        }
    }

    bool handleMenuClick(sf::Vector2i mousePos) {
        GameAction action;
        if (state == GameState::PAUSED) {
            action = pausePopUp.handleClick(mousePos);
        } else if (state == GameState::GAME_OVER) {
            action = gameOverPopUp.handleClick(mousePos);
        } else {
            action = GameAction::NONE;
        }
        switch (action) {
            case GameAction::START:
                startGame();
                return false;
            case GameAction::RESTART:
                restartGame();
                return false;
            case GameAction::QUIT:
                pauseGame();
                return true;
            default:
                return false;
        }
    }
};

#endif //TETRIS_GAME_STATE_MANAGER_H