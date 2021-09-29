
#include "Asteroid.hpp"

#include "AsteroidsGame.hpp"
#include "Laser.hpp"
#include "SpaceShip.hpp"

void Asteroid::initializeValues(AsteroidsGame* game, Asteroid::AsteroidSize size)
{
	switch (size)
	{
	case AsteroidSize::Small:
		radius = 15;
		break;
	case AsteroidSize::Medium:
		radius = 25;
		break;
	case AsteroidSize::Big:
		radius = 55;
		break;
	}
	this->game = game;
	this->size = size;
	winSize = sre::Renderer::instance->getDrawableSize();

	constexpr int maxRotation = 7;
	rotationSpeed = rand() % maxRotation;
	rotationSpeed -= maxRotation / 2;

	constexpr float maxVelocity = 3.0f;
	const float velocityX = AsteroidsGame::getRandomFloat(-maxVelocity, maxVelocity); // (-maxVelocity + 1) + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (maxVelocity - (-maxVelocity + 1));
	const float velocityY = AsteroidsGame::getRandomFloat(-maxVelocity, maxVelocity); //(-maxVelocity + 1) + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (maxVelocity - (-maxVelocity + 1));
	velocity = glm::vec2(velocityX, velocityY);
}


Asteroid::Asteroid(const sre::Sprite& sprite, AsteroidsGame* game, AsteroidSize size, glm::vec2 pos) : GameObject(sprite)
{
	position = pos;
	initializeValues(game, size);
}



Asteroid::Asteroid(const sre::Sprite& sprite, AsteroidsGame* game, AsteroidSize size) : GameObject(sprite)
{
	const float randomPosX = rand() % static_cast<int>(winSize.x);
	const float randomPosY = rand() % static_cast<int>(winSize.y);
	position = glm::vec2(randomPosX, randomPosY);

	initializeValues(game, size);
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
	const auto laser = std::dynamic_pointer_cast<Laser>(other);
	if (laser != nullptr)
	{
		game->destroyAsteroid(this);
		return;
	}
	const auto spaceShip = std::dynamic_pointer_cast<SpaceShip>(other);
	if (spaceShip != nullptr)
	{
		game->destroySpaceShip(spaceShip.get());
	}


	//std::cout << "collided" << std::endl;
}