/*
 * Copyright 2019 Ilya Malgin
 * KeyboardManager.cpp
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
 * Purpose: contains definitions for KeyboardManager class
 * @version 0.1.0
 */

#include "KeyboardManager"
#include "Camera"
#include "TheSunFacade"

/** @todo remove this from release version of the game */
extern float debug_sunSpeed;

/**
* @brief plain ctor
* @param window window handler
* @param camera player's camera
* @param shadowCamera auxiliary camera defining shadow regions
* @param options set of options
* @param sun sun facade (used for debugging only)
*/
KeyboardManager::KeyboardManager( GLFWwindow * window, 
								  Camera & camera, 
								  Camera & shadowCamera, 
								  Options & options, 
								  TheSunFacade & sun ) noexcept
	: window( window )
	, camera( camera )
	, shadowCamera( shadowCamera )
	, options( options )
	, sun( sun )
{}

/**
* @brief walks through all the possible keys and processes binded events if a certain key is pressed/released
*/
void KeyboardManager::processInput()
{
	if( glfwGetKey( window, GLFW_KEY_ESCAPE ) == GLFW_PRESS )
	{
		glfwSetWindowShouldClose( window, GL_TRUE );
	}

	processKey( GLFW_KEY_F1, [&]() noexcept
	{
		camera.switchFPSmode();
		shadowCamera.switchFPSmode();
	} );
	processKey( GLFW_KEY_F3, OPT_ANIMATE_WATER );
	processKey( GLFW_KEY_F4, OPT_DRAW_TREES );
	processKey( GLFW_KEY_F5, OPT_DRAW_DEBUG_TEXT );
	processKey( GLFW_KEY_F6, OPT_DRAW_BUILDABLE );
	processKey( GLFW_KEY_F7, OPT_MODELS_PHONG_SHADING );
	processKey( GLFW_KEY_F8, OPT_SHOW_VRAM_AVAILABLE );
	processKey( GLFW_KEY_F9, [&]()
	{
		options[OPT_RECREATE_TERRAIN_REQUEST] = true;
	} );
	processKey( GLFW_KEY_F10, [&]()
	{
		options[OPT_SAVE_REQUEST] = true;
	} );
	processKey( GLFW_KEY_F11, [&]()
	{
		options[OPT_LOAD_REQUEST] = true;
	} );
	processKey( GLFW_KEY_T, OPT_HILLS_CULLING );
	processKey( GLFW_KEY_M, [&]()
	{
		options.toggle( OPT_USE_MULTISAMPLING );
		if( options[OPT_USE_MULTISAMPLING] )
		{
			glEnable( GL_MULTISAMPLE );
		}
		else
		{
			glDisable( GL_MULTISAMPLE );
		}
	} );
	processKey( GLFW_KEY_Q, OPT_USE_SHADOWS );
	processKey( GLFW_KEY_H, OPT_DRAW_LAND );
	processKey( GLFW_KEY_G, OPT_DRAW_WATER );
	processKey( GLFW_KEY_Y, OPT_DRAW_HILLS );
	processKey( GLFW_KEY_U, OPT_MODELS_LAND_BLENDING );
	processKey( GLFW_KEY_L, OPT_POLYGON_LINE );
	processKey( GLFW_KEY_P, OPT_DEBUG_RENDER );
	processKey( GLFW_KEY_C, OPT_USE_SHADOW_CAMERA_MATRIX );
	processKey( GLFW_KEY_Z, OPT_CSM_VISUALIZATION );
	processKey( GLFW_KEY_F, OPT_FRUSTUM_VISUALIZATION );
	processKey( GLFW_KEY_V, OPT_USE_VIGNETTE );
	processKey( GLFW_KEY_B, OPT_ACTUAL_VOLUME_VISUALIZATION );
	processKey( GLFW_KEY_J, OPT_USE_DOF );
	processKey( GLFW_KEY_K, OPT_GRASS_SHADOW );
	processKey( GLFW_KEY_X, [&]()
	{
		options.toggle( OPT_SHADOW_CAMERA_FIXED );
		if( !options[OPT_SHADOW_CAMERA_FIXED] )
		{
			shadowCamera = camera;
		}
	} );

	processKey( GLFW_KEY_KP_8, [&]()
	{
		camera.setYaw( -90.0f );
		shadowCamera.setYaw( -90.0f );
	} );
	processKey( GLFW_KEY_KP_6, [&]()
	{
		camera.setYaw( 0.0f );
		shadowCamera.setYaw( 0.0f );
	} );
	processKey( GLFW_KEY_KP_2, [&]()
	{
		camera.setYaw( 90.0f );
		shadowCamera.setYaw( 90.0f );
	} );
	processKey( GLFW_KEY_KP_4, [&]()
	{
		camera.setYaw( 180.0f );
		shadowCamera.setYaw( 180.0f );
	} );
	processKey( GLFW_KEY_KP_5, [&]()
	{
		camera.setPitch( -90.0f );
		shadowCamera.setPitch( -90.0f );
	} );
	processKey( GLFW_KEY_E, [&]() noexcept
	{
		camera.switchAcceleration();
		shadowCamera.switchAcceleration();
	} );
	processKey( GLFW_KEY_UP, []() noexcept
	{
		debug_sunSpeed += 0.5f;
	} );
	processKey( GLFW_KEY_DOWN, []() noexcept
	{
		debug_sunSpeed -= 0.5f;
	} );
	processKey( GLFW_KEY_KP_0, []() noexcept
	{
		debug_sunSpeed = 0.0f;
	} );

	//temporary debugging stuff
	processKey( GLFW_KEY_EQUAL, [&]()
	{
		debug_sunSpeed = 0.0f;
		sun.moveAbsolutePosition( 0.0f );
	} );
	processKey( GLFW_KEY_1, [&]()
	{
		debug_sunSpeed = 0.0f;
		sun.moveAbsolutePosition( 360.0f / 12 * 1 );
	} );
	processKey( GLFW_KEY_2, [&]()
	{
		debug_sunSpeed = 0.0f;
		sun.moveAbsolutePosition( 360.0f / 12 * 2 );
	} );
	processKey( GLFW_KEY_3, [&]()
	{
		debug_sunSpeed = 0.0f;
		sun.moveAbsolutePosition( 360.0f / 12 * 3 );
	} );
	processKey( GLFW_KEY_4, [&]()
	{
		debug_sunSpeed = 0.0f;
		sun.moveAbsolutePosition( 360.0f / 12 * 4 );
	} );
	processKey( GLFW_KEY_5, [&]()
	{
		debug_sunSpeed = 0.0f;
		sun.moveAbsolutePosition( 360.0f / 12 * 5 );
	} );
	processKey( GLFW_KEY_6, [&]()
	{
		debug_sunSpeed = 0.0f;
		sun.moveAbsolutePosition( 360.0f / 12 * 6 );
	} );
	processKey( GLFW_KEY_7, [&]()
	{
		debug_sunSpeed = 0.0f;
		sun.moveAbsolutePosition( 360.0f / 12 * 7 );
	} );
	processKey( GLFW_KEY_8, [&]()
	{
		debug_sunSpeed = 0.0f;
		sun.moveAbsolutePosition( 360.0f / 12 * 8 );
	} );
	processKey( GLFW_KEY_9, [&]()
	{
		debug_sunSpeed = 0.0f;
		sun.moveAbsolutePosition( 360.0f / 12 * 9 );
	} );
	processKey( GLFW_KEY_0, [&]()
	{
		debug_sunSpeed = 0.0f;
		sun.moveAbsolutePosition( 360.0f / 12 * 10 );
	} );
	processKey( GLFW_KEY_MINUS, [&]()
	{
		debug_sunSpeed = 0.0f;
		sun.moveAbsolutePosition( 360.0f / 12 * 11 );
	} );

	//process camera
	camera.disableMoveAcceleration();
	shadowCamera.disableMoveAcceleration();
	if( glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS &&
		glfwGetKey( window, GLFW_KEY_S ) != GLFW_PRESS )
	{
		camera.updateMoveAccelerations( FORWARD );
		shadowCamera.updateMoveAccelerations( FORWARD );
	}
	if( glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS &&
		glfwGetKey( window, GLFW_KEY_W ) != GLFW_PRESS )
	{
		camera.updateMoveAccelerations( BACKWARD );
		shadowCamera.updateMoveAccelerations( BACKWARD );
	}
	if( glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS &&
		glfwGetKey( window, GLFW_KEY_D ) != GLFW_PRESS )
	{
		camera.updateMoveAccelerations( LEFT );
		shadowCamera.updateMoveAccelerations( LEFT );
	}
	if( glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS &&
		glfwGetKey( window, GLFW_KEY_A ) != GLFW_PRESS )
	{
		camera.updateMoveAccelerations( RIGHT );
		shadowCamera.updateMoveAccelerations( RIGHT );
	}
	if( glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS &&
		glfwGetKey( window, GLFW_KEY_SPACE ) != GLFW_PRESS )
	{
		camera.updateMoveAccelerations( DOWN );
		shadowCamera.updateMoveAccelerations( DOWN );
	}
	if( glfwGetKey( window, GLFW_KEY_SPACE ) == GLFW_PRESS &&
		glfwGetKey( window, GLFW_KEY_LEFT_SHIFT ) != GLFW_PRESS )
	{
		camera.updateMoveAccelerations( UP );
		shadowCamera.updateMoveAccelerations( UP );
	}
}

/**
* @brief utility function that switches an option on/off
* @param keyCode virtual keyboard code
* @param option option unit to toggle
*/
void KeyboardManager::processKey( int keyCode, 
								  OPTION option )
{
	if( glfwGetKey( window, keyCode ) == GLFW_PRESS )
	{
		if( !keysPressed[keyCode] )
		{
			options.toggle( option );
			keysPressed[keyCode] = true;
		}
	}
	else if( glfwGetKey( window, keyCode ) == GLFW_RELEASE )
	{
		keysPressed[keyCode] = false;
	}
}

/**
* @brief utility function that invokes promoted argument function if related key was pressed
* @param keyCode virtual keyboard code
* @param function function object to be called
*/
void KeyboardManager::processKey( int keyCode, 
								  std::function<void()> function )
{
	if( glfwGetKey( window, keyCode ) == GLFW_PRESS )
	{
		if( !keysPressed[keyCode] )
		{
			function();
			keysPressed[keyCode] = true;
		}
	}
	else if( glfwGetKey( window, keyCode ) == GLFW_RELEASE )
	{
		keysPressed[keyCode] = false;
	}
}
