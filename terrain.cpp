
#include "terrain.hpp"

bool generateTerrain (
	int length, int width,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
	){
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	glm::vec3 vertex;
	glm::vec2 uv;
	glm::vec3 normal;
	for ( int i = 0; i < length + 1; i++ ){
		for ( int j = 0; j < width + 1; j++ ){
			vertex.x = i - length / 2;
			vertex.z = j - width / 2;
			vertex.y = 0;
			uv.x = (float) i / (float) length;
			uv.y = (float) j / (float) width;
			normal.x = 0;
			normal.z = 0;
			normal.y = 1;   //Set terrain height here
			temp_vertices.push_back ( vertex );
			temp_uvs.push_back ( uv );
			temp_normals.push_back ( normal );
		}
	}
	for ( int a = 0; a < length; a++ ){
		for ( int b = 0; b < width; b++ ){
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			vertexIndex[0] = uvIndex[0] = normalIndex[0] = a*length + b;
			vertexIndex[1] = uvIndex[1] = normalIndex[1] = ( a + 1 )*length + b;
			vertexIndex[2] = uvIndex[2] = normalIndex[2] = a*length + b + 1;
			vertexIndices.push_back ( vertexIndex[0] );
			vertexIndices.push_back ( vertexIndex[1] );
			vertexIndices.push_back ( vertexIndex[2] );
			uvIndices.push_back ( uvIndex[0] );
			uvIndices.push_back ( uvIndex[1] );
			uvIndices.push_back ( uvIndex[2] );
			normalIndices.push_back ( normalIndex[0] );
			normalIndices.push_back ( normalIndex[1] );
			normalIndices.push_back ( normalIndex[2] );

			vertexIndex[0] = uvIndex[0] = normalIndex[0] = a*length + b + 1;
			vertexIndex[1] = uvIndex[1] = normalIndex[1] = ( a + 1 )*length + b;
			vertexIndex[2] = uvIndex[2] = normalIndex[2] = ( a + 1 )*length + b + 1;
			vertexIndices.push_back ( vertexIndex[0] );
			vertexIndices.push_back ( vertexIndex[1] );
			vertexIndices.push_back ( vertexIndex[2] );
			uvIndices.push_back ( uvIndex[0] );
			uvIndices.push_back ( uvIndex[1] );
			uvIndices.push_back ( uvIndex[2] );
			normalIndices.push_back ( normalIndex[0] );
			normalIndices.push_back ( normalIndex[1] );
			normalIndices.push_back ( normalIndex[2] );
		}
	}

	for ( int i = 0; i < vertexIndices.size ( ); i++ ){
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];
		vertex = temp_vertices[vertexIndex];
		uv = temp_uvs[uvIndex];
		normal = temp_normals[normalIndex];
		out_vertices.push_back ( vertex );
		out_uvs.push_back ( uv );
		out_normals.push_back ( normal );
	}

	return true;
}