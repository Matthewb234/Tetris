#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "../constants.h"

class PauseDisplay {
private:
    sf::RenderTexture displayTexture;
    sf::Sprite displaySprite;
    std::vector<std::unique_ptr<Button>> buttons;
    sf::Text title;
    sf::Font font;

public:
    PauseDisplay() {
        displayTexture.create(Constants::PAUSE_DISPLAY_WIDTH, Constants::PAUSE_DISPLAY_HEIGHT);

        if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            // Handle font loading error
        }

        title.setFont(font);
        title.setString("PAUSED");
        title.setCharacterSize(36);
        title.setFillColor(sf::Color::White);
        sf::FloatRect titleBounds = title.getLocalBounds();
        title.setPosition(
            Constants::PAUSE_DISPLAY_WIDTH / 2 - titleBounds.width / 2,
            Constants::PAUSE_DISPLAY_HEIGHT * .075 - titleBounds.height / 2);

        std::vector<std::string> labels = {"Resume", "Restart", "Quit"};
        for (int i = 0; i < labels.size(); i++) {
            buttons.push_back(std::make_unique<Button>(
                Constants::PAUSE_DISPLAY_WIDTH / 2,
                Constants::PAUSE_DISPLAY_HEIGHT * (.3 + (i * .25)),
                Constants::PAUSE_DISPLAY_WIDTH * .8,
                (Constants::PAUSE_DISPLAY_HEIGHT - 50) / 5,
                labels[i],
                font,
                40
            ));
        }
    }

    void update(sf::Vector2i mousePos) {
        sf::Transform inverse = displaySprite.getInverseTransform();
        sf::Vector2f localMousePos = inverse.transformPoint(static_cast<sf::Vector2f>(mousePos));

        for (size_t i = 0; i < buttons.size(); i++) {
            buttons[i]->update(static_cast<sf::Vector2i>(localMousePos));
        }
    }

    void handleClick(sf::Vector2i mousePos) {
        sf::Transform inverse = displaySprite.getInverseTransform();
        sf::Vector2f localMousePos = inverse.transformPoint(static_cast<sf::Vector2f>(mousePos));
        for (size_t i = 0; i < buttons.size(); i++) {
            if (buttons[i]->isClicked(static_cast<sf::Vector2i>(localMousePos))) {
                auto& appCtx = ApplicationContext::get();
                switch (i) {
                    case 0:
                        appCtx.getGameManager().startGame();
                        break;
                    case 1:
                        appCtx.getGameManager().restartGame();
                        break;
                    case 2:
                        appCtx.getGameManager().pauseGame();
                        appCtx.getPageManager().switchState(PageState::MAIN_MENU);
                        break;
                }
            }
        }
    }

    void draw() {
        displayTexture.clear(sf::Color(20, 20, 30, 230));

        displayTexture.draw(title);
        for (int i = 0; i < buttons.size(); i++) {
            buttons[i]->draw(displayTexture);
        }

        displayTexture.display();

        // Update sprite with texture
        displaySprite.setTexture(displayTexture.getTexture());
    }

    sf::Sprite& getSprite() {
        return displaySprite;
    }

    void centerOn(float x, float y) {
        displaySprite.setOrigin(Constants::PAUSE_DISPLAY_WIDTH / 2, Constants::PAUSE_DISPLAY_HEIGHT / 2);
        displaySprite.setPosition(x, y);
    }
};

#endif // PAUSE_MENU_H