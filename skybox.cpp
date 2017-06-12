//
//  skybox.cpp
//  opengl
//
//  Created by 王大鑫 on 2017/6/11.
//  Copyright © 2017年 王大鑫. All rights reserved.
//

#include "skybox.h"

GLuint loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return textureID;
}

void render_skybox(GLuint skyboxVAO, GLuint skyboxVBO,GLuint cubemapTexture)
{
    glDepthFunc(GL_LEQUAL);
//    view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
//    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
//    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // skybox cube

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//    glBindVertexArray(skyboxVAO);
//
//
//


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
//    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    glDisableVertexAttribArray(0);
}

void skybox_buffer(GLuint &skyboxVAO, GLuint &skyboxVBO)
{
    GLfloat skyboxVertices[] = {
        // Positions
        -40.0f,  40.0f, -40.0f,
        -40.0f, -40.0f, -40.0f,
        40.0f, -40.0f, -40.0f,
        40.0f, -40.0f, -40.0f,
        40.0f,  40.0f, -40.0f,
        -40.0f,  40.0f, -40.0f,
        
        -40.0f, -40.0f,  40.0f,
        -40.0f, -40.0f, -40.0f,
        -40.0f,  40.0f, -40.0f,
        -40.0f,  40.0f, -40.0f,
        -40.0f,  40.0f,  40.0f,
        -40.0f, -40.0f,  40.0f,
        
        40.0f, -40.0f, -40.0f,
        40.0f, -40.0f,  40.0f,
        40.0f,  40.0f,  40.0f,
        40.0f,  40.0f,  40.0f,
        40.0f,  40.0f, -40.0f,
        40.0f, -40.0f, -40.0f,
        
        -40.0f, -40.0f,  40.0f,
        -40.0f,  40.0f,  40.0f,
        40.0f,  40.0f,  40.0f,
        40.0f,  40.0f,  40.0f,
        40.0f, -40.0f,  40.0f,
        -40.0f, -40.0f,  40.0f,
        
        -40.0f,  40.0f, -40.0f,
        40.0f,  40.0f, -40.0f,
        40.0f,  40.0f,  40.0f,
        40.0f,  40.0f,  40.0f,
        -40.0f,  40.0f,  40.0f,
        -40.0f,  40.0f, -40.0f,
        
        -40.0f, -40.0f, -40.0f,
        -40.0f, -40.0f,  40.0f,
        40.0f, -40.0f, -40.0f,
        40.0f, -40.0f, -40.0f,
        -40.0f, -40.0f,  40.0f,
        40.0f, -40.0f,  40.0f
    };
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

}

