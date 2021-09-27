#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

class Laser : public GameObject, public Collidable
{
public:
	Laser(const sre::Sprite &sprite, glm::vec2 position, float rotation);
	void update(float deltaTime) override;
	void onCollision(std::shared_ptr<GameObject> other) override;
	virtual ~Laser();
	time_t spawnTime;


private:
    glm::vec2 winSize;
	glm::vec2 velocity;



};