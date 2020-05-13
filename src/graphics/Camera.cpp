/*
 * Copyright 2019 Ilya Malgin
 * Camera.cpp
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
 * Purpose: contains definition for Camera class
 * @version 0.1.0
 */

#include "Camera"
#include "BenchmarkTimer"
#include "Logger"
#include "SceneSettings"
#include "SettingsManager"
#include "Timer"

#include <iomanip>
#include <fstream>

/**
* @brief plain ctor with necessary preset values
* @param position initial world position of a camera
*/
Camera::Camera( glm::vec3 position )
	: CAMERA_WORLD_MIN_HEIGHT( SettingsManager::getFloat( "CAMERA", "min_height" ) )
	, CAMERA_WORLD_MAX_HEIGHT( SettingsManager::getFloat( "CAMERA", "max_height" ) )
	, MOVE_ACCELERATION_DAMPENING_FACTOR( SettingsManager::getFloat( "CAMERA", "move_acceleration_dampening_factor" ) )

	, zoom( SettingsManager::getFloat( "CAMERA", "fov" ) )
	, moveSpeed( SettingsManager::getFloat( "CAMERA", "move_speed" ) )
	, mouseSensitivity( SettingsManager::getFloat( "CAMERA", "initial_mouse_sensitivity" ) )
	, useAcceleration( true )
	, viewAccelerationSensitivity( SettingsManager::getFloat( "CAMERA", "initial_view_acceleration_sensitivity" ) )
	, viewAccelerationDampeningFactor( SettingsManager::getFloat( "CAMERA", "view_acceleration_dampening_factor" ) )
	, moveAccelerationSensitivity( SettingsManager::getFloat( "CAMERA", "initial_move_acceleration_sensitivity" ) )
	, yaw( INITIAL_YAW_ANGLE_DEGREES )
	, position( position )
{
	updateDirectionVectors();
}

/**
* @brief plain ctor that delegates construction to a vec3 version
*/
Camera::Camera( float x, 
				float y, 
				float z )
	: Camera( glm::vec3( x, y, z ) )
{}

/**
* @brief copy ctor. Must explicitly call main constructor to initialize non-copied variables
* @param copy camera object to copy state from
*/
Camera::Camera( const Camera & copy )
	: Camera( copy.position )
{
	operator=( copy );
}

/**
* @brief copy assignment operator, just calls update function at the end of copying
* @param rhs camera object to copy state from
*/
Camera & Camera::operator=( const Camera & rhs )
{
	if( this != &rhs )
	{
		this->zoom = rhs.zoom;
		this->mouseSensitivity = rhs.mouseSensitivity;
		this->useAcceleration = rhs.useAcceleration;
		this->firstPersonShooterMode = rhs.firstPersonShooterMode;
		this->yaw = rhs.yaw;
		this->pitch = rhs.pitch;
		this->position = rhs.position;
		this->front = rhs.front;
		this->viewAccelerationX = rhs.viewAccelerationX;
		this->viewAccelerationY = rhs.viewAccelerationY;
		this->viewAccelerationSensitivity = rhs.viewAccelerationSensitivity;
		this->viewAccelerationDampeningFactor = rhs.viewAccelerationDampeningFactor;
		this->moveAccelerationSide = rhs.moveAccelerationSide;
		this->moveAccelerationFront = rhs.moveAccelerationFront;
		this->moveAccelerationVertical = rhs.moveAccelerationVertical;
		this->accumulateMoveFront = rhs.accumulateMoveFront;
		this->accumulateMoveSide = rhs.accumulateMoveSide;
		this->accumulateMoveVertical = rhs.accumulateMoveVertical;
		this->updateDirectionVectors();
	}
	return *this;
}

