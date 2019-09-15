/*
 * Copyright 2019 Ilya Malgin
 * Camera.h
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
 * Purpose: contains declaration for Camera class
 * @version 0.1.0
 */

#pragma once

#include "TypeAliases"
#include "SceneSettings"

#include <glm/gtc/matrix_transform.hpp>

enum CAMERA_MOVE_DIRECTION
{
	FORWARD,
	BACKWARD,
	RIGHT,
	LEFT,
	UP,
	DOWN
};

/**
* @brief Representation of a virtual camera. Rotatable around X and Y axis (pitch and yaw angles accordingly).
* This camera might have both (optionally) movement and rotation inertia, by default inertia is on. It might also
* behave like first-person shooter camera with Y coordinate of move vector being unaffected by the view vector.
* By using mouse-wheel you can slow down or speed up acceleration (if inertia mode is used).
*/
class Camera
{
public:
	Camera( glm::vec3 position );
	Camera( float x, 
			float y, 
			float z );
	Camera( const Camera & copy );
	Camera & operator=( const Camera & rhs );
	Camera( Camera && ) = delete;
	Camera & operator=( Camera && ) = delete;

	//mutators and setters
	void move( float delta, 
			   const map2D_f & hillsMap );
	void updateViewAcceleration( float xOffset, 
								 float yOffset ) noexcept;
	void updateViewDirection();
	void adjustMouseSensitivity( float yOffset );
	void updateMoveAccelerations( CAMERA_MOVE_DIRECTION dir );
	void setYaw( float newYaw );
	void setPitch( float newPitch );
	void switchFPSmode() noexcept;
	void switchAcceleration() noexcept;
	void disableMoveAcceleration() noexcept;

	//getters
	glm::mat4 getViewMatrix() const;
	glm::mat3 getViewMatrixMat3() const;
	glm::mat4 getReflectionViewMatrix() const;
	float getZoom() const noexcept;
	const glm::vec3 & getPosition() const noexcept;
	const glm::vec3 & getDirection() const noexcept;
	const glm::vec3 & getRight() const noexcept;
	const glm::vec3 & getUp() const noexcept;
	glm::ivec2 getWorldCoordinates() const;
	glm::vec2 getViewAcceleration() const;

	//file saving/loading stuff
	void serialize( std::ofstream & output );
	void deserialize( std::ifstream & input );

private:
	const glm::vec3 WORLD_UP = glm::vec3( 0.0f, 1.0f, 0.0f );
	const float FOV = 60.0f;
	const float MOVE_SPEED = 8.0f;
	const float INITIAL_MOUSE_SENSITIVITY = 0.015f;
	const float MAX_MOUSE_SENSITIVITY = 0.02f;
	const float MIN_MOUSE_SENSITIVITY = 0.002f;
	const float INITIAL_VIEW_ACCELERATION_SENSITIVITY = 0.001f;
	const float VIEW_ACCELERATION_DAMPENING_FACTOR = 0.85f;
	/**@note this value is actually might be used during hills frustum culling*/
	const float DEFAULT_VIEW_ACCELERATION = 20.0f;
	const float INITIAL_MOVE_ACCELERATION_SENSITIVITY = 0.05f;
	const float MOVE_ACCELERATION_DAMPENING_FACTOR = 0.92f;
	const float INITIAL_YAW_ANGLE_DEGREES = -90.0f;
	const float MAX_PITCH = 89.9f;
	const float MIN_PITCH = -65.0f;
	const float CAMERA_WORLD_BORDER_OFFSET = 8.0f;
	const float CAMERA_WORLD_MIN_HEIGHT = 2.0f;
	const float CAMERA_WORLD_MAX_HEIGHT = 30.0f;

	void updateDirectionVectors();
	void diminishMoveAcceleration( float & directionAccelerationValue );
	void moveCameraFrontAxial( float velocity );

	//options
	float zoom;
	float moveSpeed;
	float mouseSensitivity;
	bool useAcceleration;
	float viewAccelerationX;
	float viewAccelerationY;
	float viewAccelerationSensitivity;
	float moveAccelerationSide;
	float moveAccelerationVertical;
	float moveAccelerationFront;
	float moveAccelerationSensitivity;
	bool accumulateMoveSide = false;
	bool accumulateMoveFront = false;
	bool accumulateMoveVertical = false;
	bool firstPersonShooterMode;

	//Euler angles
	float yaw;
	float pitch;

	//position vector
	glm::vec3 position;

	//direction vectors
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
};
