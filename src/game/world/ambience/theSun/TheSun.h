/*
 * Copyright 2019 Ilya Malgin
 * TheSun.h
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
 * Purpose: contains declaration for TheSun class
 * @version 0.1.0
 */

#pragma once

#include "BufferCollection"

#include <glm/gtx/rotate_vector.hpp>

/**
 * @brief Represents the Sun entity, contains its own buffer collection and transformations applied.
 * Responsible for keeping track of its position, direction of light and buffer data
 * @todo make (de)serialization routine for save/load
 */
class TheSun
{
public:
	TheSun() noexcept;
	void move( float angleDegrees );
	void moveAbsolutePosition( float angleDegrees );
	const glm::vec3 & getPosition() const noexcept;
	const glm::vec3 & getLightDir() const noexcept;
	const glm::mat4 & getRotationTransform() const noexcept;

private:
	//renderer would need access of the VAO to use
	friend class TheSunRenderer;

	const glm::vec3 ROTATION_VECTOR;
	BufferCollection basicGLBuffers;
	glm::mat4 rotationTransform;
	glm::vec3 currentPosition;
	glm::vec3 lightDirTo;

	//constants for absolute positioning
	const glm::vec3 START_POSITION = currentPosition;
	const glm::mat4 START_MODEL = glm::mat4();
};
