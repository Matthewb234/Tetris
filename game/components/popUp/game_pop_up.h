//
// Created by Matthew on 1/31/2026.
//

#ifndef TETRIS_GAMEPOPUP_H
#define TETRIS_GAMEPOPUP_H
#include "../buttons.h"
#include "../../constants.h"

enum class GameAction {
    START,
    RESTART,
    QUIT,
    NONE
};

class GamePopUp {
protected:
    sf::RenderTexture displayTexture;
    sf::Sprite displaySprite;
    std::vector<std::unique_ptr<Button>> buttons;
    sf::Text title;
    sf::Font font;

    GamePopUp() {
        displayTexture.create(Constants::GAME_POP_UP_DISPLAY_WIDTH, Constants::GAME_POP_UP_DISPLAY_HEIGHT);

        if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            // Handle font loading error
        }
    }

public:
    void update(sf::Vector2i mousePos) {
        sf::Transform inverse = displaySprite.getInverseTransform();
        sf::Vector2f localMousePos = inverse.transformPoint(static_cast<sf::Vector2f>(mousePos));

        for (size_t i = 0; i < buttons.size(); i++) {
            buttons[i]->update(static_cast<sf::Vector2i>(localMousePos));
        }
    }

    void centerOn(float x, float y) {
        displaySprite.setOrigin(Constants::GAME_POP_UP_DISPLAY_WIDTH / 2, Constants::GAME_POP_UP_DISPLAY_HEIGHT / 2);
        displaySprite.setPosition(x, y);
    }

    sf::Sprite& getSprite() {
        return displaySprite;
    }

    virtual GameAction handleClick(sf::Vector2i mousePos) = 0;

    virtual void drawExtraContent() = 0;

    void draw() {
        displayTexture.clear(sf::Color(20, 20, 30, 230));

        displayTexture.draw(title);
        for (int i = 0; i < buttons.size(); i++) {
            buttons[i]->draw(displayTexture);
        }
        drawExtraContent();

        displayTexture.display();

        displaySprite.setTexture(displayTexture.getTexture());
    }
};
#endif //TETRIS_GAMEPOPUP_H