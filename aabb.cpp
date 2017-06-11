
#include <glm/glm.hpp>
#include <stdio.h>
#include "aabb.hpp"

glm::vec3 AABB::getCenter ( ){
	glm::vec3 center;
	center.x = 0.5f*( pmin.x + pmax.x );
	center.y = 0.5f*( pmin.y + pmax.y );
	center.z = 0.5f*( pmin.z + pmax.z );
	return center;
}

void AABB::getCorners ( glm::vec3 *dst ) const{
	dst[0] = glm::vec3 ( pmin.x, pmax.y, pmax.z );
	dst[1] = glm::vec3 ( pmin.x, pmin.y, pmax.z );
	dst[2] = glm::vec3 ( pmax.x, pmin.y, pmax.z );
	dst[3] = glm::vec3 ( pmax.x, pmax.y, pmax.z );

	dst[4] = glm::vec3 ( pmax.x, pmax.y, pmin.z );
	dst[5] = glm::vec3 ( pmax.x, pmin.y, pmin.z );
	dst[6] = glm::vec3 ( pmin.x, pmin.y, pmin.z );
	dst[7] = glm::vec3 ( pmin.x, pmax.y, pmin.z );
}

void AABB::updateMinMax ( const glm::vec3 *point, int num ){
	for ( int i = 0; i < num; i++ )
	{
		if ( point[i].x < pmin.x )
			pmin.x = point[i].x;
		if ( point[i].y < pmin.y )
			pmin.y = point[i].y;
		if ( point[i].z < pmin.z )
			pmin.z = point[i].z;
		if ( point[i].x > pmax.x )
			pmax.x = point[i].x;
		if ( point[i].y > pmax.y )
			pmax.y = point[i].y;
		if ( point[i].z > pmax.z )
			pmax.z = point[i].z;
	}
	printf ( "pmin=(%f,%f,%f),pmax=(%f,%f,%f)\n", pmin.x, pmin.y, pmin.z, pmax.x, pmax.y, pmax.z );
}

AABB AABB::transform ( const glm::mat4& mat ){
	glm::vec3 corners[8];
	AABB trans ( this->pmin, this->pmax );
	trans.getCorners ( corners );
	int x, y, z, h;
	//����任  
	for ( int i = 0; i < 8; i++ ){
		glm::vec4 position = glm::vec4 ( corners[i], 1.0 );
		glm::vec4 transform = mat*position;
		corners[i].x = transform.x / transform.w;
		corners[i].y = transform.y / transform.w;
		corners[i].z = transform.z / transform.w;
	}
	trans.updateMinMax ( corners, 8 );
	return trans;
}

bool AABB::collide ( const AABB& aabb ) const{
	return ( ( pmin.x >= aabb.pmin.x && pmin.x <= aabb.pmax.x ) || ( aabb.pmin.x >= pmin.x && aabb.pmin.x <= pmax.x ) ) &&
		( ( pmin.y >= aabb.pmin.y && pmin.y <= aabb.pmax.y ) || ( aabb.pmin.y >= pmin.y && aabb.pmin.y <= pmax.y ) ) &&
		( ( pmin.z >= aabb.pmin.z && pmin.z <= aabb.pmax.z ) || ( aabb.pmin.z >= pmin.z && aabb.pmin.z <= pmax.z ) );
}

AABB AABB::translate ( const glm::vec3 move ){
	return AABB ( this->pmin + move, this->pmax + move );
}

void AABB::translater ( const glm::vec3 move ){
	pmin += move;
	pmax += move;
}
