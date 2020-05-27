/*
 * Copyright 2019 Ilya Malgin
 * Frustum.cpp
 * This file is part of The Sugarpunky Chocolate Disaster project
 *
 * The Sugarpunky Chocolate Disaster project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Sugarpunky Chocolate Disaster project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * See <http://www.gnu.org/licenses/>
 *
 * Purpose: contains definition for Frustum class
 * @version 0.1.0
 */

#include "Frustum"

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

/**
* @brief plain ctor. Initializes all the planes to zero vectors
*/
Frustum::Frustum() noexcept
{
	planes.reserve( NUMBER_OF_PLANES );
	for( unsigned int planeIndex = 0; planeIndex < NUMBER_OF_PLANES; planeIndex++ )
	{
		planes.emplace_back( 0.0f );
	}
}

/**
* @brief normalizes plane vector (based on its XYZ part) to have unit length
* @param plane plane to normalize
*/
void Frustum::normalizePlane( FRUSTUM_PLANE plane )
{
	float magnitude = (float)glm::sqrt( planes[plane].x * planes[plane].x +
										planes[plane].y * planes[plane].y +
										planes[plane].z * planes[plane].z );
	planes[plane] /= magnitude;
}

/**
* @brief updates all the planes depending on the given projectionView matrix
* @param projectionView "projection * view"  matrix
*/
void Frustum::updateFrustum( const glm::mat4 & projectionView )
{
	const float * projectionViewElements = glm::value_ptr( projectionView );
	planes[FRUSTUM_RIGHT].x = projectionViewElements[3] - projectionViewElements[0];
	planes[FRUSTUM_RIGHT].y = projectionViewElements[7] - projectionViewElements[4];
	planes[FRUSTUM_RIGHT].z = projectionViewElements[11] - projectionViewElements[8];
	planes[FRUSTUM_RIGHT].w = projectionViewElements[15] - projectionViewElements[12];
	normalizePlane( FRUSTUM_RIGHT );

	planes[FRUSTUM_LEFT].x = projectionViewElements[3] + projectionViewElements[0];
	planes[FRUSTUM_LEFT].y = projectionViewElements[7] + projectionViewElements[4];
	planes[FRUSTUM_LEFT].z = projectionViewElements[11] + projectionViewElements[8];
	planes[FRUSTUM_LEFT].w = projectionViewElements[15] + projectionViewElements[12];
	normalizePlane( FRUSTUM_LEFT );

	planes[FRUSTUM_BOTTOM].x = projectionViewElements[3] + projectionViewElements[1];
	planes[FRUSTUM_BOTTOM].y = projectionViewElements[7] + projectionViewElements[5];
	planes[FRUSTUM_BOTTOM].z = projectionViewElements[11] + projectionViewElements[9];
	planes[FRUSTUM_BOTTOM].w = projectionViewElements[15] + projectionViewElements[13];
	normalizePlane( FRUSTUM_BOTTOM );

	planes[FRUSTUM_TOP].x = projectionViewElements[3] - projectionViewElements[1];
	planes[FRUSTUM_TOP].y = projectionViewElements[7] - projectionViewElements[5];
	planes[FRUSTUM_TOP].z = projectionViewElements[11] - projectionViewElements[9];
	planes[FRUSTUM_TOP].w = projectionViewElements[15] - projectionViewElements[13];
	normalizePlane( FRUSTUM_TOP );

	planes[FRUSTUM_BACK].x = projectionViewElements[3] - projectionViewElements[2];
	planes[FRUSTUM_BACK].y = projectionViewElements[7] - projectionViewElements[6];
	planes[FRUSTUM_BACK].z = projectionViewElements[11] - projectionViewElements[10];
	planes[FRUSTUM_BACK].w = projectionViewElements[15] - projectionViewElements[14];
	normalizePlane( FRUSTUM_BACK );

	planes[FRUSTUM_FRONT].x = projectionViewElements[3] + projectionViewElements[2];
	planes[FRUSTUM_FRONT].y = projectionViewElements[7] + projectionViewElements[6];
	planes[FRUSTUM_FRONT].z = projectionViewElements[11] + projectionViewElements[10];
	planes[FRUSTUM_FRONT].w = projectionViewElements[15] + projectionViewElements[14];
	normalizePlane( FRUSTUM_FRONT );
}

/**
* @brief updates all the planes intersection points using the Kramer's rule
* @note see: https://en.wikipedia.org/wiki/Cramer%27s_rule
*/
void Frustum::calculateIntersectionPoints()
{
	const glm::vec4 & FRONT = planes[FRUSTUM_FRONT];
	const glm::vec4 & BACK = planes[FRUSTUM_BACK];
	const glm::vec4 & LEFT = planes[FRUSTUM_LEFT];
	const glm::vec4 & RIGHT = planes[FRUSTUM_RIGHT];
	const glm::vec4 & TOP = planes[FRUSTUM_TOP];
	const glm::vec4 & BOTTOM = planes[FRUSTUM_BOTTOM];
	nearLL = kramerIntersection( FRONT, BOTTOM, LEFT );
	nearLR = kramerIntersection( FRONT, BOTTOM, RIGHT );
	nearUR = kramerIntersection( FRONT, TOP, RIGHT );
	nearUL = kramerIntersection( FRONT, TOP, LEFT );
	farLL = kramerIntersection( BACK, BOTTOM, LEFT );
	farLR = kramerIntersection( BACK, BOTTOM, RIGHT );
	farUR = kramerIntersection( BACK, TOP, RIGHT );
	farUL = kramerIntersection( BACK, TOP, LEFT );
}

/**
* @brief returns maximum X coordinate of each of 8 intersection points
*/
float Frustum::getMaxCoordX() const noexcept
{
	return std::max( { nearLL.x, nearLR.x, nearUR.x, nearUL.x,
					 farLL.x, farLR.x, farUR.x, farUL.x } );
}

/**
* @brief returns minimum X coordinate of each of 8 intersection points
*/
float Frustum::getMinCoordX() const noexcept
{
	return std::min( { nearLL.x, nearLR.x, nearUR.x, nearUL.x,
					 farLL.x, farLR.x, farUR.x, farUL.x } );
}

/**
* @brief returns maximum Z coordinate of each of 8 intersection points
*/
float Frustum::getMaxCoordZ() const noexcept
{
	return std::max( { nearLL.z, nearLR.z, nearUR.z, nearUL.z,
					 farLL.z, farLR.z, farUR.z, farUL.z } );
}

/**
* @brief returns minimum Z coordinate of each of 8 intersection points
*/
float Frustum::getMinCoordZ() const noexcept
{
	return std::min( { nearLL.z, nearLR.z, nearUR.z, nearUL.z,
					 farLL.z, farLR.z, farUR.z, farUL.z } );
}

/**
* @brief calculates a point of intersection of three planes
* @param frontOrBack either front or back frustum plane
* @param topOrBottom either top or bottom frustum plane
* @param rightOrLeft either left or right frustum plane
* @note see: https://en.wikipedia.org/wiki/Cramer%27s_rule
* @note It's programmer's responsibility to track chosen sides and their order promoted to this function
*/
glm::vec3 Frustum::kramerIntersection( const glm::vec4 & frontOrBack, 
									   const glm::vec4 & topOrBottom, 
									   const glm::vec4 & rightOrLeft )
{
	glm::mat3 xyzCoefficientMatrix;
	xyzCoefficientMatrix[0] = glm::vec3( frontOrBack );
	xyzCoefficientMatrix[1] = glm::vec3( topOrBottom );
	xyzCoefficientMatrix[2] = glm::vec3( rightOrLeft );
	glm::mat3 wyzCoefficientMatrix = xyzCoefficientMatrix;
	wyzCoefficientMatrix[0][0] = -frontOrBack.w;
	wyzCoefficientMatrix[1][0] = -topOrBottom.w;
	wyzCoefficientMatrix[2][0] = -rightOrLeft.w;
	glm::mat3 xwzCoefficientMatrix = xyzCoefficientMatrix;
	xwzCoefficientMatrix[0][1] = -frontOrBack.w;
	xwzCoefficientMatrix[1][1] = -topOrBottom.w;
	xwzCoefficientMatrix[2][1] = -rightOrLeft.w;
	glm::mat3 xywCoefficientMatrix = xyzCoefficientMatrix;
	xywCoefficientMatrix[0][2] = -frontOrBack.w;
	xywCoefficientMatrix[1][2] = -topOrBottom.w;
	xywCoefficientMatrix[2][2] = -rightOrLeft.w;

	float xyzCoefficientMatrixDeterminant = glm::determinant( xyzCoefficientMatrix );
	float wyzCoefficientMatrixDeterminant = glm::determinant( wyzCoefficientMatrix );
	float xwzCoefficientMatrixDeterminant = glm::determinant( xwzCoefficientMatrix );
	float xywCoefficientMatrixDeterminant = glm::determinant( xywCoefficientMatrix );

	glm::vec3 intersectionPoint;
	intersectionPoint.x = wyzCoefficientMatrixDeterminant / xyzCoefficientMatrixDeterminant;
	intersectionPoint.y = xwzCoefficientMatrixDeterminant / xyzCoefficientMatrixDeterminant;
	intersectionPoint.z = xywCoefficientMatrixDeterminant / xyzCoefficientMatrixDeterminant;
	return intersectionPoint;
}

/**
* @brief helper function that checks whether given set of coordinates is inside (or close enough) the frustum
* @param x X coordinate of a point
* @param y Y coordinate of a point
* @param z Z coordinate of a point
* @param radius an offset applied to distance: if the point is outside the frustum,
* but its "outside" section length is less than radius, then the point considered to be inside the frustum anyway.
*/
bool Frustum::isInside( float x, 
						float y, 
						float z, 
						float radius ) const
{
	return std::all_of( planes.begin(), planes.end(), [&]( const glm::vec4 & plane ) noexcept
	{
		return plane.x * x + plane.y * y + plane.z * z + plane.w > -radius;
	} );
}

const glm::vec4 & Frustum::getPlane( FRUSTUM_PLANE plane ) const
{
	return planes[plane];
}
