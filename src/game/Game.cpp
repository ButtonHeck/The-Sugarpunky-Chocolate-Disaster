#include "game/Game.h"

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
    textureLoader(TextureLoader(screenResolution)),
    textureManager(TextureManager(textureLoader)),
    csRenderer(CoordinateSystemRenderer(&shaderManager.get(SHADER_COORDINATE_SYSTEM))),
    screenBuffer(screenResolution, textureManager, shaderManager),
    depthmapBuffer(),
    textManager(FONT_DIR + "font.fnt", FONT_DIR + "font.png", shaderManager.get(SHADER_FONT))
{
  srand(time(NULL));
  Model::bindTextureLoader(textureLoader);
  scene = std::make_shared<Scene>(shaderManager, options, textureManager);
  saveLoadManager = std::make_unique<SaveLoadManager>(scene, camera);
}

Game::~Game()
{
  waterAnimator->join();
  meshIndirectBufferUpdater->join();
  BenchmarkTimer::finish(updateCount);
}

void Game::setup()
{
  Shader::cacheUniformsMode(UNIFORMS_NO_CACHE);
  RendererStateManager::setInitialRenderingState(options.get(OPT_USE_MULTISAMPLING));
  MouseInputManager::setCallbacks(window);
  scene->setup();
  setupThreads();
  shaderManager.setupConstantUniforms(glm::ortho(0.0f, (float)screenResolution.getWidth(), 0.0f, (float)screenResolution.getHeight()));
  screenBuffer.setup();
  depthmapBuffer.setup(textureManager.get(TEX_DEPTH_MAP_SUN));
}

void Game::loop()
{
  keyboard.processInput(CPU_timer.tick(), scene->getHillsFacade().getMap());
  glm::mat4 view = camera.getViewMatrix();
  glm::mat4 projectionView = projection * view;
  viewFrustum.updateFrustum(projectionView);

  while(!meshBufferReady && !updateCount == 0)
    std::this_thread::yield();
  meshBufferReady = false;

  if (options.get(OPT_RECREATE_TERRAIN_REQUEST))
    recreate();

  if ((options.get(OPT_CREATE_SHADOW_MAP_REQUEST) || updateCount % 16 == 0) && options.get(OPT_USE_SHADOWS))
    updateDepthmap();

  //render our world onto separate FBO as usual
  bool multisamplingEnabled = options.get(OPT_USE_MULTISAMPLING);
  screenBuffer.bindAppropriateFBO(multisamplingEnabled);
  drawFrameObjects(projectionView);

  {
    BENCHMARK("Game: draw frame to screen", true);
    screenBuffer.draw(multisamplingEnabled);
  }

  if (options.get(OPT_SAVE_REQUEST))
    saveState();
  if (options.get(OPT_LOAD_REQUEST))
    loadState();

  {
    BENCHMARK("Game: glfwSwapBuffers", true);
    glfwSwapBuffers(window);
  }
  ++updateCount;
}

void Game::drawFrameObjects(glm::mat4& projectionView)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, options.get(OPT_POLYGON_LINE) ? GL_LINE : GL_FILL);

  if (options.get(OPT_ANIMATE_WATER))
    {
      BENCHMARK("Water: buffer animation frame", true);
      scene->getWaterFacade().bufferNewData();
      waterNeedNewKeyFrame = true;
    }
  scene->getPlantsFacade().updateIndirectBufferData();

  glm::mat4 skyboxProjectionView(projection * glm::mat4(glm::mat3(camera.getViewMatrix())));
  scene->drawWorld(projectionView,
                   skyboxProjectionView,
                   viewFrustum,
                   camera,
                   mouseInput);

  //after all mesh related draw calls we could start updating meshes indirect data buffers
  //start updating right after we've used it and before we need that data to be updated and buffered again
  meshBufferNeedUpdate = true;

  if (options.get(OPT_DRAW_DEBUG_TEXT))
    {
      textManager.addText(screenResolution,
                          camera,
                          options,
                          mouseInput,
                          CPU_timer.getFPS());
      textManager.drawText();
      csRenderer.draw(glm::mat3(camera.getViewMatrix()), screenResolution.getAspectRatio());
    }

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Game::recreate()
{
  while(!waterKeyFrameReady)
    std::this_thread::yield();//busy wait until water thread has done its business...and business is good
  waterNeedNewKeyFrame = false; //explicitly bypass water animation frame update routine
  scene->recreate();
  options.set(OPT_RECREATE_TERRAIN_REQUEST, false);
  waterNeedNewKeyFrame = true; //it's okay now to begin animating water
}

void Game::updateDepthmap()
{
  depthmapBuffer.bindToViewport(DEPTH_MAP_TEXTURE_WIDTH, DEPTH_MAP_TEXTURE_HEIGHT);
  scene->drawWorldDepthmap();
  depthmapBuffer.unbindToViewport(screenResolution.getWidth(), screenResolution.getHeight());
  options.set(OPT_CREATE_SHADOW_MAP_REQUEST, false);
}

void Game::saveState()
{
  saveLoadManager->saveToFile(SAVES_DIR + "testSave.txt");
  options.set(OPT_SAVE_REQUEST, false);
}

void Game::loadState()
{
  while(!waterKeyFrameReady)
    std::this_thread::yield(); //busy wait
  waterNeedNewKeyFrame = false;
  saveLoadManager->loadFromFile(SAVES_DIR + "testSave.txt");
  scene->load();
  options.set(OPT_LOAD_REQUEST, false);
  waterNeedNewKeyFrame = true;
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
              float cameraOnMapX = glm::clamp(camera.getPosition().x, -HALF_WORLD_WIDTH_F, HALF_WORLD_WIDTH_F);
              float cameraOnMapZ = glm::clamp(camera.getPosition().z, -HALF_WORLD_HEIGHT_F, HALF_WORLD_HEIGHT_F);
              glm::vec2 cameraPositionXZ = glm::vec2(cameraOnMapX, cameraOnMapZ);
              scene->getPlantsFacade().prepareMeshesIndirectData(cameraPositionXZ, viewFrustum);
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
                  scene->getWaterFacade().updateAnimationFrame(glfwGetTime(), options);
                  waterKeyFrameReady = true;
                  waterNeedNewKeyFrame = false;
                }
              std::this_thread::yield();
            }
        });
}
