/*
GLuint skyboxVAO, skyboxVBO;
skybox_buffer(skyboxVAO, skyboxVBO);

//read texture
std::vector<std::string> faces
{
    "/Users/mr.wang/2017term/Computer Graphics/learnOpenGL/opengl/opengl/skybox/right.tga",
    "/Users/mr.wang/2017term/Computer Graphics/learnOpenGL/opengl/opengl/skybox/left.tga",
    "/Users/mr.wang/2017term/Computer Graphics/learnOpenGL/opengl/opengl/skybox/top.tga",
    "/Users/mr.wang/2017term/Computer Graphics/learnOpenGL/opengl/opengl/skybox/bottom.tga",
    "/Users/mr.wang/2017term/Computer Graphics/learnOpenGL/opengl/opengl/skybox/back.tga",
    "/Users/mr.wang/2017term/Computer Graphics/learnOpenGL/opengl/opengl/skybox/front.tga"
};
GLuint cubemapTexture = loadCubemap(faces);
//shader program
GLuint skyShader = LoadShaders("/Users/mr.wang/2017term/Computer Graphics/learnOpenGL/opengl/opengl/cubeShader.vs", "/Users/mr.wang/2017term/Computer Graphics/learnOpenGL/opengl/opengl/cubeShader.fs");

glUseProgram(skyShader);
view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
glUniformMatrix4fv(glGetUniformLocation(skyShader, "view"), 1, GL_FALSE, glm::value_ptr(view));
glUniformMatrix4fv(glGetUniformLocation(skyShader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
render_skybox(skyboxVAO, cubemapTexture);
 */


#ifndef skybox_h
#define skybox_h

#include <GL/glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include "stb_image.h"
#include <common/shader.hpp>

//GLuint loadCubemap(std::vector<std::string> faces);
//void render_skybox(GLuint VAO, GLuint VBO,GLuint cubemapTexture);
//void skybox_buffer(GLuint &skyboxVAO, GLuint &skyboxVBO);

GLuint loadCubemap(std::vector<std::string> faces);
void render_skybox(GLuint skyboxVAO, GLuint cubemapTexture);
void skybox_buffer(GLuint &skyboxVAO, GLuint &skyboxVBO);

#endif /* skybox_h */
