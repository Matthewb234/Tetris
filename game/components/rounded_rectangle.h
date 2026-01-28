//
// Created by Matthew on 1/27/2026.
//

#ifndef TETRIS_ROUNDED_RECTANGLE_H
#define TETRIS_ROUNDED_RECTANGLE_H

#include <SFML/Graphics.hpp>
#include <cmath>

class RoundedRectangleShape : public sf::Shape {
private:
    sf::Vector2f size;
    float radius;
    unsigned int cornerPoints;

public:
    RoundedRectangleShape(const sf::Vector2f& size = sf::Vector2f(0, 0),
                          float radius = 0,
                          unsigned int cornerPoints = 10)
        : size(size), radius(radius), cornerPoints(cornerPoints) {
        update();
    }

    void setSize(const sf::Vector2f& newSize) {
        size = newSize;
        update();
    }

    void setRadius(float newRadius) {
        radius = newRadius;
        update();
    }

    const sf::Vector2f& getSize() const {
        return size;
    }

    float getRadius() const {
        return radius;
    }

    virtual std::size_t getPointCount() const override {
        return cornerPoints * 4;
    }

    virtual sf::Vector2f getPoint(std::size_t index) const override {
        if (index >= cornerPoints * 4)
            return sf::Vector2f(0, 0);

        float deltaAngle = 90.0f / (cornerPoints - 1);
        sf::Vector2f center;
        unsigned int centerIndex = index / cornerPoints;
        static const float pi = 3.141592654f;

        switch (centerIndex) {
            case 0: center.x = size.x - radius; center.y = radius; break;
            case 1: center.x = radius; center.y = radius; break;
            case 2: center.x = radius; center.y = size.y - radius; break;
            case 3: center.x = size.x - radius; center.y = size.y - radius; break;
        }

        float angle = deltaAngle * (index - centerIndex * cornerPoints) + 270.0f + 90.0f * centerIndex;
        return sf::Vector2f(
            center.x + radius * std::cos(angle * pi / 180),
            center.y + radius * std::sin(angle * pi / 180)
        );
    }
};

#endif //TETRIS_ROUNDED_RECTANGLE_H