/*
 * Copyright 2019 Ilya Malgin
 * SkysphereFacade.cpp
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
 * Purpose: contains definitions for SkysphereFacade class
 * @version 0.1.0
 */

#include "SkysphereFacade"

#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Constructor with hardcoded paths for (hemi)sphere models
 * @param renderShader compiled shader program fed to personal shader manager
 */
SkysphereFacade::SkysphereFacade( Shader & renderShader )
	: theSunAmbientLightingHemisphere( "environment/hemisphere/hemisphere.obj" )
	, starsSkysphere( "environment/sphere/sphere.obj", glm::rotate( glm::mat4(), glm::radians( 94.0f ), glm::vec3( 1.0f, 0.0f, -0.5f ) ) )
	, cloudsCylinder( "environment/clouds/clouds.obj" )
	, shader( renderShader )
{}

/**
 * @brief manages shader program object state changes during rendering and delegates draw call
 * invocations to skysphere member objects
 * @param transform transformation matrix to apply
 * @param projectionView Projection*View matrix
 * @param lightDir direction of the sunlight (directional lighting)
 */
void SkysphereFacade::draw( const glm::mat4 & transform,
							const glm::mat4 & projectionView,
							const glm::vec3 & lightDir )
{
	float sunPositionAttenuation = glm::clamp( ( -lightDir.y + 0.05 ) * 8, 0.0, 1.0 );
	shader.update( projectionView, lightDir, sunPositionAttenuation );

	//is the Sun is low enough bypass drawing ambient lighting 
	if( sunPositionAttenuation > 0.01 )
	{
		shader.setSkysphereType( SKYSPHERE_AMBIENT_LIGHTING, transform );
		theSunAmbientLightingHemisphere.draw();
	}

	shader.setSkysphereType( SKYSPHERE_CLOUDS, glm::mat4() );
	cloudsCylinder.draw();

	//if the Sun is high enough bypass drawing stars
	if( ( 1.0 - sunPositionAttenuation ) >= 0.01 )
	{
		shader.setSkysphereType( SKYSPHERE_STARS, starsSkysphere.getRotationTransform() );
		starsSkysphere.draw();
	}
}

void SkysphereFacade::moveStarsSkysphere( float angleDegrees )
{
	starsSkysphere.rotate( angleDegrees, SKYSPHERE_STARS_ROTATION );
}
