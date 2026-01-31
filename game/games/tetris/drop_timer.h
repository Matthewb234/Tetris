#ifndef TETRIS_DROP_TIMER_H
#define TETRIS_DROP_TIMER_H
#include <chrono>
#include <iostream>
#include <ostream>

class DropTimer {
private:
    std::chrono::steady_clock::time_point lastDrop;
    double dropInterval;

    std::chrono::steady_clock::time_point lockDelayStart{};
    int lockResets = 0;

public:
    DropTimer() {
        updateDropInterval(1);
        resetTimers();
    }

    void resetTimers(bool resetDrop = true) {
        if (resetDrop) lastDrop = std::chrono::steady_clock::now();
        else if (lockResets <= 15 && lockDelayStart != std::chrono::steady_clock::time_point()) {
            lockDelayStart = std::chrono::steady_clock::time_point();
            lockResets++;
        }
    }

    bool shouldLock() {
        if (lockDelayStart == std::chrono::steady_clock::time_point()) {
            lockDelayStart = std::chrono::steady_clock::now();
        }
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lockDelayStart
        ).count();

        if (elapsed >= 500) {
            lockDelayStart = std::chrono::steady_clock::time_point();
            lockResets = 0;
            return true;
        }
        return false;
    }

    bool shouldDrop() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastDrop
        ).count();

        if (elapsed >= dropInterval) {
            lastDrop = now;
            return true;
        }
        return false;
    }

    void updateDropInterval(int level) {
        dropInterval = 1000 * pow(0.8 - ((level - 1) * 0.007), level - 1);
    }
};

#endif //TETRIS_DROP_TIMER_H