#ifndef GAME_H
#define GAME_H

#pragma once
#include <iostream>

#include "gameboard/gameboard.h"

class Game {
protected:
    std::unique_ptr<GameBoard> board;
    virtual void drawGameContent() = 0;
public:
    virtual ~Game() = default;

    virtual void handleEvent(sf::Event event) = 0;
    virtual bool update() = 0;
    virtual sf::Vector2f textureSize() {return board->getSize();}
    virtual void draw(sf::RenderTarget& target) {
        board->clear(sf::Color(20, 20, 30));
        drawGameContent();

        board->display();

        sf::Sprite boardSprite(board->getRenderTexture().getTexture());
        boardSprite.setPosition(board->getPosition());
        target.draw(boardSprite);
    };
};
#endif //GAME_H