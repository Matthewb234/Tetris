#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class PauseDisplay {
private:
    sf::RenderTexture displayTexture;
    sf::Sprite displaySprite;
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonTexts;
    sf::Font font;
    int hoveredButton;

    // Menu dimensions
    const float MENU_WIDTH = 300;
    const float MENU_HEIGHT = 400;

public:
    PauseDisplay() : hoveredButton(-1) {
        // Create the render texture
        displayTexture.create(MENU_WIDTH, MENU_HEIGHT);

        // Load font (adjust path as needed)
        if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            // Handle font loading error
        }

        // Create buttons
        std::vector<std::string> labels = {"Resume", "Restart", "Quit"};

        for (size_t i = 0; i < labels.size(); i++) {
            // Button shape
            sf::RectangleShape button;
            button.setSize(sf::Vector2f(200, 60));
            button.setPosition(50, 80 + i * 90);
            button.setFillColor(sf::Color(50, 50, 50));
            button.setOutlineColor(sf::Color::White);
            button.setOutlineThickness(2);
            buttons.push_back(button);

            // Button text
            sf::Text text;
            text.setFont(font);
            text.setString(labels[i]);
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::White);

            // Center text in button
            sf::FloatRect textBounds = text.getLocalBounds();
            text.setPosition(
                150 - textBounds.width / 2,
                105 + i * 90 - textBounds.height / 2
            );
            buttonTexts.push_back(text);
        }
    }

    void update(sf::Vector2i mousePos) {
        // Use inverse transform to convert window coords to menu coords
        sf::Transform inverse = displaySprite.getInverseTransform();
        sf::Vector2f localMousePos = inverse.transformPoint(static_cast<sf::Vector2f>(mousePos));

        hoveredButton = -1;
        for (size_t i = 0; i < buttons.size(); i++) {
            if (buttons[i].getGlobalBounds().contains(localMousePos)) {
                hoveredButton = i;
                buttons[i].setFillColor(sf::Color(80, 80, 80));
            } else {
                buttons[i].setFillColor(sf::Color(50, 50, 50));
            }
        }
    }

    int handleClick(sf::Vector2i mousePos) {
        // Use inverse transform to convert window coords to menu coords
        sf::Transform inverse = displaySprite.getInverseTransform();
        sf::Vector2f localMousePos = inverse.transformPoint(static_cast<sf::Vector2f>(mousePos));

        for (size_t i = 0; i < buttons.size(); i++) {
            if (buttons[i].getGlobalBounds().contains(localMousePos)) {
                return i;
            }
        }
        return -1;
    }

    void draw() {
        // Draw to the texture
        displayTexture.clear(sf::Color(20, 20, 30, 230)); // Semi-transparent background

        // Draw title
        sf::Text title;
        title.setFont(font);
        title.setString("PAUSED");
        title.setCharacterSize(36);
        title.setFillColor(sf::Color::White);
        sf::FloatRect titleBounds = title.getLocalBounds();
        title.setPosition(MENU_WIDTH / 2 - titleBounds.width / 2, 20);
        displayTexture.draw(title);

        // Draw buttons
        for (size_t i = 0; i < buttons.size(); i++) {
            displayTexture.draw(buttons[i]);
            displayTexture.draw(buttonTexts[i]);
        }

        displayTexture.display();

        // Update sprite with texture
        displaySprite.setTexture(displayTexture.getTexture());
    }

    sf::Sprite& getSprite() {
        return displaySprite;
    }

    void centerOn(float x, float y) {
        displaySprite.setOrigin(MENU_WIDTH / 2, MENU_HEIGHT / 2);
        displaySprite.setPosition(x, y);
    }
};

#endif // PAUSE_MENU_H