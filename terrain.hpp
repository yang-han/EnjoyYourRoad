#ifndef TERRAIN_HPP
#define TERRAIN_HPP
#include <vector>
#include <glm/glm.hpp>

bool generateTerrain (
	int length, int width,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
	);

float getHeight ( float x, float z );

#endif