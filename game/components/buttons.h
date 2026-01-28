#ifndef TETRIS_BUTTON_H
#define TETRIS_BUTTON_H

#include <iostream>
#include <SFML/Graphics.hpp>

#include "rounded_rectangle.h"

class Button {
protected:  // Changed from private so derived classes can access
    sf::RectangleShape shape;
    sf::Text text;
    sf::Font& font;
    bool isHovered;

public:
    Button(float x, float y, float width, float height, const std::string& label, sf::Font& font, int fontSize = 24)
        : font(font), isHovered(false) {

        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(x - (width/2), y - (height/2));
        shape.setFillColor(sf::Color(70, 70, 70));
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::White);

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(fontSize);
        text.setFillColor(sf::Color::White);

        // Center text in button
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            x - (textBounds.width/2),
            y  - (textBounds.height/2) - 10
        );
    }

    virtual ~Button() = default;  // Virtual destructor for inheritance

    void setText(const std::string& newText) {
        text.setString(newText);

        sf::FloatRect textBounds = text.getLocalBounds();
        sf::Vector2f center = shape.getPosition() + shape.getSize() / 2.f;
        text.setPosition(
            center.x - (textBounds.width/2),
            center.y - (textBounds.height/2) - 5
        );
    }

    virtual void update(sf::Vector2i mousePos) {
        isHovered = shape.getGlobalBounds().contains(mousePos.x, mousePos.y);
    }

    virtual bool isClicked(sf::Vector2i mousePos) {
        return isHovered && shape.getGlobalBounds().contains(mousePos.x, mousePos.y);
    }

    virtual void draw(sf::RenderTexture& texture) {
        if (isHovered) {
            shape.setFillColor(sf::Color(100, 100, 100));
        } else {
            shape.setFillColor(sf::Color(70, 70, 70));
        }
        texture.draw(shape);
        texture.draw(text);
    }
};

class TransparentButton : public Button {
    bool isHoverable;
public:
    TransparentButton(float x, float y, sf::Font& font,
        const std::string& label = "", bool isHoverable = true, int fontSize = 24)
        : Button(x, y, 0, 0, label, font, fontSize), isHoverable(isHoverable) {

        float width = text.getLocalBounds().width;
        float height = text.getLocalBounds().height;

        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(x - (width/2), y - (height/2));
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineThickness(0);

        sf::FloatRect textBounds = text.getLocalBounds();
        text.setPosition(
            x - (textBounds.width/2),
            y - (textBounds.height/2) - 5
        );
    }

    void draw(sf::RenderTexture& texture) override {
        if (isHovered && isHoverable) {
            text.setFillColor(sf::Color(200, 200, 200));
        } else {
            text.setFillColor(sf::Color::White);
        }
        texture.draw(shape);
        texture.draw(text);
    }
};

#endif //TETRIS_BUTTON_H