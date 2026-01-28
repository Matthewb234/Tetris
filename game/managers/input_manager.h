//
// Created by Matthew on 1/26/2026.
//

#ifndef TETRIS_INPUT_MANAGER_H
#define TETRIS_INPUT_MANAGER_H
#include <map>
#include <memory>
#include <string>
#include <SFML/Window/Keyboard.hpp>

class InputContext {
protected:
    std::map<std::string, sf::Keyboard::Key> keyBindings;

public:
    virtual ~InputContext() = default;

    bool rebind (const std::string& action, sf::Keyboard::Key key) {
        for (const auto& [action, boundKey] : keyBindings) {
            if (boundKey == key) {
                return false;
            }
        }
        keyBindings[action] = key;
        return true;
    }

    sf::Keyboard::Key getKey (const std::string& action) const {
        auto binding = keyBindings.find(action);
        return (binding != keyBindings.end()) ? binding->second : sf::Keyboard::Unknown;
    }

    bool isPressed(const std::string& action, sf::Keyboard::Key pressedKey) const {
        auto binding = keyBindings.find(action);
        return (binding != keyBindings.end()) && (binding->second == pressedKey);
    }

    const std::map<std::string, sf::Keyboard::Key>& getBindings() const {
        return keyBindings;
    }
};

class TetrisControls : public InputContext {
public:
    TetrisControls() {
        // Default bindings
        keyBindings["Move Left"] = sf::Keyboard::Left;
        keyBindings["Move Right"] = sf::Keyboard::Right;
        keyBindings["Move Down"] = sf::Keyboard::Down;
        keyBindings["Rotate CW"] = sf::Keyboard::Up;
        keyBindings["Rotate CCW"] = sf::Keyboard::Z;
        keyBindings["Push Down"] = sf::Keyboard::Space;
        keyBindings["Store"] = sf::Keyboard::C;
    }
};

class InputManager {
protected:
    std::map<std::string, std::unique_ptr<InputContext>> contexts;
    InputContext* activeContext = nullptr;
public:
    InputManager() {
        registerContext("tetris", std::make_unique<TetrisControls>());

        auto globalContext = std::make_unique<InputContext>();
        registerContext("global", std::move(globalContext));
    }

    void registerContext(const std::string& name, std::unique_ptr<InputContext> context) {
        contexts[name] = std::move(context);
    }

    void setActiveContext(const std::string& name) {
        auto context = contexts.find(name);
        if (context != contexts.end()) {
            activeContext = context->second.get();
        }
    }

    InputContext* getContext(const std::string& name) {
        auto context = contexts.find(name);
        return (context != contexts.end()) ? context->second.get() : nullptr;
    }

    InputContext* getActiveContext() { return activeContext; }

    bool isPressed(const std::string& action, sf::Keyboard::Key key) const {
        return activeContext ? activeContext->isPressed(action, key) : false;
    }

    bool isGlobalPressed(const std::string& action, sf::Keyboard::Key key) const {
        auto context = contexts.find("global");
        return (context != contexts.end()) ? context->second->isPressed(action, key) : false;
    }

    const std::map<std::string, sf::Keyboard::Key>& getActiveBindings() const {
        static const std::map<std::string, sf::Keyboard::Key> emptyMap;
        return activeContext ? activeContext->getBindings() : emptyMap;
    }

    std::string getKeyDisplayString(const InputContext *context, const std::string& action) const {
        auto key = context->getKey(action);
        if (key == sf::Keyboard::Unknown) return "Unbound";
        return keyToString(key);
    }

private:
    // Helper to convert key enum to string for display
    std::string keyToString(sf::Keyboard::Key key) const {
        switch(key) {
            case sf::Keyboard::A: return "A";
            case sf::Keyboard::B: return "B";
            case sf::Keyboard::C: return "C";
            case sf::Keyboard::D: return "D";
            case sf::Keyboard::S: return "S";
            case sf::Keyboard::W: return "W";
            case sf::Keyboard::X: return "X";
            case sf::Keyboard::Z: return "Z";
            case sf::Keyboard::Up: return "Up";
            case sf::Keyboard::Down: return "Down";
            case sf::Keyboard::Left: return "Left";
            case sf::Keyboard::Right: return "Right";
            case sf::Keyboard::Space: return "Space";
            case sf::Keyboard::Enter: return "Enter";
            case sf::Keyboard::Escape: return "Escape";
            case sf::Keyboard::LShift: return "LShift";
            case sf::Keyboard::LControl: return "LCtrl";
            default: return "Unknown";
        }
    }
};
#endif //TETRIS_INPUT_MANAGER_H