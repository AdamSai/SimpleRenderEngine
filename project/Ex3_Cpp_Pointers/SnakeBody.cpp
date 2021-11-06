//
// Created by Morten Nobel-Jørgensen on 04/09/2017.
// Updated by Henrique in 2021.
//

#include "SnakeBody.hpp"

int SnakeBody::totalSnakeBodies = 0;

SnakeBody::SnakeBody(int x, int y) {
    this->x = x;
    this->y = y;

    // incremented every time a new SnakeBody is created
    totalSnakeBodies++;
}

SnakeBody::~SnakeBody() {
    // decremented every time a new SnakeBody is created
    totalSnakeBodies--;

    // TODO ensure there is no memory leak when using raw pointers
    if(this->next != nullptr)
        delete this->next;

}

void SnakeBody::moveTo(int x, int y) {
    // TODO may modify to solve ex 3.2 (move all body parts of the snake).
    //  Alternatively, you can solve that in the method Snake::move()

    // set its own position
    this->x = x;
    this->y = y;
}
