#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <vector>
#define CHARACTER 0
#define OBJECT 1
void setAABB ( std::vector<glm::vec3> & vertices, int type, glm::mat4 trans = glm::mat4 ( 1.0f ) );
bool checkCollide ( glm::mat4 m );
glm::mat4 computeMatricesFromInputs(glm::mat4&);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif