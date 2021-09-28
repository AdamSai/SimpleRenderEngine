#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "SpaceShip.hpp"

#include "Asteroid.hpp"
#include "AsteroidsGame.hpp"
#include "sre/Renderer.hpp"

SpaceShip::SpaceShip(const sre::Sprite& sprite, AsteroidsGame* game) : GameObject(sprite)
{
	scale = glm::vec2(0.5f, 0.5f);
	winSize = sre::Renderer::instance->getDrawableSize();
	radius = 23;
	position = winSize * 0.5f;
	velocity = glm::vec2(0.0f, 0.0f);
	this->game = game;
}

void SpaceShip::update(float deltaTime)
{
	if (thrust)
	{
		float acceleration = deltaTime * thrustPower;
		glm::vec2 direction = glm::rotateZ(glm::vec3(0, acceleration, 0), glm::radians(rotation));
		velocity += direction;
		float speed = glm::length(velocity);
		if (speed > maxSpeed)
		{
			velocity = velocity * (maxSpeed / speed);
		}
	}

	else
	{
		velocity = velocity * (1.0f - drag * deltaTime);
	}
	position += velocity * deltaTime;
	if (rotateCCW)
	{
		rotation += deltaTime * rotationSpeed;
	}
	if (rotateCW)
	{
		rotation -= deltaTime * rotationSpeed;
	}

	// wrap around
	if (position.x < 0)
	{
		position.x += winSize.x;
	}
	else if (position.x > winSize.x)
	{
		position.x -= winSize.x;
	}
	if (position.y < 0)
	{
		position.y += winSize.y;
	}
	else if (position.y > winSize.y)
	{
		position.y -= winSize.y;
	}
}

void SpaceShip::onCollision(std::shared_ptr<GameObject> other)
{
	auto asteroid = std::dynamic_pointer_cast<Asteroid>(other);
	if(asteroid != nullptr)
	{
		// Spawn Bang and kill player
	}
}

void SpaceShip::Laser(glm::vec2 pos, glm::vec2 vel, float rot)
{
	game->fireLaser(pos, vel, rot);
}
void SpaceShip::onKey(SDL_Event& keyEvent)
{
	if (keyEvent.key.keysym.sym == SDLK_UP)
	{
		thrust = keyEvent.type == SDL_KEYDOWN;
	}
	if (keyEvent.key.keysym.sym == SDLK_LEFT)
	{
		rotateCCW = keyEvent.type == SDL_KEYDOWN;
	}
	if (keyEvent.key.keysym.sym == SDLK_RIGHT)
	{
		rotateCW = keyEvent.type == SDL_KEYDOWN;
	}
	if (keyEvent.key.keysym.sym == SDLK_SPACE && (float)(time(nullptr) - lastShotTime) >= shootCooldown) 
	{
		lastShotTime = time(nullptr);
		glm::vec2 direction = glm::rotateZ(glm::vec3(0, 5, 0), glm::radians(rotation));
		Laser(position, direction * 100.0f, rotation);
	}

}