/**
* @brief handles everything that is related with camera movement
* @param delta time slice form previous frame used to determine stable velocity
* @param hillsMap map of the hill tiles. Used to prevent camera collide with hills
*/
void Camera::move( float delta, 
				   const map2D_f & hillsMap )
{
	BENCHMARK( "Camera: move", true );
	float velocity = delta * moveSpeed;

	//Moving forward/backward
	if( moveAccelerationFront != 0.0f )
	{
		if( !accumulateMoveFront )
		{
			diminishMoveAcceleration( moveAccelerationFront, delta );
		}
		//this kind of movement might be different from others, depending on FPS mode
		moveCameraFrontAxial( velocity );
	}

	//Moving left/right
	if( moveAccelerationSide != 0.0f )
	{
		if( !accumulateMoveSide )
		{
			diminishMoveAcceleration( moveAccelerationSide, delta );
		}
		glm::vec3 move = right * velocity * moveAccelerationSide;
		position += move;
	}

	//Moving up/down
	if( moveAccelerationVertical != 0.0f )
	{
		if( !accumulateMoveVertical )
		{
			diminishMoveAcceleration( moveAccelerationVertical, delta );
		}
		glm::vec3 move = WORLD_UP * velocity * moveAccelerationVertical;
		position += move;
	}

	//Limit position if camera goes out of bounds
	position.x = glm::clamp( position.x,
							 -HALF_WORLD_WIDTH + CAMERA_WORLD_BORDER_OFFSET,
							 HALF_WORLD_WIDTH - CAMERA_WORLD_BORDER_OFFSET );
	position.y = glm::clamp( position.y,
							 CAMERA_WORLD_MIN_HEIGHT,
							 CAMERA_WORLD_MAX_HEIGHT );
	position.z = glm::clamp( position.z,
							 -HALF_WORLD_HEIGHT + CAMERA_WORLD_BORDER_OFFSET,
							 HALF_WORLD_HEIGHT - CAMERA_WORLD_BORDER_OFFSET );

	//Prevent collision with hills
	//Beware of float truncation!
	if( hillsMap[position.z + HALF_WORLD_HEIGHT][position.x + HALF_WORLD_WIDTH] > 0 ||
		hillsMap[position.z + HALF_WORLD_HEIGHT + 1][position.x + HALF_WORLD_WIDTH] > 0 ||
		hillsMap[position.z + HALF_WORLD_HEIGHT][position.x + HALF_WORLD_WIDTH + 1] > 0 ||
		hillsMap[position.z + HALF_WORLD_HEIGHT + 1][position.x + HALF_WORLD_WIDTH + 1] > 0 )
	{
		//calculate tile bounds
		int x1 = std::floor( position.x + HALF_WORLD_WIDTH );
		int x2 = std::floor( position.x + HALF_WORLD_WIDTH + 1 );
		int z1 = std::floor( position.z + HALF_WORLD_HEIGHT );
		int z2 = std::floor( position.z + HALF_WORLD_HEIGHT + 1 );
		float xRatio = position.x + HALF_WORLD_WIDTH - x1;
		float zRatio = position.z + HALF_WORLD_HEIGHT - z1;

		//get hill tile corners height values
		float x1z1Height = hillsMap[z1][x1];
		float x2z1Height = hillsMap[z1][x2];
		float x1z2Height = hillsMap[z2][x1];
		float x2z2Height = hillsMap[z2][x2];

		//some kind of bilinear filtering applied
		float x1HeightMix = glm::mix( x1z1Height, x1z2Height, zRatio );
		float x2HeightMix = glm::mix( x2z1Height, x2z2Height, zRatio );
		float medianHeight = glm::mix( x1HeightMix, x2HeightMix, xRatio );
		position.y = glm::max( position.y, medianHeight + CAMERA_WORLD_MIN_HEIGHT );
	}
}

/**
* @brief accumulates acceleration of view vector
* @param xOffset accumulation value for X component
* @param yOffset accumulation value for Y component
*/
void Camera::updateViewAcceleration( float xOffset, 
									 float yOffset ) noexcept
{
	viewAccelerationX += xOffset * mouseSensitivity;
	viewAccelerationY += yOffset * mouseSensitivity;
}

/**
* @brief process view direction vector's transition tick: update Euler angles with acceleration values
* and damp acceleration itself (or set to 0 if no acceleration is used at all)
* @param frameDelta frame tick time used to adjust view dampening ratio depend on current average framerate
*/
void Camera::updateViewDirection( float frameDelta )
{
	BENCHMARK( "Camera: update view direction", true );
	yaw -= viewAccelerationX;
	pitch -= viewAccelerationY;
	if( pitch >= MAX_PITCH )
	{
		pitch = MAX_PITCH;
	}
	if( pitch <= MIN_PITCH )
	{
		pitch = MIN_PITCH;
	}

	const double CURRENT_FRAME_VIEW_DAMPENING_FACTOR = glm::pow( viewAccelerationDampeningFactor, frameDelta / Timer::FRAME_TICK_TIME_60_FPS );
	viewAccelerationX *= useAcceleration ? CURRENT_FRAME_VIEW_DAMPENING_FACTOR : 0;
	viewAccelerationY *= useAcceleration ? CURRENT_FRAME_VIEW_DAMPENING_FACTOR : 0;

	updateDirectionVectors();
}

/**
* @brief tweaks mouse sensitivity
* @param yOffset measure of adjustment
*/
void Camera::adjustMouseSensitivity( float yOffset )
{
	mouseSensitivity += yOffset * viewAccelerationSensitivity;
	mouseSensitivity = glm::clamp( mouseSensitivity, MIN_MOUSE_SENSITIVITY, MAX_MOUSE_SENSITIVITY );
}

/**
* @brief process movement acceleration tick: for each axis and both directions check if need to accumulate acceleration
* @param dir direction of current movement
* @param frameDelta frame tick time used to adjust move acceleration value depend on current average framerate
*/
void Camera::updateMoveAccelerations( CAMERA_MOVE_DIRECTION dir,
									  float frameDelta )
{
	const double CURRENT_FRAME_MOVE_ACCELERATION_VALUE = moveAccelerationSensitivity * ( frameDelta / Timer::FRAME_TICK_TIME_60_FPS );

	//forward / backward
	if( dir == FORWARD )
	{
		accumulateMoveFront = true;
		if( !useAcceleration )
		{
			moveAccelerationFront = 1.0f;
		}
		else
		{
			moveAccelerationFront += CURRENT_FRAME_MOVE_ACCELERATION_VALUE;
			moveAccelerationFront = glm::min( moveAccelerationFront, 1.0f );
		}
	}
	else if( dir == BACKWARD )
	{
		accumulateMoveFront = true;
		if( !useAcceleration )
		{
			moveAccelerationFront = -1.0f;
		}
		else
		{
			moveAccelerationFront -= CURRENT_FRAME_MOVE_ACCELERATION_VALUE;
			moveAccelerationFront = glm::max( moveAccelerationFront, -1.0f );
		}
	}

	//right / left
	if( dir == RIGHT )
	{
		accumulateMoveSide = true;
		if( !useAcceleration )
		{
			moveAccelerationSide = 1.0f;
		}
		else
		{
			moveAccelerationSide += CURRENT_FRAME_MOVE_ACCELERATION_VALUE;
			moveAccelerationSide = glm::min( moveAccelerationSide, 1.0f );
		}
	}
	else if( dir == LEFT )
	{
		accumulateMoveSide = true;
		if( !useAcceleration )
		{
			moveAccelerationSide = -1.0f;
		}
		else
		{
			moveAccelerationSide -= CURRENT_FRAME_MOVE_ACCELERATION_VALUE;
			moveAccelerationSide = glm::max( moveAccelerationSide, -1.0f );
		}
	}

	//up / down
	if( dir == UP )
	{
		accumulateMoveVertical = true;
		if( !useAcceleration )
		{
			moveAccelerationVertical = 1.0f;
		}
		else
		{
			moveAccelerationVertical += CURRENT_FRAME_MOVE_ACCELERATION_VALUE;
			moveAccelerationVertical = glm::min( moveAccelerationVertical, 1.0f );
		}
	}
	else if( dir == DOWN )
	{
		accumulateMoveVertical = true;
		if( !useAcceleration )
		{
			moveAccelerationVertical = -1.0f;
		}
		else
		{
			moveAccelerationVertical -= CURRENT_FRAME_MOVE_ACCELERATION_VALUE;
			moveAccelerationVertical = glm::max( moveAccelerationVertical, -1.0f );
		}
	}
}

void Camera::setYaw( float newYaw )
{
	yaw = newYaw;
	updateDirectionVectors();
}

void Camera::setPitch( float newPitch )
{
	pitch = newPitch;
	updateDirectionVectors();
}

void Camera::switchFPSmode() noexcept
{
	firstPersonShooterMode = !firstPersonShooterMode;
}

void Camera::switchAcceleration() noexcept
{
	useAcceleration = !useAcceleration;
}

void Camera::disableMoveAcceleration() noexcept
{
	accumulateMoveSide = accumulateMoveFront = accumulateMoveVertical = false;
}

glm::mat4 Camera::getViewMatrix() const
{
	return glm::lookAt( position, position + front, WORLD_UP );
}

glm::mat3 Camera::getViewMatrixMat3() const
{
	return glm::mat3( getViewMatrix() );
}

glm::mat4 Camera::getReflectionViewMatrix() const
{
	glm::vec3 positionReflected( position.x, -position.y, position.z );
	glm::vec3 lookDirectionReflected( position + front );
	lookDirectionReflected.y *= -1;
	return glm::lookAt( positionReflected, lookDirectionReflected, WORLD_UP );
}

float Camera::getZoom() const noexcept
{
	return zoom;
}

const glm::vec3 & Camera::getPosition() const noexcept
{
	return position;
}

const glm::vec3 & Camera::getDirection() const noexcept
{
	return front;
}

const glm::vec3 & Camera::getRight() const noexcept
{
	return right;
}

const glm::vec3 & Camera::getUp() const noexcept
{
	return up;
}

/**
* @brief get current position vector in "map"-space integer coordinates (from 0 to "world width/height")
*/
glm::ivec2 Camera::getWorldCoordinates() const
{
	using glm::clamp;
	return glm::ivec2( clamp( (int)( WORLD_WIDTH + clamp( position.x, -HALF_WORLD_WIDTH_F, HALF_WORLD_WIDTH_F ) ) - HALF_WORLD_WIDTH, 0, WORLD_WIDTH - 1 ),
					   clamp( (int)( WORLD_HEIGHT + clamp( position.z, -HALF_WORLD_HEIGHT_F, HALF_WORLD_HEIGHT_F ) ) - HALF_WORLD_HEIGHT, 0, WORLD_HEIGHT - 1 ) );
}

/**
* @brief returns vector of both X and Y components of view acceleration if enabled, otherwise vector of very high acceleration value
* @note see note on DEFAULT_VIEW_ACCELERATION
*/
glm::vec2 Camera::getViewAcceleration() const
{
	return useAcceleration ? glm::vec2( viewAccelerationX, viewAccelerationY ) : glm::vec2( DEFAULT_VIEW_ACCELERATION );
}

/**
* @brief save all important state variables
* @param output file stream to write data to
*/
void Camera::serialize( std::ofstream & output )
{
	output << std::setprecision( 5 );
	output << position.x << " ";
	output << position.y << " ";
	output << position.z << " ";
	output << pitch << " ";
	output << yaw << " ";
	Logger::log( "camera serialized successfully\n" );
}

/**
* @brief load state variables and update others dependent on them
* @param input file stream to read data from
*/
void Camera::deserialize( std::ifstream & input )
{
	input >> position.x >> position.y >> position.z >> pitch >> yaw;
	updateDirectionVectors();
	Logger::log( "camera deserialized successfully\n" );
}

/**
* @brief calculates new front, right and up vectors based on pitch and yaw values
*/
void Camera::updateDirectionVectors()
{
	float x = std::cos( glm::radians( yaw ) ) * std::cos( glm::radians( pitch ) );
	float y = std::sin( glm::radians( pitch ) );
	float z = std::sin( glm::radians( yaw ) ) * std::cos( glm::radians( pitch ) );
	front = glm::normalize( glm::vec3( x, y, z ) );
	right = glm::normalize( glm::cross( front, WORLD_UP ) );
	/* for "up" vector no need to normalize it explicitly
	 * as the cross product of 2 orthogonal unit vectors (front and WORLD_UP) is already a unit length vector
	 */
	up = glm::cross( right, front );
}

/**
* @brief damp acceleration value of movement if acceleration is used, otherwise just set it to 0
* @param directionAccelerationValue reference to acceleration value to damp
* @param frameDelta frame tick time used to adjust move dampening ratio depend on current average framerate
*/
void Camera::diminishMoveAcceleration( float & directionAccelerationValue,
									   float frameDelta )
{
	const double CURRENT_FRAME_MOVE_DAMPENING_FACTOR = glm::pow( MOVE_ACCELERATION_DAMPENING_FACTOR, frameDelta / Timer::FRAME_TICK_TIME_60_FPS );
	if( !useAcceleration )
	{
		directionAccelerationValue = 0.0f;
	}
	else
	{
		directionAccelerationValue *= CURRENT_FRAME_MOVE_DAMPENING_FACTOR;
		if( glm::abs( directionAccelerationValue ) <= 0.01f )
		{
			directionAccelerationValue = 0.0f;
		}
	}
}

/**
* @brief process forward/backward movement event differently depending on FPS mode flag
* @param velocity movement velocity value
*/
void Camera::moveCameraFrontAxial( float velocity )
{
	float moveDistance = velocity * moveAccelerationFront;
	if( !firstPersonShooterMode )
	{
		position += front * moveDistance;
	}
	else
	{
		//don't move on Y if FPS mode is true 
		position.x += front.x * moveDistance;
		position.z += front.z * moveDistance;
	}
}
