#include <iostream>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "src/game/Settings.h"
#include "src/game/Timer.h"
#include "src/game/InputController.h"
#include "src/game/SaveLoadManager.h"
#include "src/graphics/Shader.h"
#include "src/graphics/Camera.h"
#include "src/graphics/TextureLoader.h"
#include "src/graphics/TextureManager.h"
#include "src/graphics/Renderer.h"
#include "src/graphics/Skybox.h"
#include "src/graphics/FontManager.h"
#include "src/graphics/CoordinateSystemRenderer.h"
#include "src/graphics/Frustum.h"
#include "src/generator/WaterMapGenerator.h"
#include "src/generator/HillsMapGenerator.h"
#include "src/generator/UnderwaterQuadMapGenerator.h"
#include "src/generator/TreeGenerator.h"
#include "src/generator/BuildableMapGenerator.h"
#include "src/chunk/ModelChunk.h"

GLFWwindow* initGLFW();
void prepareTerrain();
void printMapsInfos();
void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message, const void*);

int scr_width;
int scr_height;
float aspect_ratio;
GLFWwindow* window;
Timer timer;
Camera camera(glm::vec3(0.0f, 3.0f, 0.0f));
Renderer renderer(camera);
Frustum viewFrustum;
glm::vec3 cursorToViewportDirection;
InputController input;
TextureLoader textureLoader;
WaterMapGenerator* waterMapGenerator = new WaterMapGenerator();
HillsMapGenerator* hillMapGenerator = new HillsMapGenerator(waterMapGenerator->getMap());
BaseMapGenerator* baseMapGenerator = new BaseMapGenerator(waterMapGenerator->getMap(), hillMapGenerator->getMap());
BuildableMapGenerator* buildableMapGenerator = new BuildableMapGenerator(baseMapGenerator->getMap(), hillMapGenerator->getMap());
SaveLoadManager* saveLoadManager = new SaveLoadManager(*baseMapGenerator, *hillMapGenerator, *waterMapGenerator, buildableMapGenerator);
TreeGenerator* treeGenerator;
std::vector<ModelChunk> treeModelChunks;
std::vector<ModelChunk> hillTreeModelChunks;
bool renderShadowOnTrees = true;
bool renderTreeModels = true;
bool animateWater = true;
bool renderDebugText = true;
bool recreateTerrainRequest = false;
bool saveRequest = false;
bool loadRequest = false;
bool showCursor = false;
bool showBuildable = false;
bool modelsFrustumCulling = true;
bool hillsFrustumCulling = true;
bool waterFrustumCulling = true;

int main()
{
  //initializing and presetup routines
  window = initGLFW();
  glewExperimental = GL_TRUE;
  glewInit();
  GLint flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(glDebugCallback, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

  //shaders loading
  Shader hills("/shaders/hills.vs", "/shaders/hills.gs", "/shaders/hills.fs");
  Shader hills_noFC("/shaders/hills_noFC.vs", "/shaders/hills_noFC.fs");
  Shader shore("/shaders/shore.vs", "/shaders/shore.fs");
  Shader underwater("/shaders/underwater.vs", "/shaders/underwater.fs");
  Shader flat("/shaders/flat.vs", "/shaders/flat.fs");
  Shader water("/shaders/water.vs", "/shaders/water.gs", "/shaders/water.fs");
  Shader water_noFC("/shaders/water_noFC.vs", "/shaders/water_noFC.fs");
  Shader sky("/shaders/skybox.vs", "/shaders/skybox.fs");
  Shader modelShader("/shaders/model.vs", "/shaders/model.fs");
  Shader fontShader("/shaders/font.vs", "/shaders/font.fs");
  Shader csShader("/shaders/coordinateSystem.vs", "/shaders/coordinateSystem.gs", "/shaders/coordinateSystem.fs");
  Shader buildableShader("/shaders/buildableTiles.vs", "/shaders/buildableTiles.fs");
  Shader selectedTileShader("/shaders/selectedTile.vs", "/shaders/selectedTile.fs");
  std::vector<Shader*> shaders =
  {&hills, &hills_noFC, &shore, &underwater, &flat, &water, &water_noFC, &sky, &modelShader, &fontShader, &csShader, &buildableShader, &selectedTileShader};

  //setup debug visual output objects
  FontManager fontManager("Laconic_Bold.otf", glm::ortho(0.0f, (float)scr_width, 0.0f, (float)scr_height), &fontShader);
  CoordinateSystemRenderer csRenderer(&csShader);

  //models and model-related objects loading
  Model tree1("/models/tree1/tree1.obj", textureLoader);
  Model tree2("/models/tree2/tree2.obj", textureLoader);
  Model tree3("/models/tree3/tree3.obj", textureLoader);
  Model hillTree1("/models/hillTree1/hillTree1.obj", textureLoader);
  Model hillTree2("/models/hillTree2/hillTree2.obj", textureLoader);
  Model hillTree3("/models/hillTree3/hillTree3.obj", textureLoader);
  treeGenerator = new TreeGenerator({tree1, tree2, tree3}, {hillTree1, hillTree2, hillTree3});
  saveLoadManager->setTreeGenerator(*treeGenerator);

  //generating the terrain landscape data and filling related vertex/element buffers
  prepareTerrain();
  treeGenerator->setupPlainModels(baseMapGenerator->getMap(), hillMapGenerator->getMap(), treeModelChunks);
  treeGenerator->setupHillModels(hillMapGenerator->getMap(), hillTreeModelChunks);
  UnderwaterQuadMapGenerator underwaterQuadGenerator;
  Skybox skybox;

  //textures loading
  TextureManager textureManager(textureLoader);
  textureManager.createUnderwaterReliefTexture(waterMapGenerator);

  //shaders setup
  hills.use();
  hills.setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  hills.setInt("u_flat_diffuse", FLAT_x2);
  hills.setInt("u_flat_diffuse2", FLAT_2_x2);
  hills.setInt("u_hills_diffuse", HILL);
  hills.setInt("u_hills_diffuse2", HILL_2);
  hills.setInt("u_hills_specular", HILL_SPECULAR);
  hills.setInt("u_normal_map", NORMAL_MAP);
  hills.setInt("u_mapDimension", TILES_WIDTH);
  hills_noFC.use();
  hills_noFC.setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  hills_noFC.setInt("u_flat_diffuse", FLAT_x2);
  hills_noFC.setInt("u_flat_diffuse2", FLAT_2_x2);
  hills_noFC.setInt("u_hills_diffuse", HILL);
  hills_noFC.setInt("u_hills_diffuse2", HILL_2);
  hills_noFC.setInt("u_hills_specular", HILL_SPECULAR);
  hills_noFC.setInt("u_normal_map", NORMAL_MAP);
  hills_noFC.setInt("u_mapDimension", TILES_WIDTH);
  shore.use();
  shore.setInt("u_flat_diffuse", FLAT);
  shore.setInt("u_flat_diffuse2", FLAT_2);
  shore.setInt("u_sand_diffuse", SHORE);
  shore.setInt("u_sand_diffuse2", SHORE_2);
  shore.setInt("u_normal_map", NORMAL_MAP);
  shore.setInt("u_mapDimension", TILES_WIDTH);
  shore.setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  underwater.use();
  underwater.setInt("u_underwater_diffuse", UNDERWATER);
  underwater.setInt("u_normal_map", NORMAL_MAP);
  underwater.setInt("u_mapDimension", TILES_WIDTH);
  underwater.setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  underwater.setInt("u_bottomRelief_diffuse", UNDERWATER_RELIEF);
  flat.use();
  flat.setInt("u_flat_diffuse", FLAT);
  flat.setInt("u_flat_diffuse2", FLAT_2);
  flat.setInt("u_normal_map", NORMAL_MAP);
  flat.setInt("u_mapDimension", TILES_WIDTH);
  flat.setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  water.use();
  water.setInt("u_water_diffuse", WATER);
  water.setInt("u_water_specular", WATER_SPECULAR);
  water.setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  water.setInt("u_skybox", SKYBOX);
  water_noFC.use();
  water_noFC.setInt("u_water_diffuse", WATER);
  water_noFC.setInt("u_water_specular", WATER_SPECULAR);
  water_noFC.setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));
  water_noFC.setInt("u_skybox", SKYBOX);
  sky.use();
  sky.setInt("u_skybox", SKYBOX);
  modelShader.use();
  modelShader.setVec3("u_lightDir", glm::normalize(-LIGHT_DIR_TO));

  //etc
  printMapsInfos();
  glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()), (float)scr_width / (float)scr_height, NEAR_PLANE, FAR_PLANE);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  //MAIN LOOP
  while(!glfwWindowShouldClose(window))
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      float delta = timer.tick();
      input.processKeyboard(delta);
      glm::mat4 view = camera.getViewMatrix();
      glm::vec3 viewPosition = camera.getPosition();
      glm::mat4 projectionView = projection * view;
      viewFrustum.updateFrustum(projectionView);

      if (recreateTerrainRequest)
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
          treeGenerator->setupPlainModels(baseMapGenerator->getMap(), hillMapGenerator->getMap(), treeModelChunks);
          treeGenerator->setupHillModels(hillMapGenerator->getMap(), hillTreeModelChunks);
          delete saveLoadManager;
          saveLoadManager = new SaveLoadManager(*baseMapGenerator, *hillMapGenerator, *waterMapGenerator, buildableMapGenerator);
          saveLoadManager->setTreeGenerator(*treeGenerator);
          recreateTerrainRequest = false;
          textureManager.createUnderwaterReliefTexture(waterMapGenerator);
        }

      //hills rendering
      if (hillsFrustumCulling)
        {
          hills.use();
          hills.setMat4("u_projectionView", projectionView);
          hills.setVec3("u_viewPosition", viewPosition);
          hills.setVec4("u_frustumPlanes[0]", viewFrustum.getPlane(FRUSTUM_LEFT));
          hills.setVec4("u_frustumPlanes[1]", viewFrustum.getPlane(FRUSTUM_RIGHT));
          hills.setVec4("u_frustumPlanes[2]", viewFrustum.getPlane(FRUSTUM_BOTTOM));
          hills.setVec4("u_frustumPlanes[3]", viewFrustum.getPlane(FRUSTUM_TOP));
        }
      else
        {
          hills_noFC.use();
          hills_noFC.setMat4("u_projectionView", projectionView);
          hills_noFC.setVec3("u_viewPosition", viewPosition);
        }
      renderer.drawHills(hillMapGenerator);

      //shore terrain chunks drawing
      shore.use();
      shore.setMat4("u_projectionView", projectionView);
      renderer.drawShore(baseMapGenerator, viewFrustum);

      //flat terrain chunks drawing
      flat.use();
      flat.setMat4("u_projectionView", projectionView);
      renderer.drawFlatTerrain(baseMapGenerator, viewFrustum);

      //underwater tile
      underwater.use();
      underwater.setMat4("u_projectionView", projectionView);
      renderer.drawUnderwaterQuad(&underwaterQuadGenerator);

      //buildable tiles
      if (showBuildable)
        {
          buildableShader.use();
          buildableShader.setMat4("u_projectionView", projectionView);
          renderer.drawBuildableTiles(buildableMapGenerator);
        }

      //cursor selected tile
      if (showCursor)
        {
          input.updateCursorMappingCoordinates(camera, baseMapGenerator, hillMapGenerator, buildableMapGenerator);
          if (buildableMapGenerator->getMap()[input.getCursorMapZ()][input.getCursorMapX()] != 0)
            {
              selectedTileShader.use();
              selectedTileShader.setMat4("u_projectionView", projectionView);
              glm::mat4 selectedModel;
              selectedModel = glm::translate(selectedModel, glm::vec3(-HALF_TILES_WIDTH + input.getCursorMapX(), 0.0f, -HALF_TILES_HEIGHT + input.getCursorMapZ()));
              selectedTileShader.setMat4("u_model", selectedModel);
              renderer.drawSelectedTile(buildableMapGenerator);
            }
        }

      //water rendering
      if (waterFrustumCulling)
        {
          water.use();
          water.setMat4("u_projectionView", projectionView);
          water.setVec3("u_viewPosition", viewPosition);
          water.setVec4("u_frustumPlanes[0]", viewFrustum.getPlane(FRUSTUM_LEFT));
          water.setVec4("u_frustumPlanes[1]", viewFrustum.getPlane(FRUSTUM_RIGHT));
          water.setVec4("u_frustumPlanes[2]", viewFrustum.getPlane(FRUSTUM_BOTTOM));
          water.setVec4("u_frustumPlanes[3]", viewFrustum.getPlane(FRUSTUM_TOP));
        }
      else
        {
          water_noFC.use();
          water_noFC.setMat4("u_projectionView", projectionView);
          water_noFC.setVec3("u_viewPosition", viewPosition);
        }
      renderer.drawWater(waterMapGenerator, animateWater);

      //Skybox rendering
      sky.use();
      sky.setMat4("u_view", glm::mat4(glm::mat3(view)));
      sky.setMat4("u_projection", projection);
      renderer.drawSkybox(&skybox);

      //trees chunks rendering
      if (renderTreeModels)
        {
          modelShader.use();
          modelShader.setMat4("u_projectionView", projectionView);
          modelShader.setVec3("u_viewPosition", viewPosition);
          modelShader.setBool("u_shadow", renderShadowOnTrees);
          renderer.drawTrees(treeGenerator, modelShader, modelsFrustumCulling, treeModelChunks, hillTreeModelChunks);
        }

      //font rendering
      if (renderDebugText)
        {
          ModelChunk cameraChunk = camera.getChunk(treeModelChunks);
          fontManager.renderText("FPS: " + std::to_string(timer.getFPS()), 10.0f, (float)scr_height - 25.0f, 0.35f);
          fontManager.renderText("Camera pos: " + std::to_string(viewPosition.x).substr(0,6) + ": "
                                 + std::to_string(viewPosition.y).substr(0,6) + ": "
                                 + std::to_string(viewPosition.z).substr(0,6), 10.0f, (float)scr_height - 45.0f, 0.35f);
          fontManager.renderText("Camera on map: " + std::to_string(camera.getMapCoordX()) + ": " + std::to_string(camera.getMapCoordZ()),
                                 10.0f, (float)scr_height - 65.0f, 0.35f);
          fontManager.renderText("View dir: " + std::to_string(camera.getDirection().x).substr(0,6) + ": "
                                 + std::to_string(camera.getDirection().y).substr(0,6) + ": "
                                 + std::to_string(camera.getDirection().z).substr(0,6), 10.0f, (float)scr_height - 85.0f, 0.35f);
          fontManager.renderText("Cursor at: " + (!showCursor ? "inactive" : (std::to_string(cursorToViewportDirection.x).substr(0,6) + ": "
                                 + std::to_string(cursorToViewportDirection.y).substr(0,6) + ": "
                                 + std::to_string(cursorToViewportDirection.z).substr(0,6))), 10.0f, (float)scr_height - 105.0f, 0.35f);
          fontManager.renderText("Cursor on map: " + (!showCursor ? "inactive" : (std::to_string(input.getCursorMapX()) + ": "
                                 + std::to_string(input.getCursorMapZ()-1) + ", " + input.getCursorTileName())),
                                 10.0f, (float)scr_height - 125.0f, 0.35f);
          fontManager.renderText("Camera in chunk: x-" + std::to_string(cameraChunk.getLeft()) + ":" + std::to_string(cameraChunk.getRight())
                                 + ", z-" + std::to_string(cameraChunk.getTop()) + ":" + std::to_string(cameraChunk.getBottom()),
                                 10.0f, (float)scr_height - 145.0f, 0.35f);
          fontManager.renderText("Water culling: " + (waterFrustumCulling ? std::string("On") : std::string("Off")), 10.0f, 10.0f, 0.35f);
          fontManager.renderText("Hills culling: " + (hillsFrustumCulling ? std::string("On") : std::string("Off")), 10.0f, 30.0f, 0.35f);
          fontManager.renderText("Trees culling: " + (modelsFrustumCulling ? std::string("On") : std::string("Off")), 10.0f, 50.0f, 0.35f);
          csRenderer.draw(view, aspect_ratio);
        }

      //reset texture units to terrain textures after we done with models and text
      glActiveTexture(GL_TEXTURE0 + FLAT);
      glBindTexture(GL_TEXTURE_2D, textureManager.getTexture(FLAT));

      //save/load routine
      if (saveRequest)
        {
          saveLoadManager->saveToFile(RES_DIR + "/saves/testSave.txt", treeModelChunks, hillTreeModelChunks);
          saveRequest = false;
        }
      if (loadRequest)
        {
          saveLoadManager->loadFromFile(RES_DIR + "/saves/testSave.txt", treeModelChunks, hillTreeModelChunks);
          loadRequest = false;
          textureManager.createUnderwaterReliefTexture(waterMapGenerator);
        }

      glfwPollEvents();
      glfwSwapBuffers(window);
    }

  //cleanup
  textureManager.deleteTextures();
  delete baseMapGenerator;
  delete hillMapGenerator;
  delete waterMapGenerator;
  delete saveLoadManager;
  delete buildableMapGenerator;
  delete treeGenerator;
  for (Shader* shader: shaders)
    shader->cleanUp();
  glfwDestroyWindow(window);
  glfwTerminate();
}

void prepareTerrain()
{
  waterMapGenerator->prepareMap(); //prepare water map
  hillMapGenerator->prepareMap(); //generating hill height map
  hillMapGenerator->fillBufferData(!HILLS_TEXTURE_MAPPING_SLOPE_CORRECTION); //fill hills buffer
  baseMapGenerator->prepareMap(BASE_TERRAIN_RANDOMIZE_SHORE_FORM); //generating base terrain data
  baseMapGenerator->fillBufferData(); //fill base terrain vertex data
  baseMapGenerator->fillChunkBufferData(); //generating data for chunk instance rendering
  baseMapGenerator->fillCellBufferData(); //generating data for 1x1 tile instance rendering
  waterMapGenerator->postPrepareMap();
  waterMapGenerator->fillBufferData(); //fill water buffer
  buildableMapGenerator->prepareMap();
  buildableMapGenerator->fillBufferData();
}

GLFWwindow* initGLFW()
{
  glfwSetErrorCallback([](int, const char* msg){printf("Error with GLFW: %s", msg);});
  if (!glfwInit())
    throw std::runtime_error("Error while loading GLFW\n");
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 8);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
  std::cout << glfwGetVersionString() << std::endl;
  scr_width = vidmode->width;
  scr_height = vidmode->height;
  aspect_ratio = (float)scr_width / (float)scr_height;
  GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "Terrain Test", monitor, 0);
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, input.cursorCallback);
  glfwSetMouseButtonCallback(window, input.cursorClickCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return window;
}

