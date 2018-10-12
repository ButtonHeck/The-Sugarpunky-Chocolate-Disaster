#include "Game.h"

int ramAvailable, ramSize;
float ramSizeFloatPercentage;

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
  Model::bindTextureLoader(textureLoader);
  worldFacade = std::make_shared<WorldGeneratorFacade>(shaderManager, renderer, options, textureManager);
  saveLoadManager = std::make_unique<SaveLoadManager>(worldFacade, camera);
  glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &ramSize);
  ramSizeFloatPercentage = (float)ramSize / 100;
}

Game::~Game()
{
  waterAnimator->join();
  meshIndirectBufferUpdater->join();
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
  worldFacade->setup();
  setupThreads();
  shaderManager.setupConstantUniforms(glm::ortho(0.0f, (float)screenResolution.getWidth(), 0.0f, (float)screenResolution.getHeight()));
  screenBuffer.setup();
  depthmapBuffer.setup(textureManager.get(TEX_DEPTH_MAP_SUN));
}

void Game::drawFrameObjects(glm::mat4& projectionView)
{
  if (options.get(OPT_POLYGON_LINE))
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  worldFacade->bufferWaterNewData();
  glm::mat4 skyboxProjectionView(projection * glm::mat4(glm::mat3(camera.getViewMatrix())));
  worldFacade->drawWorld(projectionView,
                         skyboxProjectionView,
                         viewFrustum,
                         camera,
                         mouseInput,
                         updateCount);

  //text rendering
  if (options.get(OPT_DRAW_DEBUG_TEXT))
    {
      glEnable(GL_BLEND);
      glDisable(GL_CULL_FACE);
      {
        BENCHMARK("Renderer: add and draw text", true);
        float scrHeight = (float)screenResolution.getHeight();
        glm::vec3 viewPosition = camera.getPosition();
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
        textRenderer.addText("Water anim thread works: " + (waterAnimatorIsWorking ? std::string("On") : std::string("Off")), 10.0f, 80.0f, 0.18f);
        glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &ramAvailable);
        textRenderer.addText("RAM available: " + (std::to_string(ramAvailable)
                                                     .append(", ")
                                                     .append(std::to_string(ramAvailable / ramSizeFloatPercentage))
                                                     .append("%")), 10.0f, 100.0f, 0.18f);
        textRenderer.addText("Models Phong: " + (options.get(OPT_MODELS_PHONG_SHADING) ? std::string("On") : std::string("Off")), 10.0f, 120.0f, 0.18f);
        textRenderer.drawText();
      }
      glDisable(GL_BLEND);
      glEnable(GL_CULL_FACE);
      csRenderer.draw(glm::mat3(camera.getViewMatrix()), screenResolution.getAspectRatio());
    }

  if (options.get(OPT_POLYGON_LINE))
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Game::loop()
{
  //even if we don't need to render models make sure we update indirect buffer data for meshes
  {
    BENCHMARK("Game: wait for mesh indirect ready", true);
    while(!meshBufferReady && !updateCount == 0 && meshBufferNeedUpdate)
      std::this_thread::yield();
  }
  meshBufferReady = false;

  keyboard.processInput(CPU_timer.tick(), worldFacade->getHillsGenerator()->getMap());

  //recreate routine
  if (options.get(OPT_RECREATE_TERRAIN_REQUEST))
    {
      while(!waterKeyFrameReady)
        std::this_thread::yield();//busy wait until water thread has done its business...and business is good
      waterNeedNewKeyFrame = false; //explicitly bypass water animation frame update routine
      worldFacade->recreate();
      options.set(OPT_RECREATE_TERRAIN_REQUEST, false);
      waterNeedNewKeyFrame = true; //it's okay now to begin animating water
    }

  if ((options.get(OPT_CREATE_SHADOW_MAP_REQUEST) || updateCount % 16 == 0) && options.get(OPT_USE_SHADOWS))
    {
      depthmapBuffer.bindToViewport(DEPTH_MAP_TEXTURE_WIDTH, DEPTH_MAP_TEXTURE_HEIGHT);
      glClear(GL_DEPTH_BUFFER_BIT);
      worldFacade->drawWorldDepthmap(updateCount);
      depthmapBuffer.unbindToViewport(screenResolution.getWidth(), screenResolution.getHeight());
      options.set(OPT_CREATE_SHADOW_MAP_REQUEST, false);
    }

  //update view and projection matrices
  glm::mat4 view = camera.getViewMatrix();
  glm::mat4 projectionView = projection * view;
  viewFrustum.updateFrustum(projectionView);

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
        std::this_thread::yield(); //busy wait
      waterNeedNewKeyFrame = false;
      saveLoadManager->loadFromFile(SAVES_DIR + "testSave.txt");
      worldFacade->load();
      options.set(OPT_LOAD_REQUEST, false);
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
              worldFacade->getPlantsGeneratorFacade()->prepareMeshesIndirectData(cameraPositionXZ, viewFrustum);
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
                  worldFacade->getWaterGenerator()->updateAnimationFrame(options);
                  waterKeyFrameReady = true;
                  waterAnimatorIsWorking = true;
                }
              else
                {
                  waterAnimatorIsWorking = false;
                  std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
              std::this_thread::yield();
            }
        });
}
