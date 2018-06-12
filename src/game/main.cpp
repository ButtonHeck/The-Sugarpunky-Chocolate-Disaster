#include <iostream>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "src/game/Settings.h"
#include "src/game/Timer.h"
#include "src/game/InputController.h"
#include "src/game/SaveLoadManager.h"
#include "src/game/Options.h"
#include "src/graphics/ShaderManager.h"
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
Options options;
InputController input;
TextureLoader textureLoader;
WaterMapGenerator* waterMapGenerator = new WaterMapGenerator();
HillsMapGenerator* hillMapGenerator = new HillsMapGenerator(waterMapGenerator->getMap());
BaseMapGenerator* baseMapGenerator = new BaseMapGenerator(waterMapGenerator->getMap(), hillMapGenerator->getMap());
BuildableMapGenerator* buildableMapGenerator = new BuildableMapGenerator(baseMapGenerator->getMap(), hillMapGenerator->getMap());
SaveLoadManager* saveLoadManager = new SaveLoadManager(*baseMapGenerator, *hillMapGenerator, *waterMapGenerator, buildableMapGenerator);
TreeGenerator* treeGenerator;

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
  ShaderManager shaderManager;

  //setup debug visual output objects
  FontManager fontManager("Laconic_Bold.otf", glm::ortho(0.0f, (float)scr_width, 0.0f, (float)scr_height), &shaderManager.get(SHADER_FONT));
  CoordinateSystemRenderer csRenderer(&shaderManager.get(SHADER_CS));

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
  treeGenerator->setupPlainModels(baseMapGenerator->getMap(), hillMapGenerator->getMap());
  treeGenerator->setupHillModels(hillMapGenerator->getMap());
  UnderwaterQuadMapGenerator underwaterQuadGenerator;
  Skybox skybox;

  //textures loading
  TextureManager textureManager(textureLoader);
  textureManager.createUnderwaterReliefTexture(waterMapGenerator);

  //shaders setup
  shaderManager.setupConstantUniforms();

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
          fontManager.renderText("FPS: " + std::to_string(timer.getFPS()), 10.0f, (float)scr_height - 25.0f, 0.35f);
          fontManager.renderText("Camera pos: " + std::to_string(viewPosition.x).substr(0,6) + ": "
                                 + std::to_string(viewPosition.y).substr(0,6) + ": "
                                 + std::to_string(viewPosition.z).substr(0,6), 10.0f, (float)scr_height - 45.0f, 0.35f);
          fontManager.renderText("Camera on map: " + std::to_string(camera.getMapCoordX()) + ": " + std::to_string(camera.getMapCoordZ()),
                                 10.0f, (float)scr_height - 65.0f, 0.35f);
          fontManager.renderText("View dir: " + std::to_string(camera.getDirection().x).substr(0,6) + ": "
                                 + std::to_string(camera.getDirection().y).substr(0,6) + ": "
                                 + std::to_string(camera.getDirection().z).substr(0,6), 10.0f, (float)scr_height - 85.0f, 0.35f);
          fontManager.renderText("Cursor at: " + (!options.get(SHOW_CURSOR) ? "inactive" : (std::to_string(cursorToViewportDirection.x).substr(0,6) + ": "
                                 + std::to_string(cursorToViewportDirection.y).substr(0,6) + ": "
                                 + std::to_string(cursorToViewportDirection.z).substr(0,6))), 10.0f, (float)scr_height - 105.0f, 0.35f);
          fontManager.renderText("Cursor on map: " + (!options.get(SHOW_CURSOR) ? "inactive" : (std::to_string(input.getCursorMapX()) + ": "
                                 + std::to_string(input.getCursorMapZ()-1) + ", " + input.getCursorTileName())),
                                 10.0f, (float)scr_height - 125.0f, 0.35f);
          fontManager.renderText("Water culling: " + (options.get(WATER_FC) ? std::string("On") : std::string("Off")), 10.0f, 10.0f, 0.35f);
          fontManager.renderText("Hills culling: " + (options.get(HILLS_FC) ? std::string("On") : std::string("Off")), 10.0f, 30.0f, 0.35f);
          fontManager.renderText("Trees culling: " + (options.get(MODELS_FC) ? std::string("On") : std::string("Off")), 10.0f, 50.0f, 0.35f);
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

  //cleanup
  textureManager.deleteTextures();
  shaderManager.deleteShaders();
  delete baseMapGenerator;
  delete hillMapGenerator;
  delete waterMapGenerator;
  delete saveLoadManager;
  delete buildableMapGenerator;
  delete treeGenerator;
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
