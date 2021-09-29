#pragma once

#include <chrono>

#include "GameObject.hpp"
#include "Collidable.hpp"
#include "AsteroidsGame.hpp"

class SpaceShip : public GameObject, public Collidable
{
public:
	SpaceShip(const sre::Sprite& sprite, AsteroidsGame* game);

	void update(float deltaTime) override;
	void onCollision(std::shared_ptr<GameObject> other) override;
	void onKey(SDL_Event& keyEvent) override;
	void Laser(glm::vec2 pos, glm::vec2 vel, float rot);

private:
	bool rotateCW = false;
	bool rotateCCW = false;
	bool thrust = false;

	float drag = 0.80f;
	float maxSpeed = 460.0f;
	float thrustPower = 600.0f;
	float rotationSpeed = 100.0f;
	float shootCooldown = 0.4f;

	long long lastShotTime;

	glm::vec2 velocity;
	glm::vec2 winSize;

	AsteroidsGame* game;
};


