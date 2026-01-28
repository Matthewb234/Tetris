//
// Created by Matthew on 1/24/2026.
//

#include "main_menu.h"
#include "../../application_context.h"

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

    for (int i = 0; i < labels.size(); i++) {
        buttons.push_back(std::make_unique<Button>(
            Constants::WINDOW_WIDTH / 2,
            Constants::WINDOW_HEIGHT * (.5 + (i * .15)),
            Constants::WINDOW_WIDTH / 3,
            Constants::WINDOW_HEIGHT / 10,
            labels[i],
            font,
            40
        ));
    }
}

void MainMenu::drawMenuContent() {
    renderTexture.clear(sf::Color(20, 20, 30, 230));

    renderTexture.draw(title);
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i]->draw(renderTexture);
    }
}

void MainMenu::update(sf::Vector2i mousePos) {
    for (int i = 0; i < buttons.size(); i++) {
        buttons[i]->update(mousePos);
    }
}

void MainMenu::handleClick(sf::Vector2i mousePos) {
    for (int i = 0; i < buttons.size(); i++) {
        if (buttons[i]->isClicked(mousePos)) {
            auto& appCtx = ApplicationContext::get();
            switch(i) {
                case 0:
                    appCtx.getPageManager().switchState(PageState::GAME);
                    if (!appCtx.getGameManager().hasGameLoaded()) {
                        appCtx.getGameManager().loadGame(std::make_unique<Tetris>());
                        appCtx.getGameManager().startGame();
                    } else {
                        appCtx.getGameManager().startGame();

                    }
                    break;
                case 1:
                    appCtx.getPageManager().switchState(PageState::SETTINGS);
                    break;
                case 2:
                    appCtx.getWindow().close();
                    break;
            }
            return;
        }
    }
}
