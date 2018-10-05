#include "Game.h"

#ifdef _DEBUG
int ram_available, ram_size;
float ram_size_float_percentage;
#endif

Game::Game(GLFWwindow *window, Camera& camera, Options& options, ScreenResolution &screenResolution)
  :
    screenResolution(screenResolution),
    window(window),
    camera(camera),
    projection(glm::perspective(glm::radians(camera.getZoom()), screenResolution.getAspectRatio(), NEAR_PLANE, FAR_PLANE)),
    options(options),
    keyboard(KeyboardManager(window, camera, options)),
    mouseInput(MouseInputManager::getInstance()),
    shaderManager(),
    renderer(Renderer(camera)),
    textureLoader(TextureLoader(screenResolution)),
    textureManager(TextureManager(textureLoader)),
    csRenderer(CoordinateSystemRenderer(&shaderManager.get(SHADER_CS))),
    screenBuffer(screenResolution, textureManager, shaderManager),
    depthmapBuffer(),
    fontLoader(FONT_DIR + "font.fnt", FONT_DIR + "font.png"),
    textRenderer(fontLoader, shaderManager.get(SHADER_FONT))
{
  srand(time(NULL));
  waterMapGenerator = std::make_shared<WaterMapGenerator>(shaderManager.get(SHADER_WATER_CULLING));
  hillMapGenerator = std::make_shared<HillsMapGenerator>(shaderManager.get(SHADER_HILLS_CULLING), waterMapGenerator->getMap());
  baseMapGenerator = std::make_shared<BaseMapGenerator>(waterMapGenerator->getMap(), hillMapGenerator->getMap());
  buildableMapGenerator = std::make_shared<BuildableMapGenerator>(baseMapGenerator, hillMapGenerator);
  Model::bindTextureLoader(textureLoader);
  plantGeneratorFacade = std::make_shared<PlantGeneratorFacade>();
  saveLoadManager = std::make_unique<SaveLoadManager>(baseMapGenerator, hillMapGenerator, waterMapGenerator, buildableMapGenerator, plantGeneratorFacade, camera);
#ifdef _DEBUG
  glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &ram_size);
  ram_size_float_percentage = (float)ram_size / 100;
#endif
}

Game::~Game()
{
  waterAnimator->join();
  meshIndirectBufferUpdater->join();
  textureManager.deleteTextures();
  shaderManager.deleteShaders();
  BenchmarkTimer::finish(updateCount);
}

void Game::setupVariables()
{
  Shader::cacheUniformsMode(UNIFORMS_NO_CACHE);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_DITHER);
  if (options.get(OPT_USE_MULTISAMPLING))
    glEnable(GL_MULTISAMPLE);
  else
    glDisable(GL_MULTISAMPLE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glfwSetCursorPosCallback(window, MouseInputManager::cursorCallback);
  glfwSetMouseButtonCallback(window, MouseInputManager::cursorClickCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  {
    BENCHMARK("Game: Prepare Terrain", false);
    prepareTerrain();
  }
  setupThreads();
  textureManager.createUnderwaterReliefTexture(waterMapGenerator);
  shaderManager.setupConstantUniforms(glm::ortho(0.0f, (float)screenResolution.getWidth(), 0.0f, (float)screenResolution.getHeight()));
  screenBuffer.setup();
  depthmapBuffer.setup(textureManager.get(TEX_DEPTH_MAP_SUN));
}

void Game::prepareTerrain()
{
  waterMapGenerator->prepareMap();
  hillMapGenerator->prepareMap();
  hillMapGenerator->fillBufferData();
  baseMapGenerator->setup();
  waterMapGenerator->postPrepareMap();
  waterMapGenerator->fillBufferData();
  buildableMapGenerator->setup(baseMapGenerator, hillMapGenerator);
  plantGeneratorFacade->setup(baseMapGenerator->getMap(), hillMapGenerator->getMap());
}

void Game::drawFrameObjects(glm::mat4& projectionView)
{
  if (options.get(OPT_POLYGON_LINE))
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glm::vec3 viewPosition = camera.getPosition();

  if (options.get(OPT_ANIMATE_WATER))
    {
      BENCHMARK("Water: buffer animation frame", true);
      waterMapGenerator->bufferVertices();
    }

  //hills rendering
  shaderManager.updateHillsShaders(options.get(OPT_HILLS_CULLING), options.get(OPT_USE_SHADOWS), projectionView, viewPosition, viewFrustum, hillMapGenerator->getMaxHeight());
  {
    BENCHMARK("Renderer: draw hills", true);
    renderer.drawHills(options.get(OPT_HILLS_CULLING), hillMapGenerator, shaderManager.get(SHADER_HILLS_CULLING), shaderManager.get(SHADER_HILLS));
  }

  //flat terrain chunks drawing
  if (options.get(OPT_DRAW_FLAT_TERRAIN))
    {
      shaderManager.updateFlatShader(projectionView, options.get(OPT_USE_SHADOWS));
      {
        BENCHMARK("Renderer: draw flat", true);
        renderer.drawFlatTerrain(baseMapGenerator, viewFrustum, textureManager.get(TEX_FLAT));
      }
    }

  //underwater tile
  shaderManager.updateUnderwaterShader(projectionView);
  renderer.drawUnderwaterQuad(underwaterQuadGenerator);

  //shore terrain chunks drawing
  shaderManager.updateShoreShader(projectionView, options.get(OPT_USE_SHADOWS));
  {
    BENCHMARK("Renderer: draw shore", true);
    renderer.drawShore(baseMapGenerator);
  }

  //trees chunks rendering
  if (options.get(OPT_DRAW_TREES))
    {
      shaderManager.updateModelShader(projectionView, viewPosition,
                                      options.get(OPT_TREES_SHADOW_EMPHASIZE),
                                      options.get(OPT_USE_SHADOWS),
                                      options.get(OPT_MODELS_FLAT_BLENDING));
      {
        BENCHMARK("Renderer: draw models", true);
        renderer.drawPlants(plantGeneratorFacade,
                           options.get(OPT_MODELS_PHONG_SHADING) ? shaderManager.get(SHADER_MODELS_PHONG) : shaderManager.get(SHADER_MODELS),
                           options.get(OPT_MODELS_CULLING),
                           true,
                           updateCount % MESH_INDIRECT_BUFFER_UPDATE_FREQ == 0,
                           true,
                           options.get(OPT_MODELS_FLAT_BLENDING));
      }
    }

  //buildable tiles
  if (options.get(OPT_DRAW_BUILDABLE))
    {
      shaderManager.updateBuildableShader(projectionView);
      {
        BENCHMARK("Renderer: draw buildable", true);
        renderer.drawBuildableTiles(buildableMapGenerator);
      }
    }

  //cursor selected tile
  if (options.get(OPT_SHOW_CURSOR))
    {
      mouseInput.updateCursorMappingCoordinates(camera, baseMapGenerator, hillMapGenerator, buildableMapGenerator);
      if (buildableMapGenerator->getMap()[mouseInput.getCursorMapZ()][mouseInput.getCursorMapX()] != 0)
        {
          glm::mat4 selectedModel;
          selectedModel = glm::translate(selectedModel, glm::vec3(-HALF_WORLD_WIDTH + mouseInput.getCursorMapX(), 0.0f, -HALF_WORLD_HEIGHT + mouseInput.getCursorMapZ()));
          shaderManager.updateSelectedShader(projectionView, selectedModel);
          renderer.drawSelectedTile(buildableMapGenerator);
        }
    }

  //water rendering
  if (options.get(OPT_DRAW_WATER))
    {
      shaderManager.updateWaterShaders(options.get(OPT_WATER_CULLING), projectionView, viewPosition, viewFrustum);
      {
        BENCHMARK("Renderer: draw water (full func)", true);
        renderer.drawWater(options.get(OPT_WATER_CULLING), waterMapGenerator, shaderManager.get(SHADER_WATER_CULLING), shaderManager.get(SHADER_WATER));
      }
    }

  //Skybox rendering
  glm::mat4 skyProjectionView = projection * glm::mat4(glm::mat3(camera.getViewMatrix()));
  shaderManager.updateSkyShader(skyProjectionView);
  renderer.drawSkybox(&skybox);

  //font rendering
  if (options.get(OPT_DRAW_DEBUG_TEXT))
    {
      glEnable(GL_BLEND);
      glDisable(GL_CULL_FACE);
      {
        BENCHMARK("Renderer: add and draw text", true);
        float scrHeight = (float)screenResolution.getHeight();
        textRenderer.addText("CPU UPS: " + std::to_string(CPU_timer.getFPS()), 10.0f, scrHeight - 15.0f, 0.18f);
        textRenderer.addText("Camera pos: " + std::to_string(viewPosition.x).substr(0,6) + ": "
                               + std::to_string(viewPosition.y).substr(0,6) + ": "
                               + std::to_string(viewPosition.z).substr(0,6), 10.0f, scrHeight - 35.0f, 0.18f);
        textRenderer.addText("Camera on map: " + std::to_string(camera.getMapCoordX()) + ": " + std::to_string(camera.getMapCoordZ()),
                               10.0f, scrHeight - 55.0f, 0.18f);
        textRenderer.addText("View dir: " + std::to_string(camera.getDirection().x).substr(0,6) + ": "
                               + std::to_string(camera.getDirection().y).substr(0,6) + ": "
                               + std::to_string(camera.getDirection().z).substr(0,6), 10.0f, scrHeight - 75.0f, 0.18f);
        const glm::vec3& cursorToViewportDirection = mouseInput.getCursorToViewportDirection();
        textRenderer.addText("Cursor at: " + (!options.get(OPT_SHOW_CURSOR) ? "inactive" : (std::to_string(cursorToViewportDirection.x).substr(0,6) + ": "
                               + std::to_string(cursorToViewportDirection.y).substr(0,6) + ": "
                               + std::to_string(cursorToViewportDirection.z).substr(0,6))), 10.0f, scrHeight - 95.0f, 0.18f);
        textRenderer.addText("Cursor on map: " + (!options.get(OPT_SHOW_CURSOR) ? "inactive" : (std::to_string(mouseInput.getCursorMapX()) + ": "
                               + std::to_string(mouseInput.getCursorMapZ()-1) + ", " + mouseInput.getCursorTileName())),
                               10.0f, scrHeight - 115.0f, 0.18f);
        textRenderer.addText("Water culling: " + (options.get(OPT_WATER_CULLING) ? std::string("On") : std::string("Off")), 10.0f, 20.0f, 0.18f);
        textRenderer.addText("Hills culling: " + (options.get(OPT_HILLS_CULLING) ? std::string("On") : std::string("Off")), 10.0f, 40.0f, 0.18f);
        textRenderer.addText("Trees culling: " + (options.get(OPT_MODELS_CULLING) ? std::string("On") : std::string("Off")), 10.0f, 60.0f, 0.18f);
#ifdef _DEBUG
        textRenderer.addText("Water anim thread works: " + (waterAnimatorIsWorking ? std::string("On") : std::string("Off")), 10.0f, 80.0f, 0.18f);
        glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &ram_available);
        textRenderer.addText("RAM available: " + (std::to_string(ram_available)
                                                     .append(", ")
                                                     .append(std::to_string(ram_available / ram_size_float_percentage))
                                                     .append("%")), 10.0f, 100.0f, 0.18f);
        textRenderer.addText("Models Phong: " + (options.get(OPT_MODELS_PHONG_SHADING) ? std::string("On") : std::string("Off")), 10.0f, 120.0f, 0.18f);
#endif
        textRenderer.drawText();
      }
      glDisable(GL_BLEND);
      glEnable(GL_CULL_FACE);
      {
        BENCHMARK("Renderer: draw cs", true);
        csRenderer.draw(glm::mat3(camera.getViewMatrix()), screenResolution.getAspectRatio());
      }
    }

  if (options.get(OPT_POLYGON_LINE))
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Game::drawFrameObjectsDepthmap()
{
  glDisable(GL_CULL_FACE); //or set front face culling

  shaderManager.get(SHADER_SHADOW_TERRAIN).use();
  renderer.drawHillsDepthmap(hillMapGenerator);
  renderer.drawShore(baseMapGenerator);

  if (options.get(OPT_DRAW_TREES))
    {
      shaderManager.get(SHADER_SHADOW_MODELS).use();
      {
        BENCHMARK("Renderer: draw models depthmap", true);
        renderer.drawPlants(plantGeneratorFacade, shaderManager.get(SHADER_SHADOW_MODELS),
                           options.get(OPT_MODELS_CULLING),
                           false,
                           updateCount % MESH_INDIRECT_BUFFER_UPDATE_FREQ == 0,
                           false,
                           false);
      }
    }

  glEnable(GL_CULL_FACE); //or set back face culling
}

void Game::loop()
{
  //even if we don't need to render models make sure we update indirect buffer data for meshes
  {
    BENCHMARK("Game: wait for mesh indirect ready", true);
    while(!meshBufferReady && !updateCount == 0 && meshBufferNeedUpdate)
      {
        std::this_thread::yield();
      }
  }
  meshBufferReady = false;

  keyboard.processKeyboard();
  keyboard.processKeyboardCamera(CPU_timer.tick(), hillMapGenerator->getMap());

  //recreate routine
  if (options.get(OPT_RECREATE_TERRAIN_REQUEST))
    {
      while(!waterKeyFrameReady)
        {
          std::this_thread::yield();//busy wait until water thread has done its business...and business is good
        }
      waterNeedNewKeyFrame = false; //explicitly bypass water animation frame update routine
      baseMapGenerator.reset(new BaseMapGenerator(waterMapGenerator->getMap(), hillMapGenerator->getMap()));
      waterMapGenerator->initializeMap(waterMapGenerator->getMap());
      hillMapGenerator->initializeMap(hillMapGenerator->getMap());

      prepareTerrain();
      saveLoadManager->update(baseMapGenerator, hillMapGenerator, waterMapGenerator, buildableMapGenerator, plantGeneratorFacade, camera);
      options.set(OPT_RECREATE_TERRAIN_REQUEST, false);
      textureManager.createUnderwaterReliefTexture(waterMapGenerator);
      waterNeedNewKeyFrame = true; //it's okay now to begin animating water
    }

  if ((options.get(OPT_CREATE_SHADOW_MAP_REQUEST) || updateCount % 16 == 0) && options.get(OPT_USE_SHADOWS))
    {
      depthmapBuffer.bindToViewport(DEPTH_MAP_TEXTURE_WIDTH, DEPTH_MAP_TEXTURE_HEIGHT);
      glClear(GL_DEPTH_BUFFER_BIT);
      drawFrameObjectsDepthmap();
      depthmapBuffer.unbindToViewport(screenResolution.getWidth(), screenResolution.getHeight());
      options.set(OPT_CREATE_SHADOW_MAP_REQUEST, false);
    }

  //update view and projection matrices
  glm::mat4 view = camera.getViewMatrix();
  glm::mat4 projectionView = projection * view;
  viewFrustum.updateFrustum(projectionView);

  /*
   * If the MULTISAMPLE_ENABLE option is true we only need to rebind fbo to use one with MS
   * because the msFBO is already configured to use multisample generated texture
   * with a MULTISAMPLES number of sampling (check Settings.h). If the OPT_USE_MULTISAMPLING set to false
   * we just bind another one fbo without MS (that is also used as read buffer and then used for buffer blitting
   * to the default fbo IF the ms enabled), moreover, we do not need to do blitting,
   * because the fbo itself already contains all the data drawn into it
   * and it could be used by default fbo immediately
   */
  bool multisamplingEnabled = options.get(OPT_USE_MULTISAMPLING);
  screenBuffer.bindAppropriateFBO(multisamplingEnabled);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //render our world onto separate FBO as usual
  drawFrameObjects(projectionView);

  //after all mesh related draw calls we could start updating meshes indirect data buffers
  //start updating right after we've used it and before we need that data to be updated and buffered again
  meshBufferNeedUpdate = updateCount % MESH_INDIRECT_BUFFER_UPDATE_FREQ == 1;

  //render result onto the default FBO and apply HDR/MS if the flag are set
  {
    BENCHMARK("Game: draw frame to screen", true);
    screenBuffer.draw(multisamplingEnabled);
  }

  //save/load routine
  if (options.get(OPT_SAVE_REQUEST))
    {
      saveLoadManager->saveToFile(SAVES_DIR + "testSave.txt");
      options.set(OPT_SAVE_REQUEST, false);
    }
  if (options.get(OPT_LOAD_REQUEST))
    {
      while(!waterKeyFrameReady)
        {
          std::this_thread::yield(); //busy wait
        }
      waterNeedNewKeyFrame = false;
      saveLoadManager->loadFromFile(SAVES_DIR + "testSave.txt");
      options.set(OPT_LOAD_REQUEST, false);
      textureManager.createUnderwaterReliefTexture(waterMapGenerator);
      buildableMapGenerator->setup(baseMapGenerator, hillMapGenerator);
      waterNeedNewKeyFrame = true;
    }

  {
    BENCHMARK("Game: glfwSwapBuffers", true);
    glfwSwapBuffers(window);
  }
  ++updateCount;
}

void Game::setupThreads()
{
  meshIndirectBufferUpdater = std::make_unique<std::thread>([this]()
  {
      while(!glfwWindowShouldClose(window))
        {
          if (meshBufferNeedUpdate)
            {
              BENCHMARK("(ST)Model: update meshes DIBs data", true);
              float cameraOnMapX = glm::clamp(camera.getPosition().x, -(float)HALF_WORLD_WIDTH, (float)HALF_WORLD_WIDTH);
              float cameraOnMapZ = glm::clamp(camera.getPosition().z, -(float)HALF_WORLD_HEIGHT, (float)HALF_WORLD_HEIGHT);
              glm::vec2 cameraPositionXZ = glm::vec2(cameraOnMapX, cameraOnMapZ);
              plantGeneratorFacade->prepareMeshesIndirectData(cameraPositionXZ, viewFrustum);
              meshBufferReady = true;
              meshBufferNeedUpdate = false;
            }
          std::this_thread::yield();
        }
    });
  waterAnimator = std::make_unique<std::thread>([this]()
  {
      while(!glfwWindowShouldClose(window))
            {
              if (waterNeedNewKeyFrame &&
                  options.get(OPT_ANIMATE_WATER) &&
                  options.get(OPT_DRAW_WATER))
                {
                  waterKeyFrameReady = false;
                  waterMapGenerator->updateAnimationFrame(options);
                  waterKeyFrameReady = true;
#ifdef _DEBUG
                  waterAnimatorIsWorking = true;
#endif
                }
              else
                {
#ifdef _DEBUG
                  waterAnimatorIsWorking = false;
#endif
                  std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
              std::this_thread::yield();
            }
        });
}
