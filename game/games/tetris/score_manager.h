//
// Created by Matthew on 1/28/2026.
//

#ifndef TETRIS_SCORE_MANAGER_H
#define TETRIS_SCORE_MANAGER_H

namespace BaseScores {
    constexpr int SINGLE_LINE = 100;
    constexpr int DOUBLE_LINE = 300;
    constexpr int TRIPLE_LINE = 500;
    constexpr int TETRIS = 800;
    constexpr int MINI_T_SPIN = 100;
    constexpr int MINI_T_SPIN_SINGLE = 200;
    constexpr int MINI_T_SPIN_DOUBLE = 400;
    constexpr int T_SPIN = 400;
    constexpr int T_SPIN_SINGLE = 800;
    constexpr int T_SPIN_DOUBLE = 1200;
    constexpr int T_SPIN_TRIPLE = 1600;
};

class ScoreManager {
    int score = 0;
    int lines = 0;
    int level = 1;

    bool b2b = false;
public:
    void addDropScore(int cells, bool isHardDrop) {
        score += cells * (isHardDrop ? 2 : 1);
    }
    void addTSpinScore(bool isMini, int linesCleared) {
        if (isMini) {
            switch (linesCleared) {
                case 0:
                    incrementScore(BaseScores::MINI_T_SPIN);
                    break;
                case 1:
                    incrementScore(BaseScores::MINI_T_SPIN_SINGLE);
                    break;
                case 2:
                    incrementScore(BaseScores::MINI_T_SPIN_DOUBLE);
                    break;
            }
            b2b = false;
        } else {
            switch (linesCleared) {
                case 0:
                    incrementScore(BaseScores::T_SPIN, b2b);
                    break;
                case 1:
                    incrementScore(BaseScores::T_SPIN_SINGLE, b2b);
                    break;
                case 2:
                    incrementScore(BaseScores::T_SPIN_DOUBLE, b2b);
                    break;
                case 3:
                    incrementScore(BaseScores::T_SPIN_TRIPLE, b2b);
                    break;
            }
            b2b = true;
        }
    }
    void addLineScore(int linesCleared) {
        switch (linesCleared) {
            case 1:
                incrementScore(BaseScores::SINGLE_LINE);
                break;
            case 2:
                incrementScore(BaseScores::DOUBLE_LINE);
                break;
            case 3:
                incrementScore(BaseScores::TRIPLE_LINE);
                break;
            case 4:
                incrementScore(BaseScores::TETRIS, b2b);
                b2b = true;
                return;
        }
        b2b = false;
    }
    void incrementScore(int addedScore, bool b2b = false) {
        score += (addedScore * level) * (b2b ? 1.5 : 1);
    }
    void incrementLines(int addedLines) {
        lines += addedLines;
        level = level == 15 ? 15 : 1 + (lines / 10);
        addLineScore(addedLines);
    }

    int getScore() { return score; }
    int getLines() { return lines; }
    int getLevel() { return level; }
};

class ScoreDisplay {
    sf::RenderTexture displayTexture;
    sf::RenderTexture staticTexture;
    sf::Sprite displaySprite;

    sf::Font font;
    std::vector<sf::Text> scoreTexts;
    std::vector<sf::Text> labels;

    ScoreManager scoreManager;
public:
    ScoreDisplay(float x, float y) : scoreManager(ScoreManager()){
        displayTexture.create(TetrisConstants::DISPLAY_WIDTH * .8, TetrisConstants::GRID_HEIGHT * .35);
        staticTexture.create(TetrisConstants::DISPLAY_WIDTH * .8, TetrisConstants::GRID_HEIGHT * .35);

        if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
            // Handle font loading error
        }

        drawStaticElements();

        for (int i = 0; i < 3; i++) {
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::White);
            scoreTexts.push_back(text);
        }

        displaySprite.setTexture(displayTexture.getTexture());
        displaySprite.setOrigin(
            displaySprite.getLocalBounds().width,
            displaySprite.getLocalBounds().height
        );
        displaySprite.setPosition(sf::Vector2f(x, y));
    }

    void drawStaticElements() {
        staticTexture.clear(sf::Color::Transparent);

        std::vector<std::string> labels = {"Score", "Level", "Lines"};

        for (int i = 0; i < labels.size(); i++) {
            sf::RectangleShape labelBox(sf::Vector2f(
                staticTexture.getSize().x,
                staticTexture.getSize().y/8
            ));
            labelBox.setPosition(0, (staticTexture.getSize().y/3) * i);
            labelBox.setFillColor(sf::Color(100, 100, 100));
            labelBox.setOutlineThickness(1);
            labelBox.setOutlineColor(sf::Color::White);
            staticTexture.draw(labelBox);

            sf::Text label;
            label.setFont(font);
            label.setString(labels[i]);
            label.setCharacterSize(20);
            label.setFillColor(sf::Color::White);
            label.setPosition(10, (staticTexture.getSize().y/3) * i + 2);
            staticTexture.draw(label);
        }

        sf::RectangleShape displayFrame(static_cast<sf::Vector2f>(staticTexture.getSize()));
        displayFrame.setFillColor(sf::Color::Transparent);
        displayFrame.setOutlineColor(sf::Color::White);
        displayFrame.setOutlineThickness(-3);
        staticTexture.draw(displayFrame);

        staticTexture.display();
    }

    void updateDisplay() {
        displayTexture.clear(sf::Color::Transparent);

        sf::Sprite staticSprite(staticTexture.getTexture());
        displayTexture.draw(staticSprite);

        scoreTexts[0].setString(std::to_string(scoreManager.getScore()));
        scoreTexts[1].setString(std::to_string(scoreManager.getLevel()));
        scoreTexts[2].setString(std::to_string(scoreManager.getLines()));
        for (int i = 0; i < scoreTexts.size(); i++) {
            scoreTexts[i].setPosition(sf::Vector2f(
                displayTexture.getSize().x - scoreTexts[i].getLocalBounds().width - 10,
                (displayTexture.getSize().y / 3 * i) + displayTexture.getSize().y / 8
            ));
            displayTexture.draw(scoreTexts[i]);
        }

        displayTexture.display();
    }

    sf::Sprite& getSprite() {return displaySprite;}
    ScoreManager& getScoreManager() { return scoreManager; }
};
#endif //TETRIS_SCORE_MANAGER_H