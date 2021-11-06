#include "WorldMap.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace rapidjson;

void WorldMap::loadMap(std::string filename)
{

	ifstream fis(filename);
	IStreamWrapper isw(fis);
	Document d;
	d.ParseStream(isw);
	// TODO implement json loading this instead of the following

	auto spawn = d["spawn"].GetObject();
	startingPosition.x =spawn["x"].GetFloat();
	startingPosition.y = spawn["y"].GetFloat();
	startingRotation = spawn["angle"].GetInt();

	auto tilemap = d["tileMap"].GetArray();

	for (auto& v : tilemap)
	{
		std::vector<int> temp;

		for (auto& d : v.GetArray())
		{
			temp.push_back(d.GetInt());
		}
		values.push_back(temp);
	}
}


int WorldMap::getTile(int x, int y)
{
	return values.at(y).at(x);
}

int WorldMap::getWidth()
{
	return values[0].size();
}

int WorldMap::getHeight()
{
	return values.size();
}

glm::vec2 WorldMap::getStartingPosition()
{
	return startingPosition;
}

float WorldMap::getStartingRotation()
{
	return startingRotation;
}

glm::vec4 WorldMap::getFloorColor()
{
	return floorColor;
}

glm::vec4 WorldMap::getCeilColor()
{
	return ceilColor;
}
