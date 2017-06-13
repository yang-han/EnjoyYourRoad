
#include <glm/glm.hpp>
#include <stdio.h>
#include <math.h>
#include <vector>
#include "aabb.hpp"

AABB character;
std::vector<AABB> object;
std::vector<glm::mat4> objectTransformMatrix;
int topid = 0;

int createAABB ( std::vector<glm::vec3> & vertices, int type ){
    glm::vec3 *v;
    v = new glm::vec3[vertices.size ( )];
    for ( int i = 0; i < vertices.size ( ); i++ ){
        v[i] = vertices[i];
    }
    if ( type == CHARACTER ){
        character.updateMinMax ( v, (int) vertices.size ( ) );
        return -1;
    }
    if ( type == OBJECT ){
        AABB obj;
		obj.updateMinMax ( v, (int) vertices.size ( ) );
        object.push_back(obj);
		objectTransformMatrix.push_back ( glm::mat4 ( 1.0f ) );
        return topid++;
    }
}

void setAABB ( int AABBID, glm::mat4 trans ){
    if ( AABBID < 0 ){
        character = character.transform ( trans );
    }
    else if(AABBID<topid){
		objectTransformMatrix[AABBID] = trans;
    }
}

bool checkCollide ( glm::mat4 m ){
    for ( int i = 0; i < topid; i++ ){
        if ( character.transform ( m ).collide ( object[i].transform( objectTransformMatrix[i] ) ) ){
            printf ( "collide with obj %d!\n",i );
            return true;
        }
    }
    return false;
}


glm::vec3 AABB::getCenter ( ){
	glm::vec3 center;
	center.x = 0.5f*( pmin.x + pmax.x );
	center.y = 0.5f*( pmin.y + pmax.y );
	center.z = 0.5f*( pmin.z + pmax.z );
	return center;
}

glm::vec3 AABB::getBottomCenter ( ){
	glm::vec3 bottomCenter;
	bottomCenter.x = 0.5f*( pmin.x + pmax.x );
	bottomCenter.y = 0.0f;
	bottomCenter.z = 0.5f*( pmin.z + pmax.z );
	return bottomCenter;
}

glm::vec3 AABB::getBottomFront ( ){
	glm::vec3 bottomFront;
	bottomFront.x = 0.5f*( pmin.x + pmax.x );
	bottomFront.y = 0.0f;
	bottomFront.z = pmin.z;
	return bottomFront;
}

glm::vec3 AABB::getBottomBack ( ){
	glm::vec3 bottomBack;
	bottomBack.x = 0.5f*( pmin.x + pmax.x );
	bottomBack.y = 0.0f;
	bottomBack.z = pmax.z;
	return bottomBack;
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
	//printf ( "pmin=(%f,%f,%f),pmax=(%f,%f,%f)\n", pmin.x, pmin.y, pmin.z, pmax.x, pmax.y, pmax.z );
}

AABB AABB::transform ( const glm::mat4& mat ){
	glm::vec3 corners[8];
	AABB trans ( this->pmin, this->pmax );
	trans.getCorners ( corners );
	for ( int i = 0; i < 8; i++ ){
		glm::vec4 position = glm::vec4 ( corners[i], 1.0 );
		glm::vec4 transform = mat*position;
		corners[i].x = transform.x / transform.w;
		corners[i].y = transform.y / transform.w;
		corners[i].z = transform.z / transform.w;
	}
	trans.reset ( );
	trans.updateMinMax ( corners, 8 );
	return trans;
}

bool AABB::collide ( const AABB& aabb ) const{
	return ( ( pmin.x >= aabb.pmin.x && pmin.x <= aabb.pmax.x ) || ( aabb.pmin.x >= pmin.x && aabb.pmin.x <= pmax.x ) ) &&
		( ( pmin.y >= aabb.pmin.y && pmin.y <= aabb.pmax.y ) || ( aabb.pmin.y >= pmin.y && aabb.pmin.y <= pmax.y ) ) &&
		( ( pmin.z >= aabb.pmin.z && pmin.z <= aabb.pmax.z ) || ( aabb.pmin.z >= pmin.z && aabb.pmin.z <= pmax.z ) );
}
/*
AABB AABB::translate ( const glm::vec3 move ){
	return AABB ( this->pmin + move, this->pmax + move );
}

void AABB::translater ( const glm::vec3 move ){
	pmin += move;
	pmax += move;
}

AABB AABB::rotateH ( float angle ){
	glm::vec3 corners[8];
	AABB trans ( this->pmin, this->pmax );
	trans.getCorners ( corners );
	int x, z;
	for ( int i = 0; i < 8; i++ ){
		x = corners[i].x;
		z = corners[i].z;
		corners[i].x = x*cos ( angle ) - z*sin ( angle );
		corners[i].z = x*sin ( angle ) + z*cos ( angle );
	}
	trans.updateMinMax ( corners, 8 );
	return trans;
}

void AABB::rotateHr ( float angle ){
	glm::vec3 corners[8];
	this->getCorners ( corners );
	float x, z;
	for ( int i = 0; i < 8; i++ ){
		x = corners[i].x;
		z = corners[i].z;
		corners[i].x = x*cos ( angle ) - z*sin ( angle );
		corners[i].z = x*sin ( angle ) + z*cos ( angle );
	}
	this->updateMinMax ( corners, 8 );
}
*/