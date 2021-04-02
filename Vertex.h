#pragma once

#include<glm.hpp>


struct Vertex						//stucture of vertex  , an array of color, position, texcoord  in game.cpp
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texcoord;
	glm::vec3 normal;
};

