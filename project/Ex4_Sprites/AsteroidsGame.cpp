#include <ctime>
#include <glm/gtc/constants.hpp>
#include "AsteroidsGame.hpp"

#include "Asteroid.hpp"
#include "GameObject.hpp"
#include "Laser.hpp"
#include "SpaceShip.hpp"


using namespace sre;

AsteroidsGame::AsteroidsGame()
{
	r.setWindowTitle("Asteroids");

	r.init().withSdlInitFlags(SDL_INIT_EVERYTHING)
		.withSdlWindowFlags(SDL_WINDOW_OPENGL);

	time_t t;
	// random seed based on time
	srand((unsigned)time(&t));

	atlas = SpriteAtlas::create("asteroids.json", "asteroids.png");

	restartGame();
	//gameObjects.push_back(std::make_shared<SpaceShip>(bangSprite));
	//gameObjects.push_back(std::make_shared<SpaceShip>(bigMeteorSprite));
	//gameObjects.push_back(std::make_shared<SpaceShip>(mediumMeteorSprite));
	//gameObjects.push_back(std::make_shared<SpaceShip>(smallMeteorSprite));
	//gameObjects.push_back(std::make_shared<SpaceShip>(laserSprite));

	camera.setWindowCoordinates();

	r.frameUpdate = [&](float deltaTime) {
		update(deltaTime);
	};

	r.keyEvent = [&](SDL_Event& event) {
		keyEvent(event);
	};

	r.frameRender = [&]() {
		render();
	};

	r.startEventLoop();
}


void AsteroidsGame::fireLaser(glm::vec2 pos, glm::vec2 vel, float rot)
{
	auto laser = std::make_shared<Laser>(gameObjects.size() - 1, atlas->get("laserRed15.png"), pos, vel, rot, this);
	gameObjects.push_back(laser);
}

void AsteroidsGame::destroyLaser(Laser* laser)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		auto otherLaser = std::dynamic_pointer_cast<Laser>(gameObjects[i]);
		if (laser == nullptr || otherLaser == nullptr)
			continue;
		if (laser->id == otherLaser->id)
		{
			gameObjects.erase(gameObjects.begin() + i);
			break;
		}
	}
}

void AsteroidsGame::destroyAsteroid(Asteroid* asteroid)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		auto otherAsteroid = std::dynamic_pointer_cast<Asteroid>(gameObjects[i]);
		if (asteroid == nullptr || otherAsteroid == nullptr)
			continue;
		if (asteroid->id == otherAsteroid->id)
		{
			if (asteroid->size == Asteroid::Medium)
			{
				spawnAsteroid(Asteroid::Small, asteroid->position);
			}
			else if (asteroid->size == Asteroid::Big)
			{
				spawnAsteroid(Asteroid::Medium, asteroid->position);
			}
			score++;
			gameObjects.erase(gameObjects.begin() + i);
			break;
		}
	}
}

void AsteroidsGame::spawnAsteroid(Asteroid::asteroidSize size, glm::vec2 pos)
{

	switch (size)
	{
	case Asteroid::Small:
	{
		auto smallMeteorSprite = atlas->get("meteorBrown_small1.png");
		gameObjects.push_back(std::make_shared<Asteroid>(smallMeteorSprite, this, Asteroid::Small, pos));
		gameObjects.push_back(std::make_shared<Asteroid>(smallMeteorSprite, this, Asteroid::Small, pos));
		break;
	}


	case Asteroid::Medium:
	{
		auto mediumMeteorSprite = atlas->get("meteorBrown_med1.png");
		gameObjects.push_back(std::make_shared<Asteroid>(mediumMeteorSprite, this, Asteroid::Medium, pos));
		gameObjects.push_back(std::make_shared<Asteroid>(mediumMeteorSprite, this, Asteroid::Medium, pos));
		break;
	}

	case Asteroid::Big:
	{
		auto bigMeteorSprite = atlas->get("meteorBrown_big1.png");
		gameObjects.push_back(std::make_shared<Asteroid>(bigMeteorSprite, this, Asteroid::Big, pos));
		gameObjects.push_back(std::make_shared<Asteroid>(bigMeteorSprite, this, Asteroid::Big, pos));
		break;
	}

	}
}

void AsteroidsGame::destroySpaceShip()
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		auto spaceShip = std::dynamic_pointer_cast<SpaceShip>(gameObjects[i]);
		if (spaceShip != nullptr)
		{
			playerIsAlive = false;
			// create bang
			auto bang = std::make_shared<GameObject>(atlas->get("bang.png"));
			bang->position = spaceShip->position;
			gameObjects.push_back(bang);
			// Destroy space ship
			gameObjects.erase(gameObjects.begin() + i);
			break;
		}
	}
}

void AsteroidsGame::update(float deltaTime)
{
	int size = gameObjects.size();
	for (int i = 0; i < size; i++)
	{
		size = gameObjects.size();
		if (i >= size)
			break;
		auto collidable = std::dynamic_pointer_cast<Collidable>(gameObjects[i]);
		if (collidable != nullptr)
		{
			for (int j = 0; j < size; j++)
			{
				size = gameObjects.size();
				if (i == j) // We don't want to collide with ourselves
					continue;
				if (i >= size || j >= size) // Since lasers get deleted over time AND on collision, we have to make sure we don't go out of bounds
					break;
				auto objA = gameObjects[i];
				auto objB = gameObjects[j];
				auto otherCollidable = std::dynamic_pointer_cast<Collidable>(objB);
				if (collidable == nullptr || otherCollidable == nullptr)
					continue;
				const float distance = glm::distance(objA->position, objB->position);
				if (distance <= collidable->getRadius() + otherCollidable->getRadius())
				{
					collidable->onCollision(objB);
					otherCollidable->onCollision(objA);
				}

			}

		}
	}
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update(deltaTime);

		auto laser = std::dynamic_pointer_cast<Laser>(gameObjects[i]);
		if (laser != nullptr)
		{
			if (time(nullptr) - laser->spawnTime > 1)
				gameObjects.erase(gameObjects.begin() + i);
		}
	}

	//for (int i = 0; i < lasers.size(); i++)
	//{
	//	if (lasers[i] != nullptr && time(nullptr) - lasers[i]->spawnTime >= 1)
	//	{

	//		std::cout << "Deleting laser" << std::endl;
	//		lasers[i].~shared_ptr();
	//	}
	//}
}



void drawCircle(std::vector<glm::vec3>& lines, glm::vec2 position, float radius)
{
	int sides = 15;
	float quaterPi = glm::two_pi<float>() / (float)sides;
	for (float f = 0; f < glm::two_pi<float>(); f += quaterPi)
	{
		// line from
		lines.push_back(glm::vec3{ position.x + cosf(f) * radius,
								  position.y + sinf(f) * radius,
								  0
			});
		// line to
		lines.push_back(glm::vec3{ position.x + cosf(f + quaterPi) * radius,
								  position.y + sinf(f + quaterPi) * radius,
								  0
			});
	}
}

void AsteroidsGame::render()
{
	auto renderPass = RenderPass::create()
		.withCamera(camera)
		.withClearColor(true, { .20, .60, .86, 1 })
		.build();
	auto spriteBatchBuilder = SpriteBatch::create();

	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->render(spriteBatchBuilder);
	}
	auto spriteBatch = spriteBatchBuilder.build();
	renderPass.draw(spriteBatch);

	if (debugCollisionCircles)
	{
		std::vector<glm::vec3> lines;
		for (auto& go : gameObjects)
		{
			auto col = std::dynamic_pointer_cast<Collidable>(go);
			if (col != nullptr)
			{
				drawCircle(lines, go->position, col->getRadius());
			}
		}
		renderPass.drawLines(lines);
	}

	ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x / 2 - 100, .0f), ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(ImVec2(200, 70), ImGuiSetCond_Always);
	ImGui::Begin("Score_and_Tracker", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::LabelText("GOs", "%i", (int)gameObjects.size());
	ImGui::LabelText("Score", "%i", score);
	ImGui::End();

	if (!playerIsAlive)
	{
		ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x / 2 - 125, Renderer::instance->getWindowSize().y / 2 - 100), ImGuiSetCond_Always);
		ImGui::SetNextWindowSize(ImVec2(285, 70), ImGuiSetCond_Always);
		ImGui::Begin("new", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		std::string scoreText = "You died! Press space to try again";
		float font_size = ImGui::GetFontSize() * scoreText.size() / 2;
		ImGui::SameLine(
        ImGui::GetWindowSize().x / 2 -
        font_size + (font_size / 2)
    );

		ImGui::Text( "You died! Press space to try again");
		ImGui::End();
	}
}

void AsteroidsGame::keyEvent(SDL_Event& event)
{
	if (playerIsAlive)
	{

		for (int i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->onKey(event);
		}
	}
	else
	{
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			restartGame();
		}
	}
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d)
	{
		debugCollisionCircles = !debugCollisionCircles;
	}
}

void AsteroidsGame::restartGame()
{
	playerIsAlive = true;
	score = 0;
	gameObjects.clear();

	auto spaceshipSprite = atlas->get("playerShip1_blue.png");
	auto bigMeteorSprite = atlas->get("meteorBrown_big1.png");
	auto spaceShipPointer = std::make_shared<SpaceShip>(spaceshipSprite, this);
	gameObjects.push_back(spaceShipPointer);

	for (auto i = 0; i < 5; i++)
	{
		gameObjects.push_back(std::make_shared<Asteroid>(bigMeteorSprite, this, Asteroid::Big));
	}

}


int main()
{
	new AsteroidsGame();
	return 0;
}
