// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/text2D.hpp>

#include <terrain.hpp>
#include <aabb.hpp>

#include <skybox.h>

extern std::vector<AABB> object;
extern AABB character;

const int num_of_objs = 1;
int objs_AABB_ID[num_of_objs];

void render_a_obj(GLuint VAO, GLuint vertexbuffer, GLuint uvbuffer, GLuint normalbuffer, GLuint elementbuffer, GLsizei indices_size){
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
            0,                  // attribute
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
            1,                                // attribute
            2,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
    );

    // 3rd attribute buffer : normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
            2,                                // attribute
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
    );

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    // Draw the triangles !
    glDrawElements(
            GL_TRIANGLES,      // mode
            (GLsizei)indices_size,    // count
            GL_UNSIGNED_SHORT, // type
            (void*)0           // element array buffer offset
    );
    glBindVertexArray(0);
}


void prepareObj(GLuint& VertexArrayID,

                GLuint& vertexbuffer,
                GLuint& uvbuffer,
                GLuint& normalbuffer,
                GLuint& elementbuffer,

                const char * path,
                std::vector<glm::vec3> & vertices,
                std::vector<glm::vec2> & uvs,
                std::vector<glm::vec3> & normals,

                std::vector<unsigned short> & indices,
                std::vector<glm::vec3> & indexed_vertices,
                std::vector<glm::vec2> & indexed_uvs,
                std::vector<glm::vec3> & indexed_normals)
{

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    if(path== nullptr){
        generateTerrain(100,100,vertices,uvs,normals);
    }else{
        bool res = loadOBJ(path, vertices, uvs, normals);
    }
    indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

//    GLuint uvbuffer;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

//    GLuint normalbuffer;
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

    // Generate a buffer for the indices as well
    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	glBindVertexArray ( 0 );
}



int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Road", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE); // Not this time !

    //glEnable(GL_CULL_FACE);



//	GLuint bottomID = LoadShaders ( "bottom.vs", "bottom.fs" );
//
//	GLint bottomMatrixID = glGetUniformLocation ( bottomID, "MVP" );
//	GLint bottomViewMatrixID = glGetUniformLocation ( bottomID, "V" );
//	GLint bottomModelMatrixID = glGetUniformLocation ( bottomID, "M" );


    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "VertexShader.vs", "FragmentShader.fs" );

    // Get a handle for our "MVP" uniform
    GLint MatrixID = glGetUniformLocation(programID, "MVP");
    GLint ViewMatrixID = glGetUniformLocation(programID, "V");
    GLint ModelMatrixID = glGetUniformLocation(programID, "M");

    // Load the texture
    GLuint Texture = loadDDS("bottom.DDS");
    GLuint BikeTexture = loadDDS("norm.dds");

    // Get a handle for our "myTextureSampler" uniform
    GLint TextureID = glGetUniformLocation(programID, "myTextureSampler");


    GLuint VertexArrayID;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;GLuint vertexbuffer;
    GLuint uvbuffer;
    GLuint normalbuffer;
    GLuint elementbuffer;
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> indexed_vertices;
    std::vector<glm::vec2> indexed_uvs;
    std::vector<glm::vec3> indexed_normals;
    prepareObj(VertexArrayID, vertexbuffer, uvbuffer, normalbuffer, elementbuffer, nullptr,
               vertices, uvs, normals,
               indices, indexed_vertices, indexed_uvs, indexed_normals);

    GLuint BikeVertexArrayID;
    std::vector<glm::vec3> bike_vertices;
    std::vector<glm::vec2> bike_uvs;
    std::vector<glm::vec3> bike_normals;
    std::vector<unsigned short> bike_indices;
    std::vector<glm::vec3> indexed_bike_vertices;
    std::vector<glm::vec2> indexed_bike_uvs;
    std::vector<glm::vec3> indexed_bike_normals;
    GLuint bike_vertexbuffer;
    GLuint bike_uvbuffer;
    GLuint bike_normalbuffer;
    GLuint bike_elementbuffer;
    prepareObj(BikeVertexArrayID, bike_vertexbuffer, bike_uvbuffer, bike_normalbuffer, bike_elementbuffer, "bike.obj",
               bike_vertices, bike_uvs, bike_normals,
               bike_indices, indexed_bike_vertices, indexed_bike_uvs, indexed_bike_normals);
    int bike_AABB_ID = createAABB(bike_vertices,CHARACTER);

    GLuint testVertexArrayID;
    std::vector<glm::vec3> test_vertices;
    std::vector<glm::vec2> test_uvs;
    std::vector<glm::vec3> test_normals;
    std::vector<unsigned short> test_indices;
    std::vector<glm::vec3> indexed_test_vertices;
    std::vector<glm::vec2> indexed_test_uvs;
    std::vector<glm::vec3> indexed_test_normals;
    GLuint test_vertexbuffer;
    GLuint test_uvbuffer;
    GLuint test_normalbuffer;
    GLuint test_elementbuffer;
    prepareObj(testVertexArrayID, test_vertexbuffer, test_uvbuffer, test_normalbuffer, test_elementbuffer, "testobject1.obj",
               test_vertices, test_uvs, test_normals,
               test_indices, indexed_test_vertices, indexed_test_uvs, indexed_test_normals);
    int test_AABB_ID = createAABB(test_vertices,OBJECT);

    GLuint footVertexArrayID;
    std::vector<glm::vec3> foot_vertices;
    std::vector<glm::vec2> foot_uvs;
    std::vector<glm::vec3> foot_normals;
    std::vector<unsigned short> foot_indices;
    std::vector<glm::vec3> indexed_foot_vertices;
    std::vector<glm::vec2> indexed_foot_uvs;
    std::vector<glm::vec3> indexed_foot_normals;
    GLuint foot_vertexbuffer;
    GLuint foot_uvbuffer;
    GLuint foot_normalbuffer;
    GLuint foot_elementbuffer;
    prepareObj(footVertexArrayID, foot_vertexbuffer, foot_uvbuffer, foot_normalbuffer, foot_elementbuffer, "jtb.obj",
               foot_vertices, foot_uvs, foot_normals,
               foot_indices, indexed_foot_vertices, indexed_foot_uvs, indexed_foot_normals);

    GLuint objVertexArrayID[num_of_objs];
    std::vector<glm::vec3> obj_vertices[num_of_objs];
    std::vector<glm::vec2> obj_uvs[num_of_objs];
    std::vector<glm::vec3> obj_normals[num_of_objs];
    GLuint obj_vertexbuffer[num_of_objs];
    GLuint obj_uvbuffer[num_of_objs];
    GLuint obj_normalbuffer[num_of_objs];
    GLuint obj_elementbuffer[num_of_objs];
    std::vector<unsigned short> obj_indices[num_of_objs];
    std::vector<glm::vec3> obj_indexed_vertices[num_of_objs];
    std::vector<glm::vec2> obj_indexed_uvs[num_of_objs];
    std::vector<glm::vec3> obj_indexed_normals[num_of_objs];
    const char* obj_paths [num_of_objs] = {
            "cuthead.obj",
    };

    for (int i = 0; i < num_of_objs; ++i) {
        prepareObj(objVertexArrayID[i], obj_vertexbuffer[i], obj_uvbuffer[i], obj_normalbuffer[i], obj_elementbuffer[i], obj_paths[i],
                   obj_vertices[i], obj_uvs[i], obj_normals[i],
                   obj_indices[i], obj_indexed_vertices[i], obj_indexed_uvs[i], obj_indexed_normals[i]);
        objs_AABB_ID[i] = createAABB(obj_vertices[i],OBJECT);
    }

    // Get a handle for our "LightPosition" uniform
    glUseProgram(programID);
    GLint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    std::vector<std::string> faces
            {
                    "skybox/right.tga",
                    "skybox/left.tga",
                    "skybox/top.tga",
                    "skybox/bottom.tga",
                    "skybox/back.tga",
                    "skybox/front.tga"
            };


    GLuint cubemapTexture = loadCubemap(faces);
    GLuint skyShader = LoadShaders("cubeShader.vs", "cubeShader.fs");


    GLint sky_ProjectionMatrixID = glGetUniformLocation(skyShader, "projection");
    GLint sky_ViewMatrixID = glGetUniformLocation(skyShader, "view");

 //   GLint SkyTextureID = glGetUniformLocation(skyShader, "cubemap");



    GLuint skyboxVAO, skyboxVBO;
    skybox_buffer(skyboxVAO, skyboxVBO);


    initText2D("Holstein.DDS");

//    glm::mat4 BikeScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
    glm::mat4 BikeScaleMatrix = glm::mat4(1.0f);

//    glm::vec3 SceneMyRotateAxis(0,1,0);
//    glm::mat4 SceneRotationMatrix = glm::rotate(glm::mat4(1.0f), 3.14f, SceneMyRotateAxis);
    glm::mat4 SceneRotationMatrix = glm::mat4(1.0f);
    glm::mat4 SceneTransformMatrix = glm::translate(SceneRotationMatrix, glm::vec3(0,0,0));

//    glm::vec3 BikeRotateAxis(0,1,0);
//    glm::mat4 BikeRotateMatrix = glm::rotate(glm::mat4(1.0f), 3.14f, BikeRotateAxis);
//    glm::mat4 BikeRotateMatrix = glm::mat4(1.0f);
    glm::mat4 BikeTransformMatrix = glm::mat4(1.0f);
    glm::mat4 BikeModelMatrix = BikeTransformMatrix * BikeScaleMatrix;
    glm::mat4 BaseBikeModelMatrix = BikeModelMatrix;
    glm::mat4 BikeMotionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,20));

    glm::vec3 yAxis = glm::vec3(0,1,0);
    glm::mat4 obsRotateMatrix = glm::mat4(1.0f);
    glm::mat4 obsTransformMatrix = glm::mat4(1.0f);
    glm::mat4 obsModelMatrix = glm::mat4(1.0f);

    glm::mat4 footModelMatrix = glm::mat4(1.0f);
    do{

        // Measure speed
        double currentTime = glfwGetTime();
		nbFrames++;
		char text[256];
		static int fps = 0;
        if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1sec ago
            // printf and reset
			printf ( "%f ms/frame    FPS:%d\n", 1000.0 / double ( nbFrames ), nbFrames );
			fps = nbFrames;

            nbFrames = 0;
            lastTime += 1.0;
        }


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Use our shader
        glUseProgram(programID);



        // Compute the MVP matrix from keyboard and mouse input
        glm::mat4 bike_rotate_matrix = computeMatricesFromInputs(BikeMotionMatrix, footModelMatrix);

        BikeModelMatrix = BikeMotionMatrix * bike_rotate_matrix * BaseBikeModelMatrix;

        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();

        glm::mat4 SceneModelMatrix = SceneTransformMatrix;

        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * SceneModelMatrix;

        glm::mat4 bike_MVP = ProjectionMatrix * ViewMatrix * BikeModelMatrix;

		
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &SceneModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        // Bind our texture in Texture Unit 0
      
		glm::vec3 lightPos = glm::vec3 ( -60, 200, -95 );
		glUniform3f ( LightID, lightPos.x, lightPos.y, lightPos.z );

			/*
		glUseProgram ( bottomID );
		glUniformMatrix4fv ( bottomMatrixID, 1, GL_FALSE, &MVP[0][0] );
		glUniformMatrix4fv ( bottomModelMatrixID, 1, GL_FALSE, &SceneModelMatrix[0][0] );
		glUniformMatrix4fv ( bottomViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0] );

		*/
		glActiveTexture ( GL_TEXTURE1 );
		glBindTexture ( GL_TEXTURE_2D, Texture );
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i ( TextureID, 1 );
        render_a_obj(testVertexArrayID,test_vertexbuffer, test_uvbuffer, test_normalbuffer, test_elementbuffer, (GLsizei)test_indices.size());

        //        render_a_obj(vertexbuffer, uvbuffer, normalbuffer, elementbuffer, (GLsizei)indices.size());
		//        render_a_obj(test_vertexbuffer, test_uvbuffer, test_normalbuffer, test_elementbuffer, (GLsizei)test_indices.size());
		render_a_obj ( VertexArrayID, vertexbuffer, uvbuffer, normalbuffer, elementbuffer, (GLsizei) indices.size ( ) );

        obsRotateMatrix = glm::rotate(obsRotateMatrix, 0.01f, yAxis);
        obsTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-10, 0, -5));
        obsModelMatrix = obsTransformMatrix * obsRotateMatrix;

        glm::mat4 obs_MVP = ProjectionMatrix * ViewMatrix * obsModelMatrix;
        for (int i = 0; i < num_of_objs; ++i) {
            glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &obs_MVP[0][0]);
            glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &obsModelMatrix[0][0]);
            setAABB(objs_AABB_ID[i], obsModelMatrix);
            render_a_obj ( objVertexArrayID[i], obj_vertexbuffer[i], obj_uvbuffer[i], obj_normalbuffer[i], obj_elementbuffer[i], (GLsizei) obj_indices[i].size ( ) );
        }

        glm::mat4 foot_MVP = ProjectionMatrix * ViewMatrix * footModelMatrix;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &foot_MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &footModelMatrix[0][0]);
        render_a_obj(footVertexArrayID,foot_vertexbuffer, foot_uvbuffer, foot_normalbuffer, foot_elementbuffer, (GLsizei)foot_indices.size());


        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &bike_MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &BikeModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, BikeTexture);
        glUniform1i(TextureID, 0);

        render_a_obj(BikeVertexArrayID,bike_vertexbuffer, bike_uvbuffer, bike_normalbuffer, bike_elementbuffer, (GLsizei)bike_indices.size());


        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glUseProgram(skyShader);

        glm::mat4 view = glm::mat4(glm::mat3(ViewMatrix));
        glUniformMatrix4fv(sky_ProjectionMatrixID, 1, GL_FALSE, &ProjectionMatrix[0][0]);
        glUniformMatrix4fv(sky_ViewMatrixID, 1, GL_FALSE, &view[0][0]);

//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

        render_skybox(skyboxVAO, cubemapTexture);

		sprintf ( text, "FPS:%d", fps );
		printText2D ( text, 650, 550, 20 );
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &normalbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteProgram(programID);
    glDeleteTextures(1, &Texture);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

