#ifndef TETRIS_BUTTON_H
#define TETRIS_BUTTON_H

#include <iostream>
#include <SFML/Graphics.hpp>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font& font;
    bool isHovered;

public:
    Button(float x, float y, float width, float height,
           const std::string& label, sf::Font& font)
        : font(font), isHovered(false) {

        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(x - (width/2), y - (height/2));
        shape.setFillColor(sf::Color(70, 70, 70));
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::White);

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);

        // Center text in button
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            x - (textBounds.width/2),
            y - (textBounds.height/2) - 5
        );
    }

    void update(sf::Vector2i mousePos) {
        isHovered = shape.getGlobalBounds().contains(mousePos.x, mousePos.y);
    }

    bool isClicked(sf::Vector2i mousePos) {
        return isHovered && shape.getGlobalBounds().contains(mousePos.x, mousePos.y);
    }

    void draw(sf::RenderTexture& texture) {
        if (isHovered) {
            shape.setFillColor(sf::Color(100, 100, 100));
        } else {
            shape.setFillColor(sf::Color(70, 70, 70));
        }
        texture.draw(shape);
        texture.draw(text);
    }
};
#endif //TETRIS_BUTTON_H