#pragma once

#include "SnakeGUI.hpp"

class SnakeBody {
public:
    SnakeBody(int x, int y);
    ~SnakeBody();

    void moveTo(int x, int y);

private:
    SnakeBody* next = nullptr;
    int x;
    int y;
    // totalSnakeBodies is a static counter of instances
    // e.g. it is stored in the exe with the global variables, and
    // we have only one (shared) instance of it at runtime
    static int totalSnakeBodies;
    // Snake and SnakeGUI have direct access to private member variables with the friend qualifier
    friend class Snake;
    friend class SnakeGUI;
};

