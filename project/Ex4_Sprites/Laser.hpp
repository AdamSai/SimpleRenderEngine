#pragma once

#include "GameObject.hpp"
#include "Collidable.hpp"

class Laser : public GameObject, public Collidable
{
public:
	Laser(int id, const sre::Sprite &sprite, glm::vec2 position, glm::vec2 vel, float rotation, AsteroidsGame* game);
	int id;
	void update(float deltaTime) override;
	void onCollision(std::shared_ptr<GameObject> other) override;
	time_t spawnTime;
	AsteroidsGame* game;


private:
    glm::vec2 winSize{};
	glm::vec2 velocity{};



};