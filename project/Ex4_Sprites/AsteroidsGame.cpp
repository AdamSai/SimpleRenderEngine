#include <ctime>
#include <glm/gtc/constants.hpp>
#include "AsteroidsGame.hpp"
#include "sre/Texture.hpp"
#include "sre/Renderer.hpp"
#include "sre/Material.hpp"
#include "sre/SDLRenderer.hpp"

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
	auto laser = std::make_shared<Laser>(atlas->get("laserRed15.png"), pos, vel, rot, this);
	gameObjects.push_back(laser);
}


void AsteroidsGame::destroyGameObject(GameObject* gameObject)
{
	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (gameObject == gameObjects[i].get())
		{
			gameObjects.erase(gameObjects.begin() + i);
			return;
		}
	}
}

void AsteroidsGame::destroyAsteroid(Asteroid* asteroid)
{
	if (asteroid->size == Asteroid::AsteroidSize::Medium)
	{
		spawnAsteroid(Asteroid::AsteroidSize::Small, asteroid->position);
	}
	else if (asteroid->size == Asteroid::AsteroidSize::Big)
	{
		spawnAsteroid(Asteroid::AsteroidSize::Medium, asteroid->position);
	}
	destroyGameObject(asteroid);

}

void AsteroidsGame::spawnAsteroid(Asteroid::AsteroidSize size, glm::vec2 pos = glm::vec2(-1, -1))
{

	switch (size)
	{
	case Asteroid::AsteroidSize::Small:
	{
		auto smallMeteorSprite = atlas->get("meteorBrown_small1.png");
		gameObjects.push_back(std::make_shared<Asteroid>(smallMeteorSprite, this, Asteroid::AsteroidSize::Small, pos));
		gameObjects.push_back(std::make_shared<Asteroid>(smallMeteorSprite, this, Asteroid::AsteroidSize::Small, pos));
		break;
	}


	case Asteroid::AsteroidSize::Medium:
	{
		auto mediumMeteorSprite = atlas->get("meteorBrown_med1.png");
		gameObjects.push_back(std::make_shared<Asteroid>(mediumMeteorSprite, this, Asteroid::AsteroidSize::Medium, pos));
		gameObjects.push_back(std::make_shared<Asteroid>(mediumMeteorSprite, this, Asteroid::AsteroidSize::Medium, pos));
		break;
	}

	case Asteroid::AsteroidSize::Big: // For big bois we just need to spawn one, since they are created out of thin air
	{
		auto bigMeteorSprite = atlas->get("meteorBrown_big1.png");
		gameObjects.push_back(std::make_shared<Asteroid>(bigMeteorSprite, this, Asteroid::AsteroidSize::Big));
		break;
	}

	}
}

void AsteroidsGame::destroySpaceShip(GameObject* gameObject)
{
	playerIsAlive = false;
	const auto bang = std::make_shared<GameObject>(atlas->get("bang.png"));
	bang->position = gameObject->position;
	gameObjects.push_back(bang);
	destroyGameObject(gameObject);
}


using namespace std::chrono;
long long AsteroidsGame::GetCurrentTimeMillis() const
{
	return std::chrono::duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

float AsteroidsGame::getRandomFloat(float min, float max)
{
	return (min + 1) + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (max - (min + 1));
}

void AsteroidsGame::update(float deltaTime)
{
	int size = gameObjects.size();
	for (int i = 0; i < size; i++)
	{
		size = gameObjects.size();
		if (i >= size) // Since lasers get deleted over time AND on collision, we have to make sure we don't go out of bounds
			break;

		// Is the object we are looking at a collidable? If so check for collisions against other collidables.
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

				// We need position from GameObject class and we need to pass the GameObjects to the collision methods.
				const auto current = gameObjects[i];
				const auto other = gameObjects[j];

				// If the other object also is a collidable we check if they actually collide
				auto otherCollidable = std::dynamic_pointer_cast<Collidable>(other);
				if (collidable == nullptr || otherCollidable == nullptr)
					continue;
				// If they collide we tell each object what they are colliding with
				const float distance = glm::distance(current->position, other->position);
				if (distance <= collidable->getRadius() + otherCollidable->getRadius())
				{
					collidable->onCollision(other);
					otherCollidable->onCollision(current);
				}

			}

		}
	}

	// Call update method for all game objects and check if lasers should be killed
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
		.withClearColor(true, { 0, 0, .30, 1 })
		.build();
	auto spriteBatchBuilder = SpriteBatch::create();
	for (int i = 0; i < backgroundSprites.size(); i++)
	{
		backgroundSprites[i]->render(spriteBatchBuilder);
	}

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
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Always);
	ImGui::Begin("Score_and_Tracker", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::LabelText("GOs", "%i", (int)gameObjects.size());
	ImGui::LabelText("Stars", "%i", (int)backgroundSprites.size());
	ImGui::LabelText("Score", "%i", score);
	ImGui::End();

	if (!playerIsAlive)
	{
		ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x / 2 - 125, Renderer::instance->getWindowSize().y / 2 - 100), ImGuiSetCond_Always);
		ImGui::SetNextWindowSize(ImVec2(285, 70), ImGuiSetCond_Always);
		ImGui::Begin("death_text", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
		const std::string scoreText = "You died! Press space to try again";
		const float font_size = ImGui::GetFontSize() * scoreText.size() / 2;
		ImGui::SameLine(
			ImGui::GetWindowSize().x / 2 -
			font_size + (font_size / 2)
		);

		ImGui::Text("You died! Press space to try again");
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

void AsteroidsGame::generateStars()
{
	int starsPerRow = 30;
	auto size = sre::Renderer::instance->getDrawableSize();
	for (int x = 0; x <= size.x; x += size.x / starsPerRow)
	{
		for (int y = 0; y <= size.y; y += size.y / starsPerRow)
		{

			int star = rand() % 3;
			int randPos = rand() % 50;
			switch (star)
			{
			case 0:
			{
				auto star = std::make_shared<GameObject>(atlas->get("star1.png"));
				star->position = glm::vec2(x + randPos, y + randPos);
				star->scale = glm::vec2(0.1f, 0.1f);
				backgroundSprites.push_back(star);
				break;
			}
			case 1:
			{
				auto star = std::make_shared<GameObject>(atlas->get("star2.png"));
				star->position = glm::vec2(x + randPos, y + randPos);
				star->scale = glm::vec2(0.1f, 0.1f);
				backgroundSprites.push_back(star);
				break;
			}
			case 2:
			{
				auto star = std::make_shared<GameObject>(atlas->get("star3.png"));
				star->position = glm::vec2(x + randPos, y + randPos);
				star->scale = glm::vec2(0.1f, 0.1f);
				backgroundSprites.push_back(star);
				break;
			}
			}
		}
	}
}

void AsteroidsGame::restartGame()
{
	playerIsAlive = true;
	score = 0;
	gameObjects.clear();
	backgroundSprites.clear();

	auto spaceshipSprite = atlas->get("playerShip1_blue.png");
	auto spaceShipPointer = std::make_shared<SpaceShip>(spaceshipSprite, this);
	gameObjects.push_back(spaceShipPointer);

	generateStars();



	for (auto i = 0; i < 5; i++)
	{
		spawnAsteroid(Asteroid::AsteroidSize::Big);
	}



}


int main()
{
	new AsteroidsGame();
	return 0;
}
