//
// Created by Matthew on 1/26/2026.
//

#ifndef TETRIS_SETTINGS_H
#define TETRIS_SETTINGS_H

#include <memory>
#include <optional>
#include "../../components/buttons.h"
#include "../../components/menu_page.h"

class Settings : public MenuPage {
    std::map<std::string, std::unique_ptr<Button>> buttons;
    sf::Text title;

    std::optional<std::string> waitingForRebind;  // Which action is waiting for a key
    sf::Text rebindPromptOne;
    sf::Text rebindPromptTwo;
    sf::RectangleShape overlay;

    void drawMenuContent() override;
public:
    Settings();
    void update(sf::Vector2i mousePos) override;
    void handleClick(sf::Vector2i mousePos) override;
    void handleKeyPress(sf::Keyboard::Key key);
    void rebindKey(const std::string& action);
    bool isWaitingForRebind() const { return waitingForRebind.has_value(); }
};

#endif //TETRIS_SETTINGS_H