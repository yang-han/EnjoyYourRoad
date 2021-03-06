#ifndef AABB_HPP
#define AABB_HPP

#include <vector>
#include <glm/glm.hpp>


#define CHARACTER 0
#define OBJECT 1

class AABB{
public:
	glm::vec3 pmin;
	glm::vec3 pmax;
public:
	AABB ( ) :pmin ( glm::vec3 ( 100000, 100000, 100000 ) ), pmax ( glm::vec3 ( -100000, -100000, -100000 ) ){};
	AABB ( const glm::vec3 &min, const glm::vec3 &max ) :pmin ( min ), pmax ( max ){};
	void set ( const glm::vec3 & min, const glm::vec3 & max )
	{
		pmin = min;
		pmax = max;
	}
	bool setpmin ( float x, float y, float z ){
		if ( x <= pmax.x&&y <= pmax.y&&z <= pmax.z ){
			pmin.x = x;
			pmin.y = y;
			pmin.z = z;
			return true;
		}
		else{
			return false;
		}
	}
	bool setpmax ( float x, float y, float z ){
		if ( x >= pmin.x&&y >= pmin.y&&z >= pmin.z ){
			pmax.x = x;
			pmax.y = y;
			pmax.z = z;
			return true;
		}
		else{
			return false;
		}
	}
	void reset ( ){
		pmin = glm::vec3 ( 100000, 100000, 100000 );
		pmax = glm::vec3 ( -100000, -100000, -100000 );
	}
	glm::vec3 getCenter ( );
	glm::vec3 getBottomCenter ( );
	glm::vec3 getBottomFront ( );
	glm::vec3 getBottomBack ( );
	void getCorners ( glm::vec3 *dst ) const;
	void updateMinMax ( const glm::vec3 *point, int num );
	AABB transform ( const glm::mat4& mat );
	bool collide ( const AABB& aabb ) const;
//	AABB translate ( const glm::vec3 move );
//	void translater ( const glm::vec3 move );
//	AABB rotateH ( float angle );
//	void rotateHr ( float angle );
};

int createAABB ( std::vector<glm::vec3> &, int );
void setAABB ( int, glm::mat4 );
bool checkCollide ( glm::mat4 m );
bool rearrangeAABB ( int, glm::vec3, glm::vec3 );

#endif