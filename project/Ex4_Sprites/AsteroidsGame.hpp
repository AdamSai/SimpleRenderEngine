#pragma once

#include <chrono>
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
    void destroyGameObject(GameObject* gameObject);
    void destroyAsteroid(Asteroid* asteroid);
    void spawnAsteroid(Asteroid::AsteroidSize, glm::vec2 pos);
    void destroySpaceShip(GameObject* gameObject);
    long long GetCurrentTimeMillis() const;
    static float getRandomFloat(float min, float max);

private:
    void update(float deltaTime);
    void render();
    void keyEvent(SDL_Event &event);
    void generateStars();
    void restartGame();


    sre::Camera camera;
    sre::SDLRenderer r;
    std::shared_ptr<sre::SpriteAtlas> atlas;
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::vector<std::shared_ptr<GameObject>> backgroundSprites;
    bool debugCollisionCircles = false;
    bool playerIsAlive = true;
    int score = 0;
};


