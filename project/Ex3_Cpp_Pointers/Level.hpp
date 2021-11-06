#pragma once

#include <vector>

class Level {
public:
    void init(int width, int height);
    bool isBlocked(int x, int y) const;
    bool snakeCollide(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

private:
    std::vector<std::vector<int>> blocks;  // 0 = free, 1 = blocked
    int width;
    int height;
    friend class SnakeGUI;
};
