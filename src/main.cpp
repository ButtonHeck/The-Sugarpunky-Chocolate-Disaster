/*
 * Copyright 2019 Ilya Malgin
 * main.cpp
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
 * Purpose: contains application entry point code and main() function
 * @version 0.1.0
 */

#include "Game"
#include "ScreenResolution"
#include "Logger"
#include "ResourceLoader"
#include "SettingsManager"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

/**
* @brief temporary global variable for debugging purposes
* @todo remove this from release version of the game and put it in the Sun submodule instead
*/
float debug_sunSpeed = 2.0f;

int main()
{
	//read settings
	SettingsManager::init( "config.ini" );

	//initialize GLFW stuff
	glfwSetErrorCallback( []( int,
							  const char * msg )
	{
		Logger::log( "Error with GLFW library: %\n", msg );
	} );
	if( !glfwInit() )
	{
		throw std::runtime_error( "Error while loading GLFW\n" );
	}
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
//#ifdef _DEBUG
	glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
//#endif
	GLFWmonitor * monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode * vidmode = glfwGetVideoMode( monitor );
	ScreenResolution screenResolution( vidmode->width, vidmode->height );
	GLFWwindow * window = glfwCreateWindow( screenResolution.getWidth(), screenResolution.getHeight(), "The Sugarpunky Chocolate Disaster", monitor, nullptr );
	glfwMakeContextCurrent( window );

	//initialize GLEW
	glewExperimental = GL_TRUE;
	glewInit();

//#ifdef _DEBUG
	Logger::log( "%\n", glfwGetVersionString() );
	GLint flags;
	glGetIntegerv( GL_CONTEXT_FLAGS, &flags );
	if( flags & GL_CONTEXT_FLAG_DEBUG_BIT )
	{
		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
		glDebugMessageCallback( Logger::glDebugCallback, nullptr );
		glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE );
	}
//#endif

	//explicitly make non-current from this thread, as the context will mainly be used in the game child thread
	glfwMakeContextCurrent( nullptr );

	//initialize resources before game has been created
	ResourceLoader::initialize();

	//we must keep pointer in this thread in order to keep track on time when game object is created and setup
	Game * game = nullptr;

	std::thread gameThread( [&]()
	{
		glfwMakeContextCurrent( window );
		/*
		 * compiler gives warning if I try to allocate Game object in stack memory,
		 * on the other hand, using smart pointer for this only because of warning is overkill,
		 * so, plain old new/delete is ok here
		 */
		game = new Game( window, screenResolution );
		game->setup();

		//the game thread should wait until mouse input callbacks are bound from the main thread explicitly
		while( !game->mouseCallbacksBound() )
		{
			std::this_thread::yield();
		}

		//it's ok to launch the game loop
		while( !glfwWindowShouldClose( window ) )
		{
			game->loop();
		}
		delete game;
	} );

	/**
	* mouse input callbacks should be bound within this thread, but those functions interoperate with Game member objects.
	* Thus, before any callback binding, we should wait until a game object itself is created 
	* and its member objects are created and initialized. 
	* Also, this explains the necessity to keep a pointer to a game object in this thread
	*/
	while( !game )
	{
		std::this_thread::yield();
	}
	while( !game->setupHasCompleted() )
	{
		std::this_thread::yield();
	}
	//it is safe now to bind mouse input callbacks
	game->initializeMouseInputCallbacks();

	//release resources after game has been created and initialized
	ResourceLoader::release();

	//don't know why pollEvents function is working as we nullified current context for this thread, but it works.
	while( !glfwWindowShouldClose( window ) )
	{
		glfwPollEvents();
		//~0.5ms delay of mouse/key input is applicable
		std::this_thread::sleep_for( std::chrono::microseconds( 500 ) );
	}
	gameThread.join();

	//cleanup
	glfwDestroyWindow( window );
	glfwTerminate();
}
