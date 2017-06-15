#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <vector>
glm::vec3 computeMatricesFromInputs(glm::mat4&, glm::mat4&, glm::mat4&, glm::mat4&, glm::mat4&);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif