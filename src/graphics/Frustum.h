/*
 * Copyright 2019 Ilya Malgin
 * Frustum.h
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
 * Purpose: contains declaration for Frustum class
 * @version 0.1.0
 */

#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

enum FRUSTUM_PLANE
{
	FRUSTUM_RIGHT = 0,
	FRUSTUM_LEFT = 1,
	FRUSTUM_BACK = 2,
	FRUSTUM_FRONT = 3,
	FRUSTUM_BOTTOM = 4,
	FRUSTUM_TOP = 5
};

/**
* @brief view frustum representation. Containing information about its planes and their intersection points
*/
class Frustum
{
public:
	constexpr static unsigned int NUMBER_OF_DISTINCT_VERTICES = 8;
	constexpr static unsigned int NUMBER_OF_PLANES = 6;

	Frustum() noexcept;
	void updateFrustum( const glm::mat4 & projectionView );
	void calculateIntersectionPoints();
	float getMaxCoordX() const noexcept;
	float getMinCoordX() const noexcept;
	float getMaxCoordZ() const noexcept;
	float getMinCoordZ() const noexcept;
	bool isInside( float x, 
				   float y, 
				   float z, 
				   float radius ) const;
	const glm::vec4 & getPlane( FRUSTUM_PLANE plane ) const;

private:
	friend class ShadowVolume;
	friend class FrustumRenderer;

	void normalizePlane( FRUSTUM_PLANE plane );
	glm::vec3 kramerIntersection( const glm::vec4 & frontOrBack, 
								  const glm::vec4 & topOrBottom, 
								  const glm::vec4 & rightOrLeft );

	std::vector<glm::vec4> planes;
	//8 vertices defined in the world space represents intersection points of frustum planes
	//LL - low left (min X, max Z), LR - low right (max X, max Z), UR - up right (max X, min Z), UL - up left (min X, min Z)
	glm::vec3 nearLL;
	glm::vec3 nearLR;
	glm::vec3 nearUR;
	glm::vec3 nearUL;
	glm::vec3 farLL;
	glm::vec3 farLR;
	glm::vec3 farUR;
	glm::vec3 farUL;
};
