#include "FirstPersonController.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <iostream>

using namespace sre;
using namespace glm;


FirstPersonController::FirstPersonController(sre::Camera* camera)
	:camera(camera)
{
	camera->setPerspectiveProjection(45, 0.1f, 1000);
	position = vec3(0, 0, 0);
}


void FirstPersonController::update(float deltaTime)
{
	vec3 forwardVector = vec3(0, 0, -1);
	vec3 leftVector = vec3(-1, 0, 0);

	vec3 dir = glm::rotateY(forwardVector, glm::radians(rotation));


	vec3 dirLeft = glm::rotateY(leftVector, glm::radians(rotation));
	vec3 movementDir = glm::vec3(0, 0, 0);

	if (fwd)
	{
		movementDir += dir * movementSpeed;
	}

	if (bwd)
	{
		movementDir -= dir * movementSpeed;
	}

	if (left)
	{
		movementDir += dirLeft * movementSpeed;
	}

	if (right)
	{
		movementDir -= dirLeft * movementSpeed;

	}
	// We get NaN and black screen w/o this check
	if (movementDir != glm::vec3(0, 0, 0))
	{
		float movementSpeed = 3.0f;
		position += glm::normalize(movementDir) * movementSpeed * deltaTime;
	}
	// TODO implement the camera movement using position and rotation here
	camera->lookAt(position, position + dir, vec3{ 0,1.0f,0 });
}

void FirstPersonController::onKey(SDL_Event& event)
{
	if (event.key.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_w:

			fwd = true;
			break;
		case SDLK_a:
			left = true;
			break;

		case SDLK_s:
			bwd = true;
			break;
		case SDLK_d:
			right = true;
			break;
		}
	}
	else if (event.key.type == SDL_KEYUP)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_w:
			fwd = false;
			break;
		case SDLK_a:
			left = false;
			break;
		case SDLK_s:
			bwd = false;
			break;
		case SDLK_d:
			right = false;
			break;
		}
	}


}

void FirstPersonController::onMouse(SDL_Event& event)
{
	// TODO implement mouse input here
	rotation -= event.motion.xrel * sensitivity;

}

void FirstPersonController::setInitialPosition(glm::vec2 position, float rotation)
{
	this->position = glm::vec3(position.x, 0, position.y);
	this->rotation = rotation;
}
