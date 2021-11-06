#include "Wolf3D.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>


using namespace std;
using namespace sre;
using namespace glm;

Wolf3D::Wolf3D()
	:fpsController(&camera)
{
	r.init();
	init();

	// Enable mouse lock
	SDL_SetWindowGrab(r.getSDLWindow(), SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);


	r.frameUpdate = [&](float deltaTime) {
		update(deltaTime);
	};
	r.frameRender = [&]() {
		render();
	};
	r.keyEvent = [&](SDL_Event& e) {
		fpsController.onKey(e);
	};
	r.mouseEvent = [&](SDL_Event& e) {
		if (!lockRotation)
		{
			fpsController.onMouse(e);
		}
	};
	r.startEventLoop();
}

void Wolf3D::update(float deltaTime)
{
	fpsController.update(deltaTime);
}

void Wolf3D::render()
{
	auto renderPass = RenderPass::create()
		.withCamera(camera)
		.build();

	renderPass.draw(walls, glm::mat4(1), wallMaterial);

	if (debugBricks)
	{
		renderDebugBricks(renderPass);
	}

	ImGui::SetNextWindowPos(ImVec2(Renderer::instance->getWindowSize().x / 2 - 100, .0f), ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Always);
	ImGui::Begin("", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::DragFloat("Rot", &fpsController.rotation);
	ImGui::DragFloat3("Pos", &(fpsController.position.x), 0.1f);
	ImGui::Checkbox("DebugBricks", &debugBricks);
	ImGui::Checkbox("LockRotation", &lockRotation);
	ImGui::End();
}

// Creates a textured cube with a side length of 1 (e.g. from -0.5 to 0.5).
// The cube must be centered at (x,0,z)
// The texturing
void Wolf3D::addCube(std::vector<glm::vec3>& vertexPositions, std::vector<glm::vec4>& textureCoordinates, int x, int z, int type)
{
	// Vertex positions
	glm::vec3 v0 = glm::vec3(-0.5 + x, -0.5, -0.5 + z);
	glm::vec3 v1 = glm::vec3(0.5 + x, -0.5, -0.5 + z);
	glm::vec3 v2 = glm::vec3(0.5 + x, -0.5, 0.5 + z);
	glm::vec3 v3 = glm::vec3(-0.5 + x, -0.5, 0.5 + z);
	glm::vec3 v4 = glm::vec3(-0.5 + x, 0.5, -0.5 + z);
	glm::vec3 v5 = glm::vec3(0.5 + x, 0.5, -0.5 + z);
	glm::vec3 v6 = glm::vec3(0.5 + x, 0.5, 0.5 + z);
	glm::vec3 v7 = glm::vec3(-0.5 + x, 0.5, 0.5 + z);

	// Middle of top and bottom faces. Use for ceiling and floor
	glm::vec3 v8 = glm::vec3(0.0 + x, 0.5, 0.0 + z);
	glm::vec3 v9 = glm::vec3(0.0 + x, -0.5, 0.5 + z);

	vertexPositions.insert(vertexPositions.end(), {
			v3, v2, v7,
			v6, v7, v2,

			v2, v1, v6,
			v5, v6, v1,

			v1, v0, v5,
			v4, v5, v0,

			v0, v3, v4,
			v7, v4, v3
		});

	glm::vec2 textureSize(2048, 4096);
	glm::vec2 tileSize(64, 64);
	glm::vec2 tileSizeWithBorder(65, 65);

	int widthOfTilemap = 16;
	int heightOfTilemap = 42;

	int xIdx = (type * 2) % widthOfTilemap;
	int yIdx = (type * 2) / widthOfTilemap;

	int xIdxDark = (type * 2 + 1) % widthOfTilemap;
	int yIdxDark = (type * 2 + 1) / widthOfTilemap;

	glm::vec2 min = vec2(xIdx * tileSizeWithBorder.x, (42 - yIdx) * tileSizeWithBorder.y) / textureSize;
	glm::vec2 max = min + tileSize / textureSize;

	glm::vec2 minDark = vec2(xIdxDark * tileSizeWithBorder.x, (42 - yIdxDark) * tileSizeWithBorder.y) / textureSize;
	glm::vec2 maxDark = minDark + tileSize / textureSize;

	textureCoordinates.insert(textureCoordinates.end(), {
			glm::vec4(min.x,min.y,0,0), glm::vec4(max.x,min.y,0,0), glm::vec4(min.x,max.y,0,0),
			glm::vec4(max.x,max.y,0,0), glm::vec4(min.x,max.y,0,0), glm::vec4(max.x,min.y,0,0),

			// Dark
			glm::vec4(minDark.x,minDark.y,0,0), glm::vec4(maxDark.x,minDark.y,0,0), glm::vec4(minDark.x,maxDark.y,0,0),
			glm::vec4(maxDark.x,maxDark.y,0,0), glm::vec4(minDark.x,maxDark.y,0,0), glm::vec4(maxDark.x,minDark.y,0,0),

			glm::vec4(min.x,min.y,0,0), glm::vec4(max.x,min.y,0,0), glm::vec4(min.x,max.y,0,0),
			glm::vec4(max.x,max.y,0,0), glm::vec4(min.x,max.y,0,0), glm::vec4(max.x,min.y,0,0),

			// Dark
			glm::vec4(minDark.x,minDark.y,0,0), glm::vec4(maxDark.x,minDark.y,0,0), glm::vec4(minDark.x,maxDark.y,0,0),
			glm::vec4(maxDark.x,maxDark.y,0,0), glm::vec4(minDark.x,maxDark.y,0,0), glm::vec4(maxDark.x,minDark.y,0,0),

		});
}

void Wolf3D::init()
{
	wallMaterial = Shader::getUnlit()->createMaterial();
	auto texture = Texture::create().withFile("level0.png")
		.withGenerateMipmaps(false)
		.withFilterSampling(false)
		.build();
	wallMaterial->setTexture(texture);

	map.loadMap("level0.json");

	std::vector<glm::vec3> vertexPositions;
	std::vector<glm::vec4> textureCoordinates;

	for (int x = 0; x < map.getWidth(); x++)
	{
		for (int y = 0; y < map.getHeight(); y++)
		{
			int field = map.getTile(x, y);
			if (field != -1)
			{
				addCube(vertexPositions, textureCoordinates, x, y, field);
			}
		}
	}

	fpsController.setInitialPosition(map.getStartingPosition(), map.getStartingRotation());

	walls = Mesh::create()
		.withPositions(vertexPositions)
		.withUVs(textureCoordinates)
		.build();
}

void Wolf3D::renderDebugBricks(RenderPass& renderPass)
{
	static auto cube = Mesh::create().withCube(0.5f).build();
	static vector<shared_ptr<Material> > materials = {
			Shader::getUnlit()->createMaterial(),
			Shader::getUnlit()->createMaterial(),
			Shader::getUnlit()->createMaterial()
	};

	std::vector<vec3> positions = {
			{-1,0,-2},
			{ 0,0,-3},
			{ 1,0,-4}
	};
	std::vector<sre::Color> colors = {
			{1,0,0,1},
			{0,1,0,1},
			{0,0,1,1},
	};
	for (int i = 0; i < positions.size(); i++)
	{
		materials[i]->setColor(colors[i]);
		renderPass.draw(cube, glm::translate(positions[i]), materials[i]);
	}
}

int main()
{
	new Wolf3D();
	return 0;
}