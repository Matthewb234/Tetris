//
// Created by Matthew on 1/31/2026.
//

#ifndef TETRIS_GAME_OVER_POP_UP_H
#define TETRIS_GAME_OVER_POP_UP_H
#include "game_pop_up.h"

class GameOverPopUp : public GamePopUp {
    sf::Text scoreText;
    public:
    GameOverPopUp() {
        title.setFont(font);
        title.setString("GAME OVER");
        title.setCharacterSize(36);
        title.setFillColor(sf::Color::White);
        sf::FloatRect titleBounds = title.getLocalBounds();
        title.setPosition(
            Constants::GAME_POP_UP_DISPLAY_WIDTH / 2 - titleBounds.width / 2,
            Constants::GAME_POP_UP_DISPLAY_HEIGHT * .15 - titleBounds.height / 2);

        scoreText.setFont(font);
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::White);

        std::vector<std::string> labels = {"Restart", "Quit"};
        for (int i = 0; i < labels.size(); i++) {
            buttons.push_back(std::make_unique<Button>(
                Constants::GAME_POP_UP_DISPLAY_WIDTH / 2,
                Constants::GAME_POP_UP_DISPLAY_HEIGHT * (.6 + (i * .25)),
                Constants::GAME_POP_UP_DISPLAY_WIDTH * .8,
                (Constants::GAME_POP_UP_DISPLAY_HEIGHT - 50) / 5,
                labels[i],
                font,
                40
            ));
        }
    }

    void updateScore(int score) {
        scoreText.setString("Score: " + std::to_string(score));
        sf::FloatRect titleBounds = scoreText.getLocalBounds();
        scoreText.setPosition(
            Constants::GAME_POP_UP_DISPLAY_WIDTH / 2 - titleBounds.width / 2,
            Constants::GAME_POP_UP_DISPLAY_HEIGHT * .3 - titleBounds.height / 2);
    }

    void drawExtraContent() override {
        displayTexture.draw(scoreText);
    }

    GameAction handleClick(sf::Vector2i mousePos) override {
        sf::Transform inverse = displaySprite.getInverseTransform();
        sf::Vector2f localMousePos = inverse.transformPoint(static_cast<sf::Vector2f>(mousePos));
        for (size_t i = 0; i < buttons.size(); i++) {
            if (buttons[i]->isClicked(static_cast<sf::Vector2i>(localMousePos))) {
                switch (i) {
                    case 0:
                        return GameAction::RESTART;
                    case 1:
                        return GameAction::QUIT;
                }
            }
        }
        return GameAction::NONE;
    }
};
#endif //TETRIS_GAME_OVER_POP_UP_H