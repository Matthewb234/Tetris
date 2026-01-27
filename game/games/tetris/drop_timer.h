#ifndef TETRIS_DROP_TIMER_H
#define TETRIS_DROP_TIMER_H
#include <chrono>

class DropTimer {
private:
    std::chrono::steady_clock::time_point lastDrop;
    int dropInterval; // milliseconds

public:
    DropTimer(int fallSpeed): dropInterval(1000/fallSpeed) {
        reset();
    }

    void reset() {
        lastDrop = std::chrono::steady_clock::now();
    }

    // Check if it's time to drop
    bool shouldDrop() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            now - lastDrop
        ).count();

        if (elapsed >= dropInterval) {
            lastDrop = now; // Reset for next drop
            return true;
        }
        return false;
    }

    // Speed up as levels increase
    void setDropInterval(int milliseconds) {
        dropInterval = milliseconds;
    }

    int getDropInterval() const {
        return dropInterval;
    }
};

#endif //TETRIS_DROP_TIMER_H