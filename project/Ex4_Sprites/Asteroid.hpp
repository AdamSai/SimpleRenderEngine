#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

class Asteroid : public GameObject, public Collidable
{
public:
	Asteroid(const sre::Sprite &sprite);
	void update(float deltaTime) override;
	void onCollision(std::shared_ptr<GameObject> other) override;

private:
	int size;
	float rotationSpeed;

    glm::vec2 winSize;
	glm::vec2 velocity;


};