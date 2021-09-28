#pragma once

#include <vector>

#include "Asteroid.hpp"
#include "Laser.hpp"
#include "sre/SDLRenderer.hpp"
#include "sre/SpriteAtlas.hpp"

class GameObject;

class AsteroidsGame {
public:
    AsteroidsGame();
    void fireLaser(glm::vec2 pos, glm::vec2 vel, float rot);
    void destroyLaser(Laser* laser);
    void destroyAsteroid(Asteroid* asteroid);
    void spawnAsteroid(Asteroid::asteroidSize, glm::vec2 pos);
    void destroySpaceShip();

private:
    void update(float deltaTime);
    void render();
    void keyEvent(SDL_Event &event);
    void restartGame();


    sre::Camera camera;
    sre::SDLRenderer r;
    std::shared_ptr<sre::SpriteAtlas> atlas;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    bool debugCollisionCircles = false;
    bool playerIsAlive = true;
    int score = 0;
};


