/*
 * Copyright 2019 Ilya Malgin
 * Game.cpp
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
 * Purpose: contains definition for Game class
 * @version 0.1.0
 */

#include "Game"
#include "BindlessTextureManager"
#include "MouseInputManager"
#include "DirectoriesSettings"
#include "ScreenResolution"
#include "RendererState"
#include "Shader"

/**
* @brief plain ctor. Creates all the submodules, sets randomizer seed
* @param window window of the game
* @param screenResolution current resolution of the screen
*/
Game::Game(GLFWwindow *window, const ScreenResolution &screenResolution)
  :
    screenResolution(screenResolution),
    window(window),
	updateCount(0),
    camera(glm::vec3(0.0f, 12.0f, 0.0f)),
    shadowCamera(camera),
    shadowRegionsFrustumsRenderers({{shadowRegionsFrustums[0], shadowRegionsFrustums[1]}}),
    projection(glm::perspective(glm::radians(camera.getZoom()), screenResolution.getAspectRatio(), NEAR_PLANE, FAR_PLANE)),
    cullingProjection(glm::perspective(glm::radians(camera.getZoom() + 10.0f), screenResolution.getAspectRatio(), NEAR_PLANE, FAR_PLANE)),
    options(),
    shaderManager(),
    textureLoader(screenResolution),
    textureManager(textureLoader),
    csRenderer(&shaderManager.get(SHADER_COORDINATE_SYSTEM)),
    screenFramebuffer(textureManager, screenResolution, shaderManager),
    depthmapFramebuffer(textureManager),
    reflectionFramebuffer(textureManager),
    refractionFramebuffer(textureManager),
    shadowVolume(),
    scene(shaderManager, options, textureManager, screenResolution, shadowVolume),
    shadowVolumeRenderer(shadowVolume),
    saveLoadManager(scene, camera, shadowCamera),
    keyboard(window, camera, shadowCamera, options, scene.getSunFacade()),
    mouseInput(MouseInputManager::getInstance()),
    textManager(FONT_DIR + "font.fnt", FONT_DIR + "font.png", shaderManager.get(SHADER_FONT), screenResolution)
{
  srand(time(NULL));
  Model::bindTextureLoader(textureLoader);

  //setup shadow volume projections
  shadowRegionsProjections[0] = glm::perspective(glm::radians(camera.getZoom()), screenResolution.getAspectRatio(), NEAR_PLANE, SHADOW_DISTANCE_LAYER1);
  shadowRegionsProjections[1] = glm::perspective(glm::radians(camera.getZoom()), screenResolution.getAspectRatio(), SHADOW_DISTANCE_LAYER1, SHADOW_DISTANCE_LAYER2);
  shadowRegionsProjections[2] = glm::perspective(glm::radians(camera.getZoom()), screenResolution.getAspectRatio(), SHADOW_DISTANCE_LAYER2, FAR_PLANE);

  //threads stuff
  modelsIndirectBufferPrepared = false; 
  modelsIndirectBufferNeedUpdate = false;
  landIndirectBufferHasUpdated = false;
}

/**
* @brief waits for coroutine threads to finish, sends finalization commands to submodules
*/
Game::~Game()
{
  meshIndirectBufferUpdater->join();
  BenchmarkTimer::finish(updateCount);
  BindlessTextureManager::makeAllNonResident();
}

/**
* @brief initializes all the submodules states, sends initialization commands to framebuffers
*/
void Game::setup()
{
  BENCHMARK("Game: setup", false);
  Shader::cacheUniformsMode(true);
  RendererState::setInitialRenderingState(options[OPT_USE_MULTISAMPLING]);
  MouseInputManager::initialize(window, options, screenResolution, camera, shadowCamera);
  MouseInputManager::setCallbacks();
  scene.setup();
  BindlessTextureManager::makeAllResident();
  BindlessTextureManager::loadToShader(shaderManager.get(SHADER_MODELS_GOURAUD), BINDLESS_TEXTURE_MODEL);
  BindlessTextureManager::loadToShader(shaderManager.get(SHADER_MODELS_PHONG), BINDLESS_TEXTURE_MODEL);
  BindlessTextureManager::loadToShader(shaderManager.get(SHADER_LENS_FLARE), BINDLESS_TEXTURE_LENS_FLARE);
  setupThreads();
  shaderManager.setupConstantUniforms(screenResolution);
  screenFramebuffer.setup();
  depthmapFramebuffer.setup();
  reflectionFramebuffer.setup();
  refractionFramebuffer.setup();
}

/**
* @brief "main" method of the game. Manages entire workflow of the game
*/
void Game::loop()
{
  /*
  * view matrix is unique per update, thus should be recreated in each subsequent frame. 
  * projectionView also updates once per frame (projection matrix will probably always be constant) 
  * and then it is used in all necessary places
  */
  glm::mat4 view, projectionView;

  float timerDelta = CPU_timer.tick();

  {
    BENCHMARK("Game loop: process input and camera", true);
    keyboard.processInput();
    camera.updateViewDirection();
    camera.move(timerDelta, scene.getHillsFacade().getMap());

	//projection and view matrices
    if (!options[OPT_USE_SHADOW_CAMERA_MATRIX])
      view = camera.getViewMatrix();
    else
      view = shadowCamera.getViewMatrix();
    projectionView = projection * view;

	//frustums update
    viewFrustum.updateFrustum(projectionView);
    cullingViewFrustum.updateFrustum(cullingProjection * view);

    if (!options[OPT_SHADOW_CAMERA_FIXED])
      {
        shadowCamera.updateViewDirection();
        shadowCamera.move(timerDelta, scene.getHillsFacade().getMap());
      }
  }

  //ambience update
  scene.getSunFacade().move(timerDelta);
  scene.getSkysphereFacade().moveStarsSkysphere(timerDelta * PLANET_MOVE_SPEED);

  //update sky color
  glm::vec4 currentColor = glm::mix(NIGHT_SKY_COLOR, DAY_SKY_COLOR, glm::clamp(-scene.getSunFacade().getLightDir().y * 5, 0.0f, 1.0f));
  glClearColor(currentColor.r, currentColor.g, currentColor.b, currentColor.a);

  /*
  * we might have to wait until models indirect buffer coroutine thread is done, 
  * in case that thread is not ready yet we could update land chunks indirect buffer in parallel
  * and then just chill till second thread is ready. Otherwise explicitly update land indirect buffer after
  */
  landIndirectBufferHasUpdated = false;
  {
    BENCHMARK("Game loop: wait models DIBs ready", true);
    while(!modelsIndirectBufferPrepared && updateCount != 0)
      {
        if (!landIndirectBufferHasUpdated)
          {
            scene.getLandFacade().updateCellsIndirectBuffer(viewFrustum);
            landIndirectBufferHasUpdated = true;
          }
        else
          std::this_thread::yield();
      }
    modelsIndirectBufferPrepared = false;
  }
  if (!landIndirectBufferHasUpdated)
    scene.getLandFacade().updateCellsIndirectBuffer(viewFrustum);

  //world recreation routine
  if (options[OPT_RECREATE_TERRAIN_REQUEST])
    recreate();

  /*
  * by this time model indirect buffer updater thread has prepared data, so buffer them to GPU.
  * This also should be done before any draw call that uses that data, even draw call to depthmap
  */
  scene.getPlantsFacade().updateIndirectBufferData();

  //save some processing time by updating depthmap every two frames
  if (options[OPT_USE_SHADOWS] && updateCount % 2)
    drawDepthmap();

  /*
  * draw world reflection/refraction texture ONLY if any water was visible in PREVIOUS frame,
  * thus we don't have to wait for occlusion query result during this frame
  */
  if(scene.getWaterFacade().hasWaterInFrame())
    {
      reflectionFramebuffer.bindToViewport(FRAME_WATER_REFLECTION_WIDTH, FRAME_WATER_REFLECTION_HEIGHT);
      drawFrameReflection();
      refractionFramebuffer.bindToViewport(FRAME_WATER_REFRACTION_WIDTH, FRAME_WATER_REFRACTION_HEIGHT);
      drawFrameRefraction(projectionView);
      refractionFramebuffer.unbindToViewport(screenResolution.getWidth(), screenResolution.getHeight());
    }

  //render the whole scene onto appropriate FBO, depend on multisampling option
  bool multisamplingEnabled = options[OPT_USE_MULTISAMPLING];
  screenFramebuffer.bindAppropriateFBO(multisamplingEnabled);
  drawFrame(projectionView);
  screenFramebuffer.draw(multisamplingEnabled, options[OPT_USE_DOF]);

  //save/load routines
  if (options[OPT_SAVE_REQUEST])
    saveState();
  if (options[OPT_LOAD_REQUEST])
    loadState();

  //wait for buffer swapping
  {
    BENCHMARK("GLFW: swapBuffers", true);
    glfwSwapBuffers(window);
  }

  //frame is complete
  ++updateCount;
}

