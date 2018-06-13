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
    fontManager(new FontManager("Laconic_Bold.otf", glm::ortho(0.0f, (float)scr_width, 0.0f, (float)scr_height), &shaderManager.get(SHADER_FONT)))
{
}

Game::~Game()
{
  textureManager.deleteTextures();
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

  textureManager.createUnderwaterReliefTexture(waterMapGenerator);
  shaderManager.setupConstantUniforms();
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

void Game::loop()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  float delta = timer.tick();
  input.processKeyboard(delta);
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
      textureManager.createUnderwaterReliefTexture(waterMapGenerator);
    }

  //hills rendering
  shaderManager.updateHillsShaders(options.get(HILLS_FC), projectionView, viewPosition, viewFrustum);
  renderer.drawHills(hillMapGenerator);

  //shore terrain chunks drawing
  shaderManager.updateShoreShader(projectionView);
  renderer.drawShore(baseMapGenerator, viewFrustum);

  //flat terrain chunks drawing
  shaderManager.updateFlatShader(projectionView);
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
      shaderManager.updateModelShader(projectionView, viewPosition, options.get(RENDER_SHADOW_ON_TREES));
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

  //reset texture units to terrain textures after we done with models and text
  glActiveTexture(GL_TEXTURE0 + FLAT);
  glBindTexture(GL_TEXTURE_2D, textureManager.getTexture(FLAT));

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
      textureManager.createUnderwaterReliefTexture(waterMapGenerator);
    }

  glfwPollEvents();
  glfwSwapBuffers(window);
}
