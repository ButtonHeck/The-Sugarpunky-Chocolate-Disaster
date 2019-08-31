/*
 * Copyright 2019 Ilya Malgin
 * Skysphere.h
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
 * Purpose: contains declaration for Skysphere class
 * @version 0.1.0
 */

#pragma once

#include "Model"

/**
 * @brief Represents a simple wrapper for sphere-like model, responsible for keeping and handling
 * its own transformation and .obj model used for actual rendering
 */
class Skysphere
{
public:
	Skysphere( const std::string & path, 
			   const glm::mat4 & initialTransform = glm::mat4() );
	void rotate( float angleDegrees, 
				 const glm::vec3 & rotationVector );
	void draw();
	const glm::mat4 & getRotationTransform() const noexcept;

private:
	Model model;
	glm::mat4 modelRotationTransform;
};