void printMapsInfos()
{
  std::cout << "\n-----------------------------------------------------------\n";
  std::cout << "Water tiles:\t" << waterMapGenerator->getTiles().size() << std::endl;
  std::cout << "Hills tiles:\t" << hillMapGenerator->getTiles().size() << std::endl;
  std::cout << "Base tiles:\t" << baseMapGenerator->getTiles().size() << std::endl;
  int instanced = 0;
  std::cout << "x" << CHUNK_SIZE
               << "\ttiles:\t"
               << baseMapGenerator->getChunkTiles().size() << "\t(instanced)"
               << std::endl;
  instanced += baseMapGenerator->getChunkTiles().size();
  std::cout << "1x1 \ttiles:\t"
            << baseMapGenerator->getNumCellInstances() << "\t(instanced)"
            << std::endl;
  instanced += baseMapGenerator->getNumCellInstances();
  std::cout << "Summary: \t"
            << (waterMapGenerator->getTiles().size()
                + hillMapGenerator->getTiles().size()
                + baseMapGenerator->getTiles().size()
                + instanced)
            << "\t(" << instanced << " instanced)" << std::endl;
  std::cout << "Trees on flat: ";
  unsigned int numTrees = 0;
  for (unsigned int i = 0; i < treeGenerator->getTreeModels().size(); i++)
    {
      numTrees += treeGenerator->getNumTrees(i);
      std::cout << treeGenerator->getNumTrees(i) << ", ";
    }
  std::cout << "summary: " << numTrees << std::endl;
  std::cout << "Trees on hills: ";
  unsigned int numHillTrees = 0;
  for (unsigned int i = 0; i < treeGenerator->getHillTreeModels().size(); i++)
    {
      numHillTrees += treeGenerator->getNumHillTrees(i);
      std::cout << treeGenerator->getNumHillTrees(i) << ", ";
    }
  std::cout << "summary: " << numHillTrees << std::endl;
  std::cout << "-----------------------------------------------------------\n";
}

void APIENTRY glDebugCallback(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei,
                            const GLchar* message,
                            const void*)
{
  if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
  std::cout << "Debug message: (" << id << "): " << message << std::endl;
  switch(source)
    {
    case GL_DEBUG_SOURCE_API: std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION: std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER: std::cout << "Source: Other"; break;
    }
  std::cout << std::endl;
  switch(type)
    {
    case GL_DEBUG_TYPE_ERROR: std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "Type: Undefined behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER: std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER: std::cout << "Type: Other"; break;
    }
  std::cout << std::endl;
  switch(severity)
    {
    case GL_DEBUG_SEVERITY_HIGH: std::cout << "Severity: High"; break;
    case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "Severity: Medium"; break;
    case GL_DEBUG_SEVERITY_LOW: std::cout << "Severity: Low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: Notification"; break;
    }
  std::cout << "\n\n";
}
