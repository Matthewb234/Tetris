#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "game_pop_up.h"
#include "../../constants.h"
#include "../../managers/page_state_manager.h"

class PausePopUp : public GamePopUp {
public:
    PausePopUp() {
        title.setFont(Constants::FONT);
        title.setString("PAUSED");
        title.setCharacterSize(36);
        title.setFillColor(sf::Color::White);

        std::vector<std::string> labels = {"Resume", "Restart", "Quit"};
        for (int i = 0; i < labels.size(); i++) {
            buttons.push_back(std::make_unique<Button>(
                Constants::GAME_POP_UP_DISPLAY_WIDTH / 2,
                Constants::GAME_POP_UP_DISPLAY_HEIGHT * (.3 + (i * .25)),
                Constants::GAME_POP_UP_DISPLAY_WIDTH * .8,
                (Constants::GAME_POP_UP_DISPLAY_HEIGHT - 50) / 5,
                labels[i],
                Constants::FONT,
                40
            ));
        }
    }

    void drawExtraContent() override {
        if (!initialized) {
            sf::FloatRect titleBounds = title.getLocalBounds();
            title.setPosition(
                Constants::GAME_POP_UP_DISPLAY_WIDTH / 2 - titleBounds.width / 2,
                Constants::GAME_POP_UP_DISPLAY_HEIGHT * .075 - titleBounds.height / 2);
            initialized = true;
        }
    }

    GameAction handleClick(sf::Vector2i mousePos) override {
        sf::Transform inverse = displaySprite.getInverseTransform();
        sf::Vector2f localMousePos = inverse.transformPoint(static_cast<sf::Vector2f>(mousePos));
        for (size_t i = 0; i < buttons.size(); i++) {
            if (buttons[i]->isClicked(static_cast<sf::Vector2i>(localMousePos))) {
                switch (i) {
                    case 0:
                        return GameAction::START;
                    case 1:
                        return GameAction::RESTART;
                    case 2:
                        return GameAction::QUIT;
                }
            }
        }
        return GameAction::NONE;
    }
};

#endif // PAUSE_MENU_H