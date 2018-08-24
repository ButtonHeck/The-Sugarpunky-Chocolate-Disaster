#include "Game.h"

#ifdef _DEBUG
int ram_available, ram_size;
float ram_size_float_percentage;
#endif

Game::Game(GLFWwindow *window, glm::vec3 &cursorDir, Camera& camera, Options& options, int width, int height, float aspect)
  :
    scr_width(width),
    scr_height(height),
    aspect_ratio(aspect),
    window(window),
    cursorToViewportDirection(cursorDir),
    camera(camera),
    options(options),
    fontManager(new FontManager("Laconic_Bold.otf", glm::ortho(0.0f, (float)scr_width, 0.0f, (float)scr_height), &shaderManager.get(SHADER_FONT))),
    textureManager(new TextureManager(textureLoader, scr_width, scr_height))
{
  srand(time(NULL));
#ifdef _DEBUG
  glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &ram_size);
  ram_size_float_percentage = (float)ram_size / 100;
#endif
}

Game::~Game()
{
  waterAnimationThread->join();
  delete waterAnimationThread;
  meshIndirectUpdateThread->join();
  delete meshIndirectUpdateThread;
  textureManager->deleteTextures();
  shaderManager.deleteShaders();
  delete fontManager;
  delete textureManager;
  delete baseMapGenerator;
  delete hillMapGenerator;
  delete waterMapGenerator;
  delete saveLoadManager;
  delete buildableMapGenerator;
  delete treeGenerator;
}

void Game::setupVariables()
{
  Shader::cacheUniformsMode(SHADER_NO_CACHE);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_DITHER);
  if (options.get(MULTISAMPLE_ENABLE))
    glEnable(GL_MULTISAMPLE);
  else
    glDisable(GL_MULTISAMPLE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glfwSetCursorPosCallback(window, input.cursorCallback);
  glfwSetMouseButtonCallback(window, input.cursorClickCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  {
    BENCHMARK("Game: Loading models", false);
    Model tree1("/models/tree1/tree1.obj", textureLoader);
    Model tree1_2("/models/tree1_2/tree1_2.obj", textureLoader);
    Model tree2("/models/tree2/tree2.obj", textureLoader);
    Model tree2_2("/models/tree2_2/tree2_2.obj", textureLoader);
    Model tree3("/models/tree3/tree3.obj", textureLoader);
    Model tree3_2("/models/tree3_2/tree3_2.obj", textureLoader);
    Model tree4("/models/tree4/tree4.obj", textureLoader);
    Model tree5("/models/tree5/tree5.obj", textureLoader);
    Model tree5_2("/models/tree5_2/tree5_2.obj", textureLoader);
    Model tree6("/models/tree6/tree6.obj", textureLoader);
    Model tree6_2("/models/tree6_2/tree6_2.obj", textureLoader);
    Model tree7("/models/tree7/tree7.obj", textureLoader);
    Model tree8("/models/tree8/tree8.obj", textureLoader);
    Model hillTree1("/models/hillTree1/hillTree1.obj", textureLoader);
    Model hillTree2("/models/hillTree2/hillTree2.obj", textureLoader);
    Model hillTree3("/models/hillTree3/hillTree3.obj", textureLoader);
    Model hillTree4("/models/hillTree4/hillTree4.obj", textureLoader);
    Model hillTree5("/models/hillTree5/hillTree5.obj", textureLoader);
    Model hillTree6("/models/hillTree6/hillTree6.obj", textureLoader);
    Model hillTree7("/models/hillTree7/hillTree7.obj", textureLoader);
    Model hillTree8("/models/hillTree1/hillTree1.obj", textureLoader);
    Model hillTree9("/models/hillTree3/hillTree3.obj", textureLoader);
    Model hillTree10("/models/hillTree7/hillTree7.obj", textureLoader);
    Model hillTree11("/models/hillTree1/hillTree1.obj", textureLoader);
    Model hillTree12("/models/hillTree3/hillTree3.obj", textureLoader);
    Model hillTree13("/models/hillTree7/hillTree7.obj", textureLoader);
    treeGenerator = new TreeGenerator({tree1, tree1_2, tree2, tree2_2, tree3, tree3_2, tree4, tree5, tree5_2,
                                       tree6, tree6_2, tree7, tree8},
        {hillTree1, hillTree2, hillTree3, hillTree4, hillTree5, hillTree6, hillTree7,
         hillTree8, hillTree9, hillTree10, hillTree11, hillTree12, hillTree13});
  }
  saveLoadManager->setTreeGenerator(*treeGenerator);

  {
    BENCHMARK("Game: Prepare Terrain", false);
    prepareTerrain();
  }
  meshIndirectUpdateThread = new std::thread([this]()
  {
      auto& plainTrees = treeGenerator->getPlainTrees();
      auto& hillTrees = treeGenerator->getHillTrees();
      auto& plainChunks = treeGenerator->getTreeModelChunks();
      auto& hillChunks = treeGenerator->getHillTreeModelChunks();
      while(!glfwWindowShouldClose(window))
        {
          if (meshesIndirectDataNeed)
            {
              BENCHMARK("(ST)Model: update meshes DIBs data", true);
              float cameraOnMapX = glm::clamp(camera.getPosition().x, -(float)HALF_TILES_WIDTH, (float)HALF_TILES_WIDTH);
              float cameraOnMapZ = glm::clamp(camera.getPosition().z, -(float)HALF_TILES_HEIGHT, (float)HALF_TILES_HEIGHT);
              glm::vec2 cameraPositionXZ = glm::vec2(cameraOnMapX, cameraOnMapZ);
              for (unsigned int i = 0; i < plainTrees.size(); i++)
                {
                  Model& model = plainTrees[i];
                  model.prepareMeshesIndirectData(plainChunks, i, cameraPositionXZ, viewFrustum);
                }
              for (unsigned int i = 0; i < hillTrees.size(); i++)
                {
                  Model& model = hillTrees[i];
                  model.prepareMeshesIndirectData(hillChunks, i, cameraPositionXZ, viewFrustum);
                }
              meshesIndirectDataReady = true;
              meshesIndirectDataNeed = false;
            }
        }
    });
  waterAnimationThread = new std::thread([this]()
  {
      while(!glfwWindowShouldClose(window))
            {
              if (!options.get(RECREATE_TERRAIN_REQUEST) &&
                  options.get(ANIMATE_WATER) &&
                  options.get(RENDER_WATER))
                {
                  waterMapGenerator->updateAnimationFrame(options);
#ifdef _DEBUG
                  waterThreadAnimationIsWorking = true;
#endif
                }
              else
                {
#ifdef _DEBUG
                  waterThreadAnimationIsWorking = false;
#endif
                  std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        });

  textureManager->createUnderwaterReliefTexture(waterMapGenerator);
  shaderManager.setupConstantUniforms(scr_width, scr_height);
  prepareScreenVAO();
  prepareMS_FBO();
  prepareDepthMapFBO(&depthMapFBO, DEPTH_MAP_SUN);
  prepareDepthMapFBO(&depthMapFBO_camera, DEPTH_MAP_CAMERA);
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
  treeGenerator->setupPlainModels(baseMapGenerator->getMap(), hillMapGenerator->getMap());
  treeGenerator->setupHillModels(hillMapGenerator->getMap());
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

void Game::prepareDepthMapFBO(GLuint* fbo, GLuint depthTextureUnit)
{
  glGenFramebuffers(1, fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureManager->get(depthTextureUnit), 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Depthmap framebuffer is not complete\n";
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::prepareScreenVAO()
{
  glGenVertexArrays(1, &screenVAO);
  glGenBuffers(1, &screenVBO);
  glBindVertexArray(screenVAO);
  glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(SCREEN_VERTICES), &SCREEN_VERTICES, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Game::drawFrameToScreenRectangle(bool enableMS)
{
  if (enableMS)
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampleFBO);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, screenFBO);
      glBlitFramebuffer(0, 0, scr_width, scr_height, 0, 0, scr_width, scr_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
  else
    {
      glBindFramebuffer(GL_READ_FRAMEBUFFER, screenFBO);
      glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    }
  shaderManager.get(SHADER_MS_TO_DEFAULT).use();
  glBindVertexArray(screenVAO);
  glDisable(GL_DEPTH_TEST);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glEnable(GL_DEPTH_TEST);
}

void Game::drawFrameObjects(glm::mat4& projectionView)
{
  glm::vec3 viewPosition = camera.getPosition();

  if (options.get(ANIMATE_WATER))
    {
      BENCHMARK("Water: buffer animation frame", true);
      waterMapGenerator->bufferVertices();
    }

  //hills rendering
  shaderManager.updateHillsShaders(options.get(HILLS_FC), options.get(OCCLUSION_CULLING), options.get(SHADOW_ENABLE), projectionView, viewPosition, viewFrustum);
  {
    BENCHMARK("Renderer: draw hills", true);
    renderer.drawHills(options.get(HILLS_FC), hillMapGenerator, shaderManager.get(SHADER_HILLS_FC), shaderManager.get(SHADER_HILLS_NOFC));
  }

  //trees chunks rendering
  if (options.get(RENDER_TREE_MODELS))
    {
      shaderManager.updateModelShader(projectionView, viewPosition, options.get(RENDER_SHADOW_ON_TREES), options.get(SHADOW_ENABLE), options.get(OCCLUSION_CULLING));
      {
        BENCHMARK("Renderer: draw models", true);
        renderer.drawTrees(treeGenerator, shaderManager.get(SHADER_MODELS), options.get(MODELS_FC),
                           true, updateCount % MESH_INDIRECT_BUFFER_UPDATE_FREQ == 0);
      }
    }
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, 0);
  //reset texture units to terrain textures after we done with models
  glBindTextureUnit(FLAT, textureManager->get(FLAT));

  //shore terrain chunks drawing
  shaderManager.updateShoreShader(projectionView, options.get(SHADOW_ENABLE));
  {
    BENCHMARK("Renderer: draw shore", true);
    renderer.drawShore(baseMapGenerator);
  }

  //flat terrain chunks drawing
  if (options.get(RENDER_FLAT_TERRAIN))
    {
      shaderManager.updateFlatShader(projectionView, options.get(SHADOW_ENABLE));
      {
        BENCHMARK("Renderer: draw flat", true);
        renderer.drawFlatTerrain(baseMapGenerator, viewFrustum);
      }
    }

  //underwater tile
  shaderManager.updateUnderwaterShader(projectionView);
  renderer.drawUnderwaterQuad(&underwaterQuadGenerator);

  //buildable tiles
  if (options.get(SHOW_BUILDABLE))
    {
      shaderManager.updateBuildableShader(projectionView);
      {
        BENCHMARK("Renderer: draw buildable", true);
        renderer.drawBuildableTiles(buildableMapGenerator);
      }
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
          {
            BENCHMARK("Renderer: draw selected", true);
            renderer.drawSelectedTile(buildableMapGenerator);
          }
        }
    }

  //water rendering
  if (options.get(RENDER_WATER))
    {
      shaderManager.updateWaterShaders(options.get(WATER_FC), projectionView, viewPosition, viewFrustum);
      {
        BENCHMARK("Renderer: draw water (full func)", true);
        renderer.drawWater(options.get(WATER_FC), waterMapGenerator, shaderManager.get(SHADER_WATER_FC), shaderManager.get(SHADER_WATER_NOFC));
      }
    }

  //Skybox rendering
  glm::mat4 skyProjectionView = projection * glm::mat4(glm::mat3(camera.getViewMatrix()));
  shaderManager.updateSkyShader(skyProjectionView);
  renderer.drawSkybox(&skybox);

  //font rendering
  if (options.get(RENDER_DEBUG_TEXT))
    {
      {
        BENCHMARK("Renderer: draw text", true);
        glEnable(GL_BLEND);
        fontManager->renderText("CPU UPS: " + std::to_string(CPU_timer.getFPS()), 10.0f, (float)scr_height - 25.0f, 0.35f);
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
#ifdef _DEBUG
        fontManager->renderText("Water anim thread works: " + (waterThreadAnimationIsWorking ? std::string("On") : std::string("Off")), 10.0f, 70.0f, 0.35f);
        glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &ram_available);
        fontManager->renderText("RAM available: " + (std::to_string(ram_available)
                                                     .append(", ")
                                                     .append(std::to_string(ram_available / ram_size_float_percentage))
                                                     .append("%")), 10.0f, 90.0f, 0.35f);
#endif
      }
      glDisable(GL_BLEND);
      {
        BENCHMARK("Renderer: draw cs", true);
        csRenderer.draw(glm::mat3(camera.getViewMatrix()), aspect_ratio);
      }
    }
}

void Game::drawFrameObjectsDepthmap()
{
  glDisable(GL_CULL_FACE); //or set front face culling

  shaderManager.get(SHADER_SHADOW_TERRAIN).use();
  renderer.drawHillsDepthmap(hillMapGenerator);
  renderer.drawShore(baseMapGenerator);

  if (options.get(RENDER_TREE_MODELS))
    {
      shaderManager.get(SHADER_SHADOW_MODELS).use();
      {
        BENCHMARK("Renderer: draw models depthmap", true);
        renderer.drawTrees(treeGenerator, shaderManager.get(SHADER_SHADOW_MODELS), options.get(MODELS_FC),
                           false, updateCount % MESH_INDIRECT_BUFFER_UPDATE_FREQ == 0);
      }
    }

  glEnable(GL_CULL_FACE); //or set back face culling

  //reset texture units to terrain textures after we done with models
  glBindTextureUnit(FLAT, textureManager->get(FLAT));
}

void Game::drawFrameObjectsDepthMapCamera(glm::mat4 &projectionView)
{
  Shader* shader = &shaderManager.get(SHADER_SHADOW_TERRAIN_CAMERA);
  shader->use();
  shader->setMat4("u_lightSpaceMatrix", projectionView);
  renderer.drawHillsDepthmap(hillMapGenerator);

  if (options.get(RENDER_TREE_MODELS))
    {
      shader = &shaderManager.get(SHADER_SHADOW_MODELS_CAMERA);
      shader->use();
      shader->setMat4("u_lightSpaceMatrix", projectionView);
      renderer.drawTrees(treeGenerator, shaderManager.get(SHADER_SHADOW_MODELS_CAMERA), options.get(MODELS_FC),
                         false, updateCount % MESH_INDIRECT_BUFFER_UPDATE_FREQ == 0);
    }
}

void Game::loop()
{
  //even if we don't need to render models make sure we update indirect buffer data for meshes
  {
    BENCHMARK("Game: wait for mesh indirect ready", true);
    while(!meshesIndirectDataReady && !updateCount == 0 && meshesIndirectDataNeed) {}
  }
  meshesIndirectDataReady = false;

  {
    BENCHMARK("Input: process keyboard", true);
    input.processKeyboard();
    input.processKeyboardCamera(CPU_timer.tick(), hillMapGenerator->getMap());
  }

  //recreate routine
  if (options.get(RECREATE_TERRAIN_REQUEST))
    {
      delete waterMapGenerator;
      delete hillMapGenerator;
      delete baseMapGenerator;
      delete buildableMapGenerator;
      waterMapGenerator = new WaterMapGenerator(shaderManager.get(SHADER_WATER_FC));
      hillMapGenerator = new HillsMapGenerator(shaderManager.get(SHADER_HILLS_FC), waterMapGenerator->getMap());
      baseMapGenerator = new BaseMapGenerator(waterMapGenerator->getMap(), hillMapGenerator->getMap());
      buildableMapGenerator = new BuildableMapGenerator(baseMapGenerator->getMap(), hillMapGenerator->getMap());
      prepareTerrain();
      delete saveLoadManager;
      saveLoadManager = new SaveLoadManager(*baseMapGenerator, *hillMapGenerator, *waterMapGenerator, buildableMapGenerator, camera);
      saveLoadManager->setTreeGenerator(*treeGenerator);
      options.set(RECREATE_TERRAIN_REQUEST, false);
      textureManager->createUnderwaterReliefTexture(waterMapGenerator);
    }

  if ((options.get(CREATE_SHADOW_MAP_REQUEST) || updateCount % 16 == 0) && options.get(SHADOW_ENABLE))
    {
      glViewport(0, 0, DEPTH_MAP_TEXTURE_WIDTH, DEPTH_MAP_TEXTURE_HEIGHT);
      glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
      glClear(GL_DEPTH_BUFFER_BIT);
      drawFrameObjectsDepthmap();
      glViewport(0, 0, scr_width, scr_height);
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      options.set(CREATE_SHADOW_MAP_REQUEST, false);
    }

  //update view and projection matrices
  glm::mat4 view = camera.getViewMatrix();
  glm::mat4 projectionView = projection * view;
  viewFrustum.updateFrustum(projectionView);

  if (options.get(OCCLUSION_CULLING))
  {
    BENCHMARK("Game: draw occlusion map", true);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO_camera);
    glClear(GL_DEPTH_BUFFER_BIT);
    drawFrameObjectsDepthMapCamera(projectionView);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  /*
   * If the MULTISAMPLE_ENABLE option is true we only need to rebind fbo to use one with MS
   * because the msFBO is already configured to use multisample generated texture
   * with a MULTISAMPLES number of sampling (check Settings.h). If the MULTISAMPLE_ENABLE set to false
   * we just bind another one fbo without MS (that is also used as read buffer and then used for buffer blitting
   * to the default fbo IF the ms enabled), moreover, we do not need to do blitting,
   * because the fbo itself already contains all the data drawn into it
   * and it could be used by default fbo immediately
   */
  bool multisamplingEnabled = options.get(MULTISAMPLE_ENABLE);
  glBindFramebuffer(GL_FRAMEBUFFER, multisamplingEnabled ? multisampleFBO : screenFBO);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //render our world onto separate FBO as usual
  drawFrameObjects(projectionView);

  //after all mesh related draw calls we could start updating meshes indirect data buffers
  //start updating right after we've used it and before we need that data to be updated and buffered again
  meshesIndirectDataNeed = updateCount % MESH_INDIRECT_BUFFER_UPDATE_FREQ == 1;

  //render result onto the default FBO and apply HDR/MS if the flag are set
  {
    BENCHMARK("Game: draw frame to screen", true);
    drawFrameToScreenRectangle(multisamplingEnabled);
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

  {
    BENCHMARK("Game: glfwSwapBuffers", true);
    glfwSwapBuffers(window);
  }
  ++updateCount;
}
