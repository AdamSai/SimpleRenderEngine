
#include "Asteroid.hpp"

#include "AsteroidsGame.hpp"
#include "Laser.hpp"
#include "SpaceShip.hpp"
int Asteroid::nextId = nextId >= 1 ? nextId : 1;


Asteroid::Asteroid(const sre::Sprite& sprite, AsteroidsGame* game, asteroidSize size, glm::vec2 pos) : GameObject(sprite)
{
	this->id = ++nextId;
	std::cout << std::to_string(nextId);
	position = pos;
	switch (size)
	{
	case Small:
		radius = 15;
		break;
	case Medium:
		radius = 25;
		break;
	case Big:
		radius = 55;
		break;
	}
	this->game = game;
	this->size = size;

	winSize = sre::Renderer::instance->getDrawableSize();
	int maxRotation = 7;
	rotationSpeed = rand() % maxRotation;
	rotationSpeed -= maxRotation / 2;
	float maxVelocity = 3.0f;
	if (pos == glm::vec2(-1, -1)) // If we don't input a pos value, give a random position
	{
		float randomPosX = (0 + 1) + (((float)rand()) / (float)RAND_MAX) * (winSize.x - (0 + 1));
		float randomPosY = (0 + 1) + (((float)rand()) / (float)RAND_MAX) * (winSize.y - (0 + 1));
		position = glm::vec2(randomPosX, randomPosY);
	}
	else
	{
		position = pos;
	}


	float velocityX = (-maxVelocity + 1) + (((float)rand()) / (float)RAND_MAX) * (maxVelocity - (-maxVelocity + 1));
	float velocityY = (-maxVelocity + 1) + (((float)rand()) / (float)RAND_MAX) * (maxVelocity - (-maxVelocity + 1));

	velocity = glm::vec2(velocityX, velocityY);

}

#include "sre/Renderer.hpp"


void Asteroid::update(float deltaTime)
{
	position += velocity;
	rotation += rotationSpeed;


	// Warp if object goes out of screen
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

void Asteroid::onCollision(std::shared_ptr<GameObject> other)
{
	auto laser = std::dynamic_pointer_cast<Laser>(other);
	if (laser != nullptr)
	{
		game->destroyAsteroid(this);
		return;
	}
	auto spaceShip = std::dynamic_pointer_cast<SpaceShip>(other); 
	if(spaceShip != nullptr)
	{
		game->destroySpaceShip();
	}


	//std::cout << "collided" << std::endl;
}