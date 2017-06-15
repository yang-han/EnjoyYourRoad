
/*
    GLuint depthProgramID = LoadShaders( "/Users/mr.wang/2017term/Computer Graphics/Project/EnjoyYourRoad/depth.vs", "/Users/mr.wang/2017term/Computer Graphics/Project/EnjoyYourRoad/depth.fs" );
    
    int windowWidth;
    int windowHeight;
    glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
    GLuint depthMapFBO;
    GLuint depthMap;
    shadowFBO(depthMapFBO, depthMap);

    while(){
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        //--------------render depth of scene to the texture-------------
        glm::vec3 lightPos = glm::vec3(-4.0f, 35.0f, -0.0f);
        glm::mat4 lightSpaceMatrix = getlightSpaceMatrix(lightPos);

        glUseProgram(depthProgramID);
        glUniformMatrix4fv(glGetUniformLocation(depthProgramID, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
    
        // Clear the screen
        glViewport(0, 0, 2048, 2048);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        //set model
        ...
        //render as usual (only model matrix)
        ...

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //--------------------------------------------------------
        // reset viewport
        glViewport(0, 0, windowWidth, windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // render as usual (all matrix)
        ...



        ---------------------------how to use new shader----------------
        glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &SceneModelMatrix[0][0]);
        //or...
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
        
        glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
        
        glUniformMatrix4fv(glGetUniformLocation(programID, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
        //bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glUniform1i(TextureID, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glUniform1i(ShadowMapID, 1);
    }
*/

#ifndef shadow_h
#define shadow_h

#include <GL/glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
void shadowFBO(GLuint &depthMapFBO, GLuint &depthMap);
glm::mat4 getlightSpaceMatrix(glm::vec3 lightPos);

#endif //shadow_h