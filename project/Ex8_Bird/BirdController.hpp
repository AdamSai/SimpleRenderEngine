#pragma once

#include "Component.hpp"

class BirdController : public Component
{
public:
	explicit BirdController(GameObject* gameObject);

	bool onKey(SDL_Event& event) override;

	void onCollisionStart(PhysicsComponent* comp) override;

	void onCollisionEnd(PhysicsComponent* comp) override;

private:
	float jumpForce = 0.15f;
	bool canJump = false;
	std::shared_ptr<PhysicsComponent> physicsComponent;
};
