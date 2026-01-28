//
// Created by Matthew on 1/26/2026.
//

#include "settings.h"

#include "../../application_context.h"

Settings::Settings() {
    if (!font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        // Handle font loading error
    }

    title.setFont(font);
    title.setString("SETTINGS");
    title.setCharacterSize(70);
    title.setFillColor(sf::Color::White);
    sf::FloatRect titleBounds = title.getLocalBounds();
    title.setPosition(Constants::WINDOW_WIDTH / 2 - titleBounds.width / 2, Constants::WINDOW_HEIGHT * .1 - titleBounds.height / 2);

    overlay.setSize(sf::Vector2f(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));

    rebindPromptOne.setFont(font);
    rebindPromptOne.setCharacterSize(40);
    rebindPromptOne.setFillColor(sf::Color::White);
    rebindPromptTwo.setFont(font);
    rebindPromptTwo.setCharacterSize(40);
    rebindPromptTwo.setFillColor(sf::Color::White);

    auto backBtn = std::make_unique<TransparentButton>(
        Constants::WINDOW_WIDTH * .05,
        Constants::WINDOW_HEIGHT * .025,
        font,
        "BACK",
        35
    );
    buttons[""] = std::move(backBtn);

    auto& input = ApplicationContext::get().getInputManager();
    auto& bindings = input.getActiveBindings();
    size_t i = 0;

    for (const auto& [action, keys] : bindings) {
        auto button = std::make_unique<Button>(
            Constants::WINDOW_WIDTH / 2,
            Constants::WINDOW_HEIGHT * (.25 + (i * .1)),
            Constants::WINDOW_WIDTH / 3,
            Constants::WINDOW_HEIGHT / 15,
            action + ": " + input.getKeyDisplayString(input.getActiveContext(), action),
            font
        );
        buttons[action] = std::move(button);
        i++;
    }
}

void Settings::drawMenuContent() {
    renderTexture.clear(sf::Color(20, 20, 30, 230));

    renderTexture.draw(title);
    for (auto& [action, button] : buttons) {
        button->draw(renderTexture);
    }

    if (waitingForRebind) {
        renderTexture.draw(overlay);
        renderTexture.draw(rebindPromptOne);
        renderTexture.draw(rebindPromptTwo);
    }
}

void Settings::update(sf::Vector2i mousePos) {
    if (waitingForRebind) return;

    for (auto& [action, button] : buttons) {
        button->update(mousePos);
    }
}

void Settings::rebindKey(const std::string& action) {
    waitingForRebind = action;
    rebindPromptOne.setString("Press any key to bind to: " + action);
    sf::FloatRect promptOneBounds = rebindPromptOne.getLocalBounds();
    rebindPromptOne.setPosition(
        Constants::WINDOW_WIDTH / 2 - promptOneBounds.width / 2,
        Constants::WINDOW_HEIGHT * .45 - promptOneBounds.height / 2
    );
    rebindPromptTwo.setString("Press ESC to cancel");
    sf::FloatRect promptTwoBounds = rebindPromptTwo.getLocalBounds();
    rebindPromptTwo.setPosition(
        Constants::WINDOW_WIDTH / 2 - promptTwoBounds.width / 2,
        Constants::WINDOW_HEIGHT * .55 - promptTwoBounds.height / 2
    );
}

void Settings::handleKeyPress(sf::Keyboard::Key key) {
    if (!waitingForRebind) return;

    auto& inputManager = ApplicationContext::get().getInputManager();

    if (key == sf::Keyboard::Escape) {
        waitingForRebind = std::nullopt;
        return;
    }

    if (!inputManager.getActiveContext()->rebind(*waitingForRebind, key)) {
        return;
    }

    buttons[*waitingForRebind]->setText(*waitingForRebind + ": " + inputManager.getKeyDisplayString(inputManager.getActiveContext(), *waitingForRebind));

    waitingForRebind = std::nullopt;
}

void Settings::handleClick(sf::Vector2i mousePos) {
    if (waitingForRebind) return;
    auto& appCtx = ApplicationContext::get();

    for (auto& [action, button] : buttons) {
        if (button->isClicked(mousePos)) {
            if (action.empty()) {
                std::cout << "Back Button Hit" << std::endl;
                appCtx.getPageManager().switchState(PageState::MAIN_MENU);
            } else {
                // Rebind button
                std::cout << "Rebinding action: " << action << std::endl;
                rebindKey(action);
            }
            return;
        }
    }
}