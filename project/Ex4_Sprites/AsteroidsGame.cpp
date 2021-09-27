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

	auto spaceshipSprite = atlas->get("playerShip1_blue.png");
	//auto bangSprite = atlas->get("bang.png");
	auto bigMeteorSprite = atlas->get("meteorBrown_big1.png");
	//auto mediumMeteorSprite = atlas->get("meteorBrown_med1.png");
	//auto smallMeteorSprite = atlas->get("meteorBrown_small1.png");
	auto spaceShipPointer = std::make_shared<SpaceShip>(spaceshipSprite);
	gameObjects.push_back(spaceShipPointer);
	spaceShip = spaceShipPointer;

	for (auto i = 0; i < 5; i++)
	{
		gameObjects.push_back(std::make_shared<Asteroid>(bigMeteorSprite));
	}
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

void AsteroidsGame::update(float deltaTime)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->update(deltaTime);
		auto laser = std::dynamic_pointer_cast<Laser>(gameObjects[i]);
		if (laser != nullptr)
		{
			if (time(nullptr) - laser->spawnTime >= 1)
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
	ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::LabelText("GOs", "%i", (int)gameObjects.size());
	ImGui::LabelText("Score", "%i", score);
	ImGui::End();
}

void AsteroidsGame::keyEvent(SDL_Event& event)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		gameObjects[i]->onKey(event);
	}
	if (event.key.keysym.sym == SDLK_SPACE)
	{

		auto laser = std::make_shared<Laser>(atlas->get("laserRed15.png"), spaceShip->position, spaceShip->rotation);
		gameObjects.push_back(laser);


	}
	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_d)
	{
		debugCollisionCircles = !debugCollisionCircles;
	}
}

int main()
{
	new AsteroidsGame();
	return 0;
}