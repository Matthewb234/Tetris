//
// Created by Matthew on 1/26/2026.
//

#ifndef TETRIS_PAGE_STATE_MANAGER_H
#define TETRIS_PAGE_STATE_MANAGER_H
#include <memory>
#include <SFML/Graphics.hpp>

#include "../components/menu_page.h"
#include "../pages/mainMenu/main_menu.h"
#include "../pages/settings/settings.h"

enum class PageState {
    MAIN_MENU,
    SETTINGS,
    GAME,
};

class PageStateManager {
private:
    PageState state;
    std::unique_ptr<MenuPage> currentPage;
    sf::RenderTexture pageTexture;
    sf::Sprite pageSprite;

public:
    explicit PageStateManager() {
        switchState(PageState::MAIN_MENU);
    }

    MenuPage& getCurrentPage() { return *currentPage; }
    bool mainMenu() { return state == PageState::MAIN_MENU; }
    bool settings() { return state == PageState::SETTINGS; }
    bool inGame() { return state == PageState::GAME; }

    void switchState(PageState newState) {
        state = newState;
        switch (newState) {
            case PageState::MAIN_MENU:
                currentPage = std::make_unique<MainMenu>();
                break;
            case PageState::SETTINGS:
                currentPage = std::make_unique<Settings>();
                break;
            case PageState::GAME:
                currentPage = nullptr;
                return;
        }
        pageTexture.create(currentPage->textureSize().x, currentPage->textureSize().y);
    }

    void update(sf::Vector2i mousePos) {
        if (state != PageState::GAME) {
            currentPage->update(mousePos);
        }
    }

    void draw() {
        if (state != PageState::GAME) {
            pageTexture.clear(sf::Color::Transparent);
            currentPage->draw(pageTexture);
            pageTexture.display();

            // Update sprite with the texture
            pageSprite.setTexture(pageTexture.getTexture());
            pageSprite.setOrigin(currentPage->textureSize()/2.f);
        }
    }

    sf::Sprite& getSprite() {
        return pageSprite;
    }

    void handleClick(sf::Vector2i mousePos) {
        currentPage->handleClick(mousePos);
    }
};
#endif //TETRIS_PAGE_STATE_MANAGER_H