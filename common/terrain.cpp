#include <iostream>
#include "terrain.hpp"
#define PI 3.14159265

std::vector<glm::vec3> terrain;
int length;
int width;

//Set terrain height here
float height ( float x, float z ){
	if ( x > 60 && x < 70 && z<5 && z>-5 ){
		return 1.2f*( x - 60 );
	}
	else if ( x >= 70 && x < 80 && z<5 && z>-5 ){
		return 0.8f*( 80 - x );
	}
	if ( z<50 && z>-50 ){
		if ( ( -0.004*z*z + 10.0 )*cos ( ( x ) / 10.0 / PI ) > 0.0 )
			return (float)( -0.004f*z*z + 10.0 )*cos ( ( x ) / 10.0 / PI );
		else{
			return 0;
		}
	}
	else
		return 0;
}

bool generateTerrain (
	int l, int w,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
	){
	length = l;
	width = w;
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;
	glm::vec3 vertex;
	glm::vec2 uv;
	glm::vec3 normal;
	for ( int i = 0; i < l + 1; i++ ){
		for ( int j = 0; j < w + 1; j++ ){
			vertex.x = i - l / 2;
			vertex.z = j - w / 2;
			vertex.y = height ( vertex.x, vertex.z );
			uv.x = (float) i / (float) l;
			uv.y = (float) j / (float) w;
			normal.x = 0;
			normal.z = 0;
			normal.y = 1;
			terrain.push_back ( vertex );
			temp_uvs.push_back ( uv );
			temp_normals.push_back ( normal );
		}
	}
	for ( int a = 0; a < l; a++ ){
		for ( int b = 0; b < w; b++ ){
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			vertexIndex[0] = uvIndex[0] = normalIndex[0] = a*l + b;
			vertexIndex[1] = uvIndex[1] = normalIndex[1] = ( a + 1 )*l + b;
			vertexIndex[2] = uvIndex[2] = normalIndex[2] = a*l + b + 1;
			vertexIndices.push_back ( vertexIndex[0] );
			vertexIndices.push_back ( vertexIndex[1] );
			vertexIndices.push_back ( vertexIndex[2] );
			uvIndices.push_back ( uvIndex[0] );
			uvIndices.push_back ( uvIndex[1] );
			uvIndices.push_back ( uvIndex[2] );
			normalIndices.push_back ( normalIndex[0] );
			normalIndices.push_back ( normalIndex[1] );
			normalIndices.push_back ( normalIndex[2] );

			vertexIndex[0] = uvIndex[0] = normalIndex[0] = a*l + b + 1;
			vertexIndex[1] = uvIndex[1] = normalIndex[1] = ( a + 1 )*l + b;
			vertexIndex[2] = uvIndex[2] = normalIndex[2] = ( a + 1 )*l + b + 1;
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
		vertex = terrain[vertexIndex];
		uv = temp_uvs[uvIndex];
		normal = temp_normals[normalIndex];
		out_vertices.push_back ( vertex );
		out_uvs.push_back ( uv );
		out_normals.push_back ( normal );
	}

	return true;
}

float getHeight ( float x, float z ){
	if ( x<-length || x>length || z<-width || z>width )
		return 0;
	float a, b, c, d;
	float da, db, dc, dd;
	float dt;/*
			 a = terrain[( (int) x + length / 2 )*( width + 1 ) + (int) z + width / 2].y;
			 b = terrain[( (int) x + 1 + length / 2 )*( width + 1 ) + (int) z + width / 2].y;
			 c = terrain[( (int) x + length / 2 )*( width + 1 ) + (int) z + 1 + width / 2].y;
			 d = terrain[( (int) x + 1 + length / 2 )*( width + 1 ) + (int) z + 1 + width / 2].y;*/
	da = ( x - (int) x )*( x - (int) x ) + ( z - (int) z )*( z - (int) z );
	db = ( x - (int) x + 1 )*( x - (int) x + 1 ) + ( z - (int) z )*( z - (int) z );
	dc = ( x - (int) x )*( x - (int) x ) + ( z - (int) z + 1 )*( z - (int) z + 1 );
	dd = ( x - (int) x + 1 )*( x - (int) x + 1 ) + ( z - (int) z + 1 )*( z - (int) z + 1 );
	dt = da + db + dc + dd;
	return height ( x, z );//a*da / dt + b*db / dt + c*dc / dt + d*dd / dt;
}