#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

class Asteroid : public GameObject, public Collidable
{
public:
	int id;
	enum asteroidSize
	{
		Small,
		Medium,
		Big
	};
	Asteroid(const sre::Sprite& sprite, AsteroidsGame* game, asteroidSize size, glm::vec2 pos = glm::vec2(-1,-1));
	void update(float deltaTime) override;
	void onCollision(std::shared_ptr<GameObject> other) override;
	asteroidSize size;


private:
	static int nextId;
	float rotationSpeed;
	AsteroidsGame* game;

	glm::vec2 winSize;
	glm::vec2 velocity;


};