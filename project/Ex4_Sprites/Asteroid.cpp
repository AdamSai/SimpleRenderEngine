
#include "Asteroid.hpp"

#include "sre/Renderer.hpp"

Asteroid::Asteroid(const sre::Sprite& sprite) : GameObject(sprite)
{
	radius = 55;
	winSize = sre::Renderer::instance->getDrawableSize();
	int maxRotation = 7;
	rotationSpeed = rand() % maxRotation;
	rotationSpeed -= maxRotation / 2;
	float maxVelocity = 3.0f;

	float randomPosX = (0 + 1) + (((float)rand()) / (float)RAND_MAX) * (winSize.x - (0 + 1));
	float randomPosY = (0 + 1) + (((float)rand()) / (float)RAND_MAX) * (winSize.y - (0 + 1));
	position = glm::vec2(randomPosX, randomPosY);

	float velocityX = (-maxVelocity + 1) + (((float)rand()) / (float)RAND_MAX) * (maxVelocity - (-maxVelocity + 1));
	float velocityY = (-maxVelocity + 1) + (((float)rand()) / (float)RAND_MAX) * (maxVelocity - (-maxVelocity + 1));

	velocity = glm::vec2(velocityX, velocityY);

}


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
	std::cout << "collided" << std::endl;
}