#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

class Asteroid : public GameObject, public Collidable
{
public:
	enum class AsteroidSize
	{
		Small,
		Medium,
		Big
	};
	Asteroid(const sre::Sprite& sprite, AsteroidsGame* game, AsteroidSize size, glm::vec2 pos);
	void initializeValues(AsteroidsGame* game, AsteroidSize size);
	Asteroid(const sre::Sprite& sprite, AsteroidsGame* game, AsteroidSize size);
	void update(float deltaTime) override;
	void onCollision(std::shared_ptr<GameObject> other) override;
	AsteroidSize size;


private:
	float rotationSpeed;
	AsteroidsGame* game;

	glm::vec2 winSize;
	glm::vec2 velocity;


};