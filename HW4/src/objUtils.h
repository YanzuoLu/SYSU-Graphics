#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

constexpr float eps = 1e-4;

class Vertex {
public:
	float point[4];
	float normal[4];
	float color[4];
	glm::vec2 texcoord;

	bool operator==(const Vertex& other) {
		return std::abs(point[0] - other.point[0]) < eps &&
			std::abs(point[1] - other.point[1]) < eps &&
			std::abs(point[2] - other.point[2]) < eps &&
			std::abs(point[3] - other.point[3]) < eps;
	}
};

std::pair<std::vector<Vertex>, std::vector<unsigned int>> LoadOBJ(std::istream& in);