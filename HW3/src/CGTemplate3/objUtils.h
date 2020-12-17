#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

typedef struct {
	float point[4];
	float normal[4];
	float color[4];
	glm::vec2 texcoord;
} Vertex;

std::vector<Vertex> LoadOBJ(std::istream& in);