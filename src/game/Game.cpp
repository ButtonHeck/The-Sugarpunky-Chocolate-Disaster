#include "Game.h"

Game::Game(GLFWwindow *window, glm::vec3 &cursorDir, Camera& camera, Options& options, int width, int height, float aspect)
  :
    scr_width(width),
    scr_height(height),
    aspect_ratio(aspect),
    window(window),
    cursorToViewportDirection(cursorDir),
    camera(camera),
    options(options),
    waterMapGenerator(new WaterMapGenerator()),
    hillMapGenerator(new HillsMapGenerator(waterMapGenerator->getMap())),
    baseMapGenerator(new BaseMapGenerator(waterMapGenerator->getMap(), hillMapGenerator->getMap())),
    buildableMapGenerator(new BuildableMapGenerator(baseMapGenerator->getMap(), hillMapGenerator->getMap())),
    saveLoadManager(new SaveLoadManager(*baseMapGenerator, *hillMapGenerator, *waterMapGenerator, buildableMapGenerator)),
    fontManager(new FontManager("Laconic_Bold.otf", glm::ortho(0.0f, (float)scr_width, 0.0f, (float)scr_height), &shaderManager.get(SHADER_FONT))),
    textureManager(new TextureManager(textureLoader, scr_width, scr_height))
{
}

Game::~Game()
{
  textureManager->deleteTextures();
  delete textureManager;
  shaderManager.deleteShaders();
  delete baseMapGenerator;
  delete hillMapGenerator;
  delete waterMapGenerator;
  delete saveLoadManager;
  delete buildableMapGenerator;
  delete treeGenerator;
}

void Game::setupVariables()
{
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  if (options.get(MULTISAMPLE_ENABLE))
    glEnable(GL_MULTISAMPLE);
  else
    glDisable(GL_MULTISAMPLE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glfwSetCursorPosCallback(window, input.cursorCallback);
  glfwSetMouseButtonCallback(window, input.cursorClickCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  Model tree1("/models/tree1/tree1.obj", textureLoader);
  Model tree2("/models/tree2/tree2.obj", textureLoader);
  Model tree3("/models/tree3/tree3.obj", textureLoader);
  Model hillTree1("/models/hillTree1/hillTree1.obj", textureLoader);
  Model hillTree2("/models/hillTree2/hillTree2.obj", textureLoader);
  Model hillTree3("/models/hillTree3/hillTree3.obj", textureLoader);
  treeGenerator = new TreeGenerator({tree1, tree2, tree3}, {hillTree1, hillTree2, hillTree3});
  saveLoadManager->setTreeGenerator(*treeGenerator);

  prepareTerrain();
  treeGenerator->setupPlainModels(baseMapGenerator->getMap(), hillMapGenerator->getMap());
  treeGenerator->setupHillModels(hillMapGenerator->getMap());

  textureManager->createUnderwaterReliefTexture(waterMapGenerator);
  shaderManager.setupConstantUniforms();
  prepareScreenVAO();
  prepareMS_FBO();
  prepareDepthMapFBO();
}

void Game::prepareTerrain()
{
  waterMapGenerator->prepareMap(); //prepare water map
  hillMapGenerator->prepareMap(); //generating hill height map
  hillMapGenerator->fillBufferData(!HILLS_TEXTURE_MAPPING_SLOPE_CORRECTION); //fill hills buffer
  baseMapGenerator->prepareMap(BASE_TERRAIN_RANDOMIZE_SHORE_FORM); //generating base terrain data
  baseMapGenerator->fillShoreBufferData(); //fill base terrain vertex data
  baseMapGenerator->fillSquareBufferData(); //generating data for chunk instance rendering
  baseMapGenerator->fillCellBufferData(); //generating data for 1x1 tile instance rendering
  waterMapGenerator->postPrepareMap();
  waterMapGenerator->fillBufferData(); //fill water buffer
  buildableMapGenerator->prepareMap();
  buildableMapGenerator->fillBufferData();
}

void Game::prepareMS_FBO()
{
  //multisample
  glGenFramebuffers(1, &multisampleFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, multisampleFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureManager->get(FRAME_MS_TEXTURE), 0);
  GLuint msDepthRbo;
  glGenRenderbuffers(1, &msDepthRbo);
  glBindRenderbuffer(GL_RENDERBUFFER, msDepthRbo);
  glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTISAMPLES, GL_DEPTH24_STENCIL8, scr_width, scr_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msDepthRbo);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
    std::cout << "MS Framebuffer is not complete\n";
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //intermediate FBO (or direct off-screen FBO without multisampling)
  glGenFramebuffers(1, &screenFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         HDR_ENABLED ? textureManager->get(FRAME_HDR_TEXTURE) : textureManager->get(FRAME_TEXTURE), 0);
  //we don't need depth data if we use this FBO as intermediate, but we DO need it if theres no multisampling
  GLuint screenDepthRbo;
  glGenRenderbuffers(1, &screenDepthRbo);
  glBindRenderbuffer(GL_RENDERBUFFER, screenDepthRbo);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, scr_width, scr_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, screenDepthRbo);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Intermediate Framebuffer is not complete\n";
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::prepareDepthMapFBO()
{
  glGenFramebuffers(1, &depthMapFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureManager->get(DEPTH_MAP), 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Depthmap framebuffer is not complete\n";
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::prepareScreenVAO()
{
  float screenVertices[] = {
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
       1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
       1.0f,  1.0f, 0.0f, 1.0f, 1.0f,

       1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
      -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f
  };
  glGenVertexArrays(1, &screenVAO);
  glGenBuffers(1, &screenVBO);
  glBindVertexArray(screenVAO);
  glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), &screenVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Game::drawFrameToScreenRectangle(bool enableHDR, bool enableMS)
{
  if (enableMS)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleFBO);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, screenFBO);
      glBlitFramebuffer(0, 0, scr_width, scr_height, 0, 0, scr_width, scr_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
  else
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, screenFBO);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  if (enableHDR)
    shaderManager.get(SHADER_HDR).use();
  else
    shaderManager.get(SHADER_MS_TO_DEFAULT).use();
  glBindVertexArray(screenVAO);
  glDisable(GL_DEPTH_TEST);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glEnable(GL_DEPTH_TEST);
}

void Game::drawFrameObjects()
{
  glm::mat4 view = camera.getViewMatrix();
  glm::vec3 viewPosition = camera.getPosition();
  glm::mat4 projectionView = projection * view;
  viewFrustum.updateFrustum(projectionView);

  if (options.get(RECREATE_TERRAIN_REQUEST))
    {
      delete waterMapGenerator;
      delete hillMapGenerator;
      delete baseMapGenerator;
      delete buildableMapGenerator;
      waterMapGenerator = new WaterMapGenerator();
      hillMapGenerator = new HillsMapGenerator(waterMapGenerator->getMap());
      baseMapGenerator = new BaseMapGenerator(waterMapGenerator->getMap(), hillMapGenerator->getMap());
      buildableMapGenerator = new BuildableMapGenerator(baseMapGenerator->getMap(), hillMapGenerator->getMap());
      prepareTerrain();
      treeGenerator->setupPlainModels(baseMapGenerator->getMap(), hillMapGenerator->getMap());
      treeGenerator->setupHillModels(hillMapGenerator->getMap());
      delete saveLoadManager;
      saveLoadManager = new SaveLoadManager(*baseMapGenerator, *hillMapGenerator, *waterMapGenerator, buildableMapGenerator);
      saveLoadManager->setTreeGenerator(*treeGenerator);
      options.set(RECREATE_TERRAIN_REQUEST, false);
      textureManager->createUnderwaterReliefTexture(waterMapGenerator);
    }

  //hills rendering
  shaderManager.updateHillsShaders(options.get(HILLS_FC), options.get(SHADOW_ENABLE), projectionView, viewPosition, viewFrustum);
  renderer.drawHills(hillMapGenerator);

  //shore terrain chunks drawing
  shaderManager.updateShoreShader(projectionView, options.get(SHADOW_ENABLE));
  renderer.drawShore(baseMapGenerator, viewFrustum);

  //flat terrain chunks drawing
  shaderManager.updateFlatShader(projectionView, options.get(SHADOW_ENABLE));
  renderer.drawFlatTerrain(baseMapGenerator, viewFrustum);

  //underwater tile
  shaderManager.updateUnderwaterShader(projectionView);
  renderer.drawUnderwaterQuad(&underwaterQuadGenerator);

  //buildable tiles
  if (options.get(SHOW_BUILDABLE))
    {
      shaderManager.updateBuildableShader(projectionView);
      renderer.drawBuildableTiles(buildableMapGenerator);
    }

  //cursor selected tile
  if (options.get(SHOW_CURSOR))
    {
      input.updateCursorMappingCoordinates(camera, baseMapGenerator, hillMapGenerator, buildableMapGenerator);
      if (buildableMapGenerator->getMap()[input.getCursorMapZ()][input.getCursorMapX()] != 0)
        {
          glm::mat4 selectedModel;
          selectedModel = glm::translate(selectedModel, glm::vec3(-HALF_TILES_WIDTH + input.getCursorMapX(), 0.0f, -HALF_TILES_HEIGHT + input.getCursorMapZ()));
          shaderManager.updateSelectedShader(projectionView, selectedModel);
          renderer.drawSelectedTile(buildableMapGenerator);
        }
    }

  //water rendering
  shaderManager.updateWaterShaders(options.get(WATER_FC), projectionView, viewPosition, viewFrustum);
  renderer.drawWater(waterMapGenerator, options.get(ANIMATE_WATER));

  //Skybox rendering
  glm::mat4 skyView = glm::mat4(glm::mat3(view));
  shaderManager.updateSkyShader(skyView, projection);
  renderer.drawSkybox(&skybox);

  //trees chunks rendering
  if (options.get(RENDER_TREE_MODELS))
    {
      shaderManager.updateModelShader(projectionView, viewPosition, options.get(RENDER_SHADOW_ON_TREES), options.get(SHADOW_ENABLE));
      renderer.drawTrees(treeGenerator, shaderManager.get(SHADER_MODELS), options.get(MODELS_FC));
    }

  //font rendering
  if (options.get(RENDER_DEBUG_TEXT))
    {
      fontManager->renderText("FPS: " + std::to_string(timer.getFPS()), 10.0f, (float)scr_height - 25.0f, 0.35f);
      fontManager->renderText("Camera pos: " + std::to_string(viewPosition.x).substr(0,6) + ": "
                             + std::to_string(viewPosition.y).substr(0,6) + ": "
                             + std::to_string(viewPosition.z).substr(0,6), 10.0f, (float)scr_height - 45.0f, 0.35f);
      fontManager->renderText("Camera on map: " + std::to_string(camera.getMapCoordX()) + ": " + std::to_string(camera.getMapCoordZ()),
                             10.0f, (float)scr_height - 65.0f, 0.35f);
      fontManager->renderText("View dir: " + std::to_string(camera.getDirection().x).substr(0,6) + ": "
                             + std::to_string(camera.getDirection().y).substr(0,6) + ": "
                             + std::to_string(camera.getDirection().z).substr(0,6), 10.0f, (float)scr_height - 85.0f, 0.35f);
      fontManager->renderText("Cursor at: " + (!options.get(SHOW_CURSOR) ? "inactive" : (std::to_string(cursorToViewportDirection.x).substr(0,6) + ": "
                             + std::to_string(cursorToViewportDirection.y).substr(0,6) + ": "
                             + std::to_string(cursorToViewportDirection.z).substr(0,6))), 10.0f, (float)scr_height - 105.0f, 0.35f);
      fontManager->renderText("Cursor on map: " + (!options.get(SHOW_CURSOR) ? "inactive" : (std::to_string(input.getCursorMapX()) + ": "
                             + std::to_string(input.getCursorMapZ()-1) + ", " + input.getCursorTileName())),
                             10.0f, (float)scr_height - 125.0f, 0.35f);
      fontManager->renderText("Water culling: " + (options.get(WATER_FC) ? std::string("On") : std::string("Off")), 10.0f, 10.0f, 0.35f);
      fontManager->renderText("Hills culling: " + (options.get(HILLS_FC) ? std::string("On") : std::string("Off")), 10.0f, 30.0f, 0.35f);
      fontManager->renderText("Trees culling: " + (options.get(MODELS_FC) ? std::string("On") : std::string("Off")), 10.0f, 50.0f, 0.35f);
      csRenderer.draw(view, aspect_ratio);
    }

  //save/load routine
  if (options.get(SAVE_REQUEST))
    {
      saveLoadManager->saveToFile(RES_DIR + "/saves/testSave.txt");
      options.set(SAVE_REQUEST, false);
    }
  if (options.get(LOAD_REQUEST))
    {
      saveLoadManager->loadFromFile(RES_DIR + "/saves/testSave.txt");
      options.set(LOAD_REQUEST, false);
      textureManager->createUnderwaterReliefTexture(waterMapGenerator);
    }

  //reset texture units to terrain textures after we done with models and text
  glActiveTexture(GL_TEXTURE0 + FLAT);
  glBindTexture(GL_TEXTURE_2D, textureManager->get(FLAT));
}

void Game::drawFrameObjectsDepthmap()
{
  glm::mat4 view = camera.getViewMatrix();
  glm::mat4 projectionView = projection * view;
  viewFrustum.updateFrustum(projectionView);
  glDisable(GL_CULL_FACE); //or set front face culling

  //hills rendering
  shaderManager.get(SHADER_SHADOW_HILLS).use();
  renderer.drawHills(hillMapGenerator);

  //shore terrain chunks drawing
  shaderManager.get(SHADER_SHADOW_SHORE).use();
  renderer.drawShore(baseMapGenerator, viewFrustum);

  //flat terrain chunks drawing
  shaderManager.get(SHADER_SHADOW_FLAT).use();
  renderer.drawFlatTerrain(baseMapGenerator, viewFrustum);

  //water rendering
  shaderManager.get(SHADER_SHADOW_WATER).use();
  renderer.drawWater(waterMapGenerator, options.get(ANIMATE_WATER));

  //trees chunks rendering
  if (options.get(RENDER_TREE_MODELS))
    {
      shaderManager.get(SHADER_SHADOW_MODELS).use();
      renderer.drawTrees(treeGenerator, shaderManager.get(SHADER_MODELS), options.get(MODELS_FC));
    }

  glEnable(GL_CULL_FACE); //or set back face culling
  glActiveTexture(GL_TEXTURE0 + FLAT);
  glBindTexture(GL_TEXTURE_2D, textureManager->get(FLAT));
}

void Game::loop()
{
  /*
   * If the MULTISAMPLE_ENABLE option is true we only need to rebind fbo to use one with MS
   * because the msFBO is already configured to use multisample generated texture
   * with a MULTISAMPLES number of sampling (check Settings.h). If the MULTISAMPLE_ENABLE set to false
   * we just bind another one fbo without MS (that is also used as read buffer and then used for buffer blitting
   * to the default fbo IF the ms enabled), moreover, we do not need to do blitting,
   * because the fbo itself already contains all the data drawn into it
   * and it could be used by default fbo immediately
   */
  if ((options.get(CREATE_SHADOW_MAP_REQUEST) || frameCounter % 16 == 0) && options.get(SHADOW_ENABLE))
    {
      glViewport(0, 0, DEPTH_MAP_TEXTURE_WIDTH, DEPTH_MAP_TEXTURE_HEIGHT);
      glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
      glClear(GL_DEPTH_BUFFER_BIT);
      drawFrameObjectsDepthmap();
      glViewport(0, 0, scr_width, scr_height);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      options.set(CREATE_SHADOW_MAP_REQUEST, false);
    }

  bool multisamplingEnabled = options.get(MULTISAMPLE_ENABLE);
  if (multisamplingEnabled)
    glBindFramebuffer(GL_FRAMEBUFFER, multisampleFBO);
  else
    glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  float delta = timer.tick();
  input.processKeyboard(delta);

  drawFrameObjects();

  //render result onto the default FBO and apply HDR/MS if the flags are set
  drawFrameToScreenRectangle(HDR_ENABLED, multisamplingEnabled);

  glfwPollEvents();
  glfwSwapBuffers(window);
  ++frameCounter;
}