/**
* @brief prepares OpenGL state to new rendering cycle and manages rendering order of the "onscreen" elements (scene, gui etc.)
* @param projectionView "projection * view" matrix
*/
void Game::drawFrame(const glm::mat4 &projectionView)
{
  BENCHMARK("Game loop: draw frame", true);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, options[OPT_POLYGON_LINE] ? GL_LINE : GL_FILL);

  if (options[OPT_CSM_VISUALIZATION])
    drawFrustumVisualizations(projectionView);

  scene.drawWorld(projectionView,
                  projection * glm::mat4(camera.getViewMatrixMat3()),
                  viewFrustum,
                  cullingViewFrustum,
                  camera,
                  mouseInput);

  /*
  * after all mesh related draw calls we could start updating meshes indirect data buffers
  * start updating right after we've used it and before we need that data to be updated and buffered again
  */
  modelsIndirectBufferNeedUpdate = true;
  modelsIndirectBufferNeedUpdateCV.notify_all();

  if (options[OPT_DRAW_DEBUG_TEXT])
    {
      textManager.addDebugText(camera, options, mouseInput, scene.getSunFacade().getPosition(), CPU_timer.getFPS());
      textManager.drawText();
      csRenderer.draw(camera.getViewMatrixMat3(), screenResolution.getAspectRatio());
    }

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

/**
* @brief manages frustum visualization routine
* @param projectionView "projection * view" matrix
* @todo remove this function from the release version of the game
*/
void Game::drawFrustumVisualizations(const glm::mat4& projectionView)
{
  Shader& frustumShader = shaderManager.get(SHADER_FRUSTUM);
  frustumShader.use();
  frustumShader.setMat4("u_projectionView", projectionView);
  if (options[OPT_FRUSTUM_VISUALIZATION])
    {
      frustumShader.setInt("u_colorIndex", 3);
      shadowRegionsFrustumsRenderers[0].render();
      frustumShader.setInt("u_colorIndex", 4);
      shadowRegionsFrustumsRenderers[1].render();
    }
  if (options[OPT_EXPECTED_VOLUME_VISUALIZATION])
    {
      shadowVolumeRenderer.bufferExpectedVolumes();
      frustumShader.setInt("u_colorIndex", 6);
      shadowVolumeRenderer.renderExpectedVolume(0);
      frustumShader.setInt("u_colorIndex", 7);
      shadowVolumeRenderer.renderExpectedVolume(1);
      frustumShader.setInt("u_colorIndex", 8);
      shadowVolumeRenderer.renderExpectedVolume(2);
    }
  if (options[OPT_ACTUAL_VOLUME_VISUALIZATION])
    {
      shadowVolumeRenderer.bufferActualVolumes();
      frustumShader.setInt("u_colorIndex", 9);
      shadowVolumeRenderer.renderActualVolume(0);
      frustumShader.setInt("u_colorIndex", 10);
      shadowVolumeRenderer.renderActualVolume(1);
      frustumShader.setInt("u_colorIndex", 11);
      shadowVolumeRenderer.renderActualVolume(2);
      frustumShader.setInt("u_colorIndex", 0);
      shadowVolumeRenderer.renderLightSource(0);
      frustumShader.setInt("u_colorIndex", 1);
      shadowVolumeRenderer.renderLightSource(1);
      frustumShader.setInt("u_colorIndex", 2);
      shadowVolumeRenderer.renderLightSource(2);
    }
}

/**
* @brief manages OpenGL state to new world reflection rendering cycle and delegates draw reflection command to the scene
* @note world reflection rendering require reflected view matrix
*/
void Game::drawFrameReflection()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (options[OPT_USE_MULTISAMPLING])
    glDisable(GL_MULTISAMPLE);

  //for reflection rendering we need reflection view matrix
  glm::mat4 viewReflected = camera.getReflectionViewMatrix();
  scene.drawWorldReflection(projection * viewReflected,
                            projection * glm::mat4(glm::mat3(viewReflected)),
                            cullingViewFrustum,
                            camera);

  if (options[OPT_USE_MULTISAMPLING])
    glEnable(GL_MULTISAMPLE);
}

/**
* @brief manages OpenGL state to new world refraction rendering cycle and delegates draw refraction command to the scene
* @param projectionView "projection * view" matrix
*/
void Game::drawFrameRefraction(const glm::mat4 &projectionView)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (options[OPT_USE_MULTISAMPLING])
    glDisable(GL_MULTISAMPLE);

  scene.drawWorldRefraction(projectionView);

  if (options[OPT_USE_MULTISAMPLING])
    glEnable(GL_MULTISAMPLE);
}

/**
* @brief handles recreation routine
*/
void Game::recreate()
{
  BENCHMARK("Game loop: recreate", true);
  scene.recreate();
  options[OPT_RECREATE_TERRAIN_REQUEST] = false;
}

/**
* @brief manages depthmap rendering routine: everything needed to add shadows to the frame is done here
*/
void Game::drawDepthmap()
{
  glm::mat4 shadowView = shadowCamera.getViewMatrix();

  //update shadow regions view frustums
  for (unsigned int layerIndex = 0; layerIndex < NUM_SHADOW_LAYERS; layerIndex++)
    {
      shadowRegionsFrustums[layerIndex].updateFrustum(shadowRegionsProjections[layerIndex] * shadowView);
      shadowRegionsFrustums[layerIndex].calculateIntersectionPoints();
    }

  //update shadow volume
  {
    BENCHMARK("Shadow volume: update", true);
    shadowVolume.update(shadowRegionsFrustums, scene.getSunFacade());
  }

  //draw scene onto depthmap
  depthmapFramebuffer.bindToViewport(DEPTH_MAP_TEXTURE_WIDTH, DEPTH_MAP_TEXTURE_HEIGHT);
  scene.drawWorldDepthmap(options[OPT_GRASS_SHADOW]);
  depthmapFramebuffer.unbindToViewport(screenResolution.getWidth(), screenResolution.getHeight());
}

/**
* @brief handles file saving routine
* @todo make proper saving system (with GUI, file naming stuff and other user-friendly bullshit)
*/
void Game::saveState()
{
  saveLoadManager.saveToFile(SAVES_DIR + "testSave.txt");
  options[OPT_SAVE_REQUEST] = false;
}

/**
* @brief handles file loading routine. Explicitly sends load command to scene as it should recalculate its internal data
* @todo make proper loading system (with GUI, file naming stuff and other user-friendly bullshit)
*/
void Game::loadState()
{
  saveLoadManager.loadFromFile(SAVES_DIR + "testSave.txt");
  scene.load();
  options[OPT_LOAD_REQUEST] = false;
}

/**
* @brief initializes all game-coroutines threads
* @note although there is only one coroutine thread at the moment, things might have changed in a future
*/
void Game::setupThreads()
{
  meshIndirectBufferUpdater = std::make_unique<std::thread>([this]()
  {
      while(!glfwWindowShouldClose(window))
        {
          if (modelsIndirectBufferNeedUpdate)
            {
              BENCHMARK("(ST)Model: update meshes DIBs data", true);
              float cameraOnMapX = glm::clamp(camera.getPosition().x, -HALF_WORLD_WIDTH_F, HALF_WORLD_WIDTH_F);
              float cameraOnMapZ = glm::clamp(camera.getPosition().z, -HALF_WORLD_HEIGHT_F, HALF_WORLD_HEIGHT_F);
              glm::vec2 cameraPositionXZ(cameraOnMapX, cameraOnMapZ);
              scene.getPlantsFacade().prepareIndirectBufferData(cameraPositionXZ, viewFrustum);
              modelsIndirectBufferPrepared = true;
              modelsIndirectBufferNeedUpdate = false;
            }
          std::unique_lock<std::mutex> lock(modelIndirectUpdateThreadMutex);
          modelsIndirectBufferNeedUpdateCV.wait(lock, [this]() noexcept {return (bool)modelsIndirectBufferNeedUpdate || glfwWindowShouldClose(window);});
          lock.unlock();
        }
    });
}
