//
// Created by Morten Nobel-Jørgensen on 19/10/2017.
//

#include <SDL_events.h>
#include <iostream>
#include "BirdController.hpp"
#include "GameObject.hpp"
#include "SpriteComponent.hpp"
#include "PhysicsComponent.hpp"
#include "BirdGame.hpp"
#include "SpriteComponent.hpp"

BirdController::BirdController(GameObject* gameObject) : Component(gameObject)
{
	// initiate bird physics
	physicsComponent = gameObject->getComponent<PhysicsComponent>();
	physicsComponent->setLinearVelocity(glm::vec2(1, 0));
}

bool BirdController::onKey(SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_SPACE && canJump)
		{
			canJump = false;
			physicsComponent->addImpulse(glm::vec2(0, 1 * jumpForce));
			std::cout << gameObject->getPosition().x << std::endl;
		}
	}
	else if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			canJump = true;
		}
	}
	return false;
}

void BirdController::onCollisionStart(PhysicsComponent* comp)
{
	std::cout << "bird collided with something" << std::endl;
	if (!comp->isSensor())
	{
		BirdGame::instance->setGameState(GameState::GameOver);
	}
}

void BirdController::onCollisionEnd(PhysicsComponent* comp)
{

}


