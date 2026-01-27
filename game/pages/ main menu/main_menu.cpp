//
// Created by Matthew on 1/24/2026.
//

#include "main_menu.h"

#include <iostream>


MainMenu::MainMenu() {
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        // Handle font loading error
    }

    title.setFont(font);
    title.setString("TETRIS");
    title.setCharacterSize(80);
    title.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(Constants::WINDOW_WIDTH / 2 - titleBounds.width / 2, Constants::WINDOW_HEIGHT * .3 - titleBounds.height / 2);

    std::vector<std::string> labels = {"Play", "Settings", "Quit"};

    for (int i = labels.size() - 1; i >= 0; i--) {
        Button button(
            Constants::WINDOW_WIDTH / 2,
            Constants::WINDOW_HEIGHT * (.55 + (i * .15)),
            Constants::WINDOW_WIDTH / 3,
            Constants::WINDOW_HEIGHT / 10,
            labels[i],
            font
        );
        buttons.push_back(button);
    }
}

void MainMenu::drawMenuContent() {
    renderTexture.clear(sf::Color(20, 20, 30, 230));

    renderTexture.draw(title);
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].draw(renderTexture);
    }
}

void MainMenu::update(sf::Vector2i mousePos) {
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i].update(mousePos);
    }
}

int MainMenu::handleClick(sf::Vector2i mousePos) {
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i].isClicked(mousePos)) {
            return i;
        }
    }
}
