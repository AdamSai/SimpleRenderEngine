#define GLM_ENABLE_EXPERIMENTAL

#include "Laser.hpp"

#include <glm/gtx/rotate_vector.hpp>


#include "SpaceShip.hpp"
#include "sre/Renderer.hpp"

Laser::Laser(int id, const sre::Sprite& sprite, glm::vec2 position, glm::vec2 vel, float rotation, AsteroidsGame* game) : GameObject(sprite)
{
	this->id = id;
	winSize = sre::Renderer::instance->getDrawableSize();
	radius = 5;
	spawnTime = time(nullptr);
	this->position = position;
	this->rotation = rotation;
	this->game = game;
	//velocity = glm::vec2(direction.x * velocityFactor, direction.y * velocityFactor);
	velocity = vel;
	//velocity = glm::rotateZ(glm::vec3(0,100,0), glm::radians(rotation));


}


void Laser::update(float deltaTime)
{
	position += velocity * deltaTime;

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

void Laser::onCollision(std::shared_ptr<GameObject> other)
{
	auto player = std::dynamic_pointer_cast<SpaceShip>(other);
	if(player != nullptr)
		return;
	game->destroyLaser(this);
}

