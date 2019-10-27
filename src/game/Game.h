/*
 * Copyright 2019 Ilya Malgin
 * Game.h
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
 * Purpose: contains declaration for Game class
 * @version 0.1.0
 */

#pragma once

#include <GL/glew.h>
#include "Timer"
#include "KeyboardManager"
#include "SaveLoadManager"
#include "Camera"
#include "Scene"
#include "Options"
#include "ShaderManager"
#include "TextureLoader"
#include "TextureManager"
#include "TextManager"
#include "CoordinateSystemRenderer"
#include "FrustumRenderer"
#include "ShadowVolumeRenderer"
#include "ScreenFramebuffer"
#include "DepthmapFramebuffer"
#include "WaterReflectionFramebuffer"
#include "WaterRefractionFramebuffer"

#include <thread>
#include <memory>
#include <array>
#include <atomic>
#include <mutex>
#include <condition_variable>

class ScreenResolution;
class MouseInputManager;

/**
* @brief entry point for game itself. Responsible for initialization/finalization of all submodules, threads communication,
* updating events and game frame management
*/
class Game
{
public:
	Game( GLFWwindow * window, 
		  const ScreenResolution & screenResolution );
	virtual ~Game();
	void setup();
	void loop();
	bool setupHasCompleted() const;
	bool mouseCallbacksBound() const;
	void initializeMouseInputCallbacks();

private:
	void drawFrame( const glm::mat4 & projectionView );
	void drawFrustumVisualizations( const glm::mat4 & projectionView );
	void drawFrameReflection();
	void drawFrameRefraction( const glm::mat4 & projectionView );
	void recreate();
	void drawDepthmap();
	void saveState();
	void loadState();

	//context and hardware related
	/**
	* @note for now, screen resolution is supposed to be fixed during app workflow
	* @todo make it possible to change screen resolution "on the fly" (possibly with windowed mode option)
	*/
	const ScreenResolution & screenResolution;
	GLFWwindow * window;

	//frame management
	Timer CPU_timer;
	unsigned long updateCount;

	//camera and related stuff
	Camera camera;
	/**
	* @brief additional camera defining shadow regions, intended to be used only for visual debugging
	* @todo remove shadow camera in the release version of the game
	*/
	Camera shadowCamera;
	Frustum viewFrustum;
	/** @brief dedicated view frustum for hills used to perform custom frustum culling algorithm */
	Frustum cullingViewFrustum;
	std::array<Frustum, NUM_SHADOW_LAYERS> shadowRegionsFrustums;
	/** @note don't need the farthest shadow region frustum visualization, thus size is 2 instead of 3 */
	std::array<FrustumRenderer, NUM_SHADOW_LAYERS - 1> shadowRegionsFrustumsRenderers;
	glm::mat4 projection;
	/** @brief dedicated projection matrix used with culling frustum, has wider FOV */
	glm::mat4 cullingProjection;
	std::array<glm::mat4, NUM_SHADOW_LAYERS> shadowRegionsProjections;

	//options
	Options options;

	//graphics
	ShaderManager shaderManager;
	TextureLoader textureLoader;
	TextureManager textureManager;
	CoordinateSystemRenderer csRenderer;
	ScreenFramebuffer screenFramebuffer;
	DepthmapFramebuffer depthmapFramebuffer;
	WaterReflectionFramebuffer reflectionFramebuffer;
	WaterRefractionFramebuffer refractionFramebuffer;

	//world
	ShadowVolume shadowVolume;
	Scene scene;
	/** @todo remove this in the game release version*/
	ShadowVolumeRenderer shadowVolumeRenderer;
	SaveLoadManager saveLoadManager;
	const float PLANET_MOVE_SPEED;

	//input
	KeyboardManager keyboard;
	MouseInputManager & mouseInput;

	//GUI and text
	TextManager textManager;

	//multithreading
	void setupThreads();

	std::unique_ptr<std::thread> meshIndirectBufferUpdater;
	std::mutex modelIndirectUpdateThreadMutex;
	std::condition_variable modelsIndirectBufferNeedUpdateCV;
	std::atomic_bool modelsIndirectBufferPrepared;
	std::atomic_bool modelsIndirectBufferNeedUpdate;
	std::atomic_bool setupCompleted;
	std::atomic_bool mouseInputCallbacksInitialized;

	/** @note not used in any other threads, thus no need to declare as atomic */
	bool landIndirectBufferHasUpdated;
};
