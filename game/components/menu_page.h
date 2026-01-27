//
// Created by Matthew on 1/24/2026.
//

#ifndef TETRIS_MENU_PAGE_H
#define TETRIS_MENU_PAGE_H

#pragma once
#include <SFML/Graphics.hpp>

#include "../constants.h"

class MenuPage {
protected:
    sf::RenderTexture renderTexture;
    virtual void drawMenuContent() = 0;
public:
    MenuPage() {
        renderTexture.create(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
    }
    virtual ~MenuPage() = default;
    virtual int handleClick(sf::Vector2i mousePos) = 0;
    virtual void update(sf::Vector2i mousePos) = 0;
    virtual sf::Vector2f textureSize() {return sf::Vector2f(renderTexture.getSize());}
    virtual void draw(sf::RenderTarget& target) {
        renderTexture.clear(sf::Color(20, 20, 30));
        drawMenuContent();

        renderTexture.display();

        sf::Sprite menuSprite(renderTexture.getTexture());
        // boardSprite.setPosition(board->getPosition());
        target.draw(menuSprite);
    }
};
#endif //TETRIS_MENU_PAGE_H