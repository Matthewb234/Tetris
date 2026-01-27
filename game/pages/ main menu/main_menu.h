//
// Created by Matthew on 1/24/2026.
//

#ifndef TETRIS_MAIN_MENU_H
#define TETRIS_MAIN_MENU_H

#include "../../components/menu_page.h"
#include "../../components/button.h"

class MainMenu : public MenuPage {
private:
    std::vector<Button> buttons;
    sf::Text title;
    sf::Font font;

    void drawMenuContent() override;
public:
    MainMenu();
    void update(sf::Vector2i mousePos) override;
    int handleClick(sf::Vector2i mousePos) override;
};
#endif //TETRIS_MAIN_MENU_H