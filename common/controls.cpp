// Include GLFW
#include <glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.
#include <iostream>


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"
#include "aabb.hpp"

#define PI 3.1415926f

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 3, 5 );

glm::vec3 motion_direction = glm::vec3(0,0,-1);
glm::vec3 motion_right;
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 5.0f; // 3 units / second
float mouseSpeed = 0.005f;

float motion_horizonal_angle = 0;
float motion_vertical_angle = 0.0f;


glm::vec3 yAxis = glm::vec3(0,1,0);
glm::vec3 zAxis = glm::vec3(0,0,1);

glm::vec3 operator*(double m, glm::vec3& v){
	return glm::vec3(m*v.x, m*v.y, m*v.z);
}
glm::vec3 operator*(glm::vec3 v1, glm::vec3& v2){
	return glm::vec3(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
}

AABB character;
AABB object[100];
int topid = 0;
void setAABB ( std::vector<glm::vec3> & vertices, int type, glm::mat4 trans ){
	glm::vec3 *v;
	v = new glm::vec3[vertices.size ( )];
	for ( int i = 0; i < vertices.size ( ); i++ ){
		v[i] = vertices[i];
	}
	//printf ( "size=%d\n", vertices.size ( ) );
	if ( type == CHARACTER ){
		character.updateMinMax (v, (int)vertices.size());
	}
	if ( type == OBJECT ){
		object[topid].updateMinMax (v, (int)vertices.size() );
		topid++;
	}
}
bool checkCollide ( glm::mat4 m ){
	for ( int i = 0; i < topid; i++ ){
		if ( character.transform ( m ).collide ( object[i] ) ){
			// printf ( "collide!\n" );
			return true;
		}
	}
	return false;
}

glm::mat4 computeMatricesFromInputs(glm::mat4& BikeTransformMatrix){
	glm::mat4 rotateMatrix = glm::mat4(1.0f);
    static int init_flag = 0;
    if(init_flag < 100)++init_flag;
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
    if(init_flag<5){
        verticalAngle = 0.0f;
        horizontalAngle = 3.14f;
        glfwSetCursorPos(window, 1024/2, 768/2);
    }
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	if(init_flag>=5){
        horizontalAngle += mouseSpeed * float(1024/2 - xpos );
    }
    // verticalAngle += mouseSpeed * float( 768/2 - ypos );



	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 view_direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
			sin(horizontalAngle - PI/2.0f),
			0,
			cos(horizontalAngle - PI/2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross( right, view_direction );

    motion_direction = glm::vec3(
            cos(motion_vertical_angle) * sin(motion_horizonal_angle),
            sin(motion_vertical_angle),
            cos(motion_vertical_angle) * cos(motion_horizonal_angle)
    );

    motion_right = glm::vec3(
            sin(motion_horizonal_angle - PI/2.0f),
            0,
            cos(motion_horizonal_angle - PI/2.0f)
    );

    glm::vec3 delta_position = glm::vec3(0.0f);

    // Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		if ( !checkCollide(glm::translate ( BikeTransformMatrix*glm::rotate ( glm::mat4(1.0f), motion_horizonal_angle, yAxis ), -motion_direction*deltaTime*speed )))
			delta_position -= motion_direction * deltaTime * speed;
    }
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		if ( !checkCollide(glm::translate ( BikeTransformMatrix*glm::rotate ( glm::mat4(1.0f), motion_horizonal_angle, yAxis ), motion_direction*deltaTime*speed )))
			delta_position += motion_direction * deltaTime * speed;
    }

//    motion_direction = glm::vec3(
//            cos(motion_vertical_angle) * sin(motion_horizonal_angle),
//            sin(motion_vertical_angle),
//            cos(motion_vertical_angle) * cos(motion_horizonal_angle)
//    );
//
//    bikeModelMatrix = glm::translate(bikeModelMatrix, motion_speed*(delta_ws_position*motion_direction));
//
    glm::mat4 checkMatrix = glm::mat4(1.0f);
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
//		delta_position -= motion_right * deltaTime * speed;
//		if(motion_horizonal_angle > -1.0f*PI)

        checkMatrix = BikeTransformMatrix* glm::rotate(glm::rotate(glm::mat4(1.0f), motion_horizonal_angle-0.01f, yAxis), -PI/7.0f, motion_direction);
        if (!checkCollide (checkMatrix))
		{
			motion_horizonal_angle -= 0.01f;
			rotateMatrix = glm::rotate(rotateMatrix, PI/-7.0f, motion_direction);
		}
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
//		delta_position += motion_right * deltaTime * speed;
//		if(motion_horizonal_angle < PI)
        checkMatrix = BikeTransformMatrix* glm::rotate(glm::rotate(glm::mat4(1.0f), motion_horizonal_angle+0.01f, yAxis), PI/7.0f, motion_direction);
        if(!checkCollide(checkMatrix))
		{
			motion_horizonal_angle += 0.01f;
			rotateMatrix = glm::rotate(rotateMatrix, PI/7.0f, motion_direction);
		}
	}

//	std::cout << motion_horizonal_angle << std::endl;
	rotateMatrix = glm::rotate(rotateMatrix, motion_horizonal_angle, yAxis);
    BikeTransformMatrix = glm::translate(BikeTransformMatrix, delta_position);
//	character.translater(delta_position);
//    std::cout << BikeModelMatrix <<std::endl;
    glm::vec3 BikePosition = glm::vec3(BikeTransformMatrix*glm::vec4(1,1,1,1));
    position = BikePosition + glm::vec3(-2.0f,3.0f,0.0f) + motion_direction * 8.0f;

//    std::cout << motion_direction.x << std::endl;
//    std::cout << (delta_position*motion_direction).x << std::endl;


	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 200.0f);
//    std::cout << "view position: ("<< position.x <<", " << position.y << ", " << position.z  << ")"<< std::endl;
    std::cout << "bike position: ("<< BikePosition.x <<", " << BikePosition.y << ", " << BikePosition.z  << ")"<< std::endl;

    // Camera matrix
	ViewMatrix       = glm::lookAt(
			position,           // Camera is here
			position+view_direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
	return rotateMatrix;
}