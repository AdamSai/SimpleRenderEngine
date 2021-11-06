#include "Collidable.hpp"

Collidable::~Collidable() {
}

float Collidable::getRadius() const {
    return radius;
}

void Collidable::setRadius(float radius) {
    Collidable::radius = radius;
}
