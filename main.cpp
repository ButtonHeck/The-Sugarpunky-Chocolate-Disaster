#include <iostream>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Settings.h"
#include "Timer.h"
#include "Shader.h"
#include "Camera.h"
#include "InputController.h"
#include "TextureLoader.h"
#include "TerrainTile.h"
#include "WaterMapGenerator.h"
#include "HillsMapGenerator.h"
#include "UnderwaterQuadMapGenerator.h"
#include "BaseMapGenerator.h"
#include "Skybox.h"
#include "Model.h"
#include "TreeGenerator.h"
#include "FontManager.h"
#include "CoordinateSystemRenderer.h"
#include "SaveLoadManager.h"
#include "BuildableMapGenerator.h"

GLFWwindow* initGLFW();
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                            GLsizei length, const GLchar* message, const void* userParam);
void printMapsInfos();

GLFWwindow* window;
Timer timer;
Camera cam(glm::vec3(0.0f, 3.0f, 0.0f));
InputController input;
TextureLoader textureLoader;
WaterMapGenerator* waterMapGenerator = new WaterMapGenerator();
HillsMapGenerator* hillMapGenerator = new HillsMapGenerator(waterMapGenerator->getMap());
UnderwaterQuadMapGenerator underwaterQuadGenerator;
BaseMapGenerator* baseMapGenerator = new BaseMapGenerator(waterMapGenerator->getMap(), hillMapGenerator->getMap());
Skybox skybox(PROJ_PATH + "/textures/cubemap1fx/", textureLoader);
FontManager fontManager("OCTAPOST_1.ttf");
CoordinateSystemRenderer csRenderer;
SaveLoadManager* saveLoadManager = new SaveLoadManager(*baseMapGenerator, *hillMapGenerator, *waterMapGenerator);
BuildableMapGenerator buildableMapGenerator(baseMapGenerator->getMap(), hillMapGenerator->getMap());
bool renderShadowOnTrees = true;
bool renderTreeModels = true;
bool animateWater = true;
bool renderDebugText = true;
bool recreateTerrain = false;
bool saveRequest = false;
bool loadRequest = false;
bool showBuildable = false;

int main()
{
  //GLOBAL SETUP (initializing GLFW take about 500ms)
  window = initGLFW();
  glewExperimental = GL_TRUE;
  glewInit();
  GLint flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
      glEnable(GL_DEBUG_OUTPUT);
      glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
      glDebugMessageCallback(glDebugOutput, nullptr);
      glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  fontManager.loadFont();

  //SHADER LOADING (take about 150-180ms for 7 shader programs)
  Shader hills(PROJ_PATH + "/shaders/terrainVertex.vs", PROJ_PATH + "/shaders/hills.fs");
  Shader sand(PROJ_PATH + "/shaders/terrainVertex.vs", PROJ_PATH + "/shaders/sand.fs");
  Shader underwater(PROJ_PATH + "/shaders/terrainVertex.vs", PROJ_PATH + "/shaders/underwater.fs");
  Shader base(PROJ_PATH + "/shaders/terrainVertex.vs", PROJ_PATH + "/shaders/base.fs");
  Shader water(PROJ_PATH + "/shaders/terrainVertex.vs", PROJ_PATH + "/shaders/water.fs");
  Shader sky(PROJ_PATH + "/shaders/skybox.vs", PROJ_PATH + "/shaders/skybox.fs");
  Shader modelShader(PROJ_PATH + "/shaders/model.vs", PROJ_PATH + "/shaders/model.fs");
  Shader fontShader(PROJ_PATH + "/shaders/font.vs", PROJ_PATH + "/shaders/font.fs");
  Shader coordinateSystem(PROJ_PATH + "/shaders/coordinateSystem.vs",
                          PROJ_PATH + "/shaders/coordinateSystem.gs",
                          PROJ_PATH + "/shaders/coordinateSystem.fs");
  Shader buildableShader(PROJ_PATH + "/shaders/buildableTiles.vs", PROJ_PATH + "/shaders/buildableTiles.fs");

  //MODELS (take about 65ms per model)
  Model tree1(PROJ_PATH + "/models/tree1/tree1.obj", textureLoader);
  Model tree2(PROJ_PATH + "/models/tree2/tree2.obj", textureLoader);
  Model tree3(PROJ_PATH + "/models/tree3/tree3.obj", textureLoader);
  Model hillTree1(PROJ_PATH + "/models/hillTree1/hillTree1.obj", textureLoader);
  Model hillTree2(PROJ_PATH + "/models/hillTree2/hillTree2.obj", textureLoader);
  TreeGenerator treeGenerator({tree1, tree2, tree3}, {hillTree1, hillTree2});
  saveLoadManager->setTreeGenerator(treeGenerator);

  //TEXTURE LOADING (take about 90ms per texture)
  glActiveTexture(GL_TEXTURE0);
  GLuint baseTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/base.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE1);
  GLuint hillTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/hill.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE2);
  GLuint waterTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/water.png", GL_REPEAT);
  glActiveTexture(GL_TEXTURE3);
  GLuint sandTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/sand.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE4);
  GLuint waterTextureSpec = textureLoader.loadTexture(PROJ_PATH + "/textures/water_specular.png", GL_REPEAT);
  glActiveTexture(GL_TEXTURE5);
  GLuint baseTextureSpec = textureLoader.loadTexture(PROJ_PATH + "/textures/base_specular.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE6);
  GLuint hillTextureSpec = textureLoader.loadTexture(PROJ_PATH + "/textures/hill_specular.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE8);
  GLuint baseTextureNormal = textureLoader.loadTexture(PROJ_PATH + "/textures/base_normal.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE9);
  GLuint underwaterSandTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/underwater_sand.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE10);
  GLuint sandTexture2 = textureLoader.loadTexture(PROJ_PATH + "/textures/sand2.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE11);
  GLuint baseTexture2 = textureLoader.loadTexture(PROJ_PATH + "/textures/base2.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE12);
  GLuint hillTexture2 = textureLoader.loadTexture(PROJ_PATH + "/textures/hill2.jpg", GL_REPEAT);

  //SHADERS SETUP
  hills.use();
  hills.setVec3("lightDirTo", LIGHT_DIR_TO);
  hills.setInt("base_diffuse", 0);
  hills.setInt("base_diffuse2", 11);
  hills.setInt("hills_diffuse", 1);
  hills.setInt("hills_diffuse2", 12);
  hills.setInt("base_specular", 5);
  hills.setInt("hills_specular", 6);
  hills.setInt("base_normal", 8);
  hills.setBool("instanceRendering", false);
  sand.use();
  sand.setFloat("waterLevel", WATER_LEVEL);
  sand.setInt("base_diffuse", 0);
  sand.setInt("base_diffuse2", 11);
  sand.setInt("base_specular", 5);
  sand.setInt("sand_diffuse", 3);
  sand.setInt("sand_diffuse2", 10);
  sand.setInt("base_normal", 8);
  sand.setVec3("lightDirTo", LIGHT_DIR_TO);
  sand.setBool("instanceRendering", false);
  underwater.use();
  underwater.setInt("underwater_diffuse", 9);
  underwater.setInt("underwater_normal", 8);
  underwater.setVec3("lightDirTo", LIGHT_DIR_TO);
  base.use();
  base.setInt("base_diffuse", 0);
  base.setInt("base_diffuse2", 11);
  base.setInt("base_specular", 5);
  base.setInt("base_normal", 8);
  base.setVec3("lightDirTo", LIGHT_DIR_TO);
  base.setBool("instanceRendering", true);
  water.use();
  water.setInt("water_diffuse", 2);
  water.setInt("water_specular", 4);
  water.setVec3("lightDirTo", LIGHT_DIR_TO);
  water.setBool("instanceRendering", false);
  modelShader.use();
  modelShader.setVec3("lightDirTo", LIGHT_DIR_TO);

  //setup tiles (all the pre-setup takes about 1600ms)
  auto time = std::chrono::system_clock::now();
  waterMapGenerator->prepareMap(); //prepare water map
  auto waterPrepareTime = std::chrono::system_clock::now();
  hillMapGenerator->prepareMap(); //generating hill height map
  hillMapGenerator->fillBufferData(); //fill hills buffer
  auto hillPrepareTime = std::chrono::system_clock::now();
  baseMapGenerator->prepareMap(true); //generating base terrain data
  baseMapGenerator->fillBufferData(); //fill base terrain vertex data
  baseMapGenerator->fillChunkBufferData(); //generating data for chunk instance rendering
  baseMapGenerator->fillCellBufferData(); //generating data for 1x1 tile instance rendering
  auto basePrepareTime = std::chrono::system_clock::now();
  waterMapGenerator->postPrepareMap();
  waterMapGenerator->fillBufferData(); //fill water buffer
  auto waterFillBufferTime = std::chrono::system_clock::now();
  underwaterQuadGenerator.fillBufferData(); //generating underwater flat tile
  skybox.fillBufferData(); //setup skybox  
  csRenderer.fillBufferData(); //coordinate system setup
  buildableMapGenerator.prepareMap();
  buildableMapGenerator.fillBufferData();

  //setup models
  auto modelTimeBegin = std::chrono::system_clock::now();
  treeGenerator.setupPlainModels(baseMapGenerator->getMap(), hillMapGenerator->getMap()); //trees models setup
  treeGenerator.setupHillModels(hillMapGenerator->getMap()); //hill trees models setup
  auto modelTimeEnd = std::chrono::system_clock::now();  

  //print info (durations and map infos)
  std::cout << "Preparing water:\t\t"
            << std::chrono::duration_cast<std::chrono::milliseconds>(waterPrepareTime - time).count()
            << "ms\n";
  std::cout << "Preparing hills:\t\t"
            << std::chrono::duration_cast<std::chrono::milliseconds>(hillPrepareTime - waterPrepareTime).count()
            << "ms\n";
  std::cout << "Preparing base:\t\t\t"
            << std::chrono::duration_cast<std::chrono::milliseconds>(basePrepareTime - hillPrepareTime).count()
            << "ms\n";
  std::cout << "Preparing water filling:\t"
            << std::chrono::duration_cast<std::chrono::milliseconds>(waterFillBufferTime - basePrepareTime).count()
            << "ms\n";
  std::cout << "Preparing models:\t\t"
            << std::chrono::duration_cast<std::chrono::milliseconds>(modelTimeEnd - modelTimeBegin).count()
            << "ms\n";
  printMapsInfos();

  //globals
  glm::mat4 model;
  glm::mat4 projection = glm::perspective(glm::radians(cam.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  auto frameTime = std::chrono::high_resolution_clock::now();
  auto currentTime = frameTime;
  unsigned int frames = 0, fps = 0;

  //MAIN LOOP
  while(!glfwWindowShouldClose(window))
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      float delta = timer.tick();
      ++frames;
      currentTime = std::chrono::high_resolution_clock::now();
      if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - frameTime).count() > 1000)
        {
          frameTime = currentTime;
          fps = frames;
          frames = 0;
        }
      input.processKeyboard(delta);
      glm::mat4 view = cam.getViewMatrix();
      glm::vec3 viewPosition = cam.getPosition();
      glm::mat4 projectionView = projection * view;
      glLineWidth(1);

      if (recreateTerrain)
        {
          delete waterMapGenerator;
          delete hillMapGenerator;
          delete baseMapGenerator;
          waterMapGenerator = new WaterMapGenerator();
          hillMapGenerator = new HillsMapGenerator(waterMapGenerator->getMap());
          baseMapGenerator = new BaseMapGenerator(waterMapGenerator->getMap(), hillMapGenerator->getMap());
          waterMapGenerator->prepareMap(); //prepare water map
          hillMapGenerator->prepareMap(); //generating hill height map
          hillMapGenerator->fillBufferData(); //fill hills buffer
          baseMapGenerator->prepareMap(true); //generating base terrain data
          baseMapGenerator->fillBufferData(); //fill base terrain vertex data
          baseMapGenerator->fillChunkBufferData(); //generating data for chunk instance rendering
          baseMapGenerator->fillCellBufferData(); //generating data for 1x1 tile instance rendering
          waterMapGenerator->postPrepareMap();
          waterMapGenerator->fillBufferData(); //fill water buffer
          treeGenerator.setupPlainModels(baseMapGenerator->getMap(), hillMapGenerator->getMap()); //trees models setup
          treeGenerator.setupHillModels(hillMapGenerator->getMap()); //hill trees models setup
          delete saveLoadManager;
          saveLoadManager = new SaveLoadManager(*baseMapGenerator, *hillMapGenerator, *waterMapGenerator);
          saveLoadManager->setTreeGenerator(treeGenerator);
          recreateTerrain = false;
        }

      //reset GL_TEXTURE0
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, baseTexture);

      //hill tiles
      hills.use();
      hills.setMat4("projectionView", projectionView);
      hills.setMat4("model", model);
      hills.setVec3("viewPosition", viewPosition);
      glBindVertexArray(hillMapGenerator->getVAO());
      glDrawElements(GL_TRIANGLES, 6 * hillMapGenerator->getTiles().size(), GL_UNSIGNED_INT, 0);

      //sand terrain tiles
      sand.use();
      sand.setMat4("projectionView", projectionView);
      sand.setMat4("model", model);
      sand.setVec3("viewPosition", viewPosition);
      glBindVertexArray(baseMapGenerator->getVAO());
      glDrawArrays(GL_TRIANGLES, 0, 6 * baseMapGenerator->getTiles().size());

      //underwater tile
      underwater.use();
      underwater.setMat4("projectionView", projectionView);
      underwater.setMat4("model", model);
      glBindVertexArray(underwaterQuadGenerator.getVAO());
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      //base terrain chunk tiles
      base.use();
      base.setMat4("projectionView", projectionView);
      base.setVec3("viewPosition", viewPosition);
      for (unsigned int vao = 0; vao < NUM_BASE_TERRAIN_CHUNKS; vao++)
        {
          glBindVertexArray(baseMapGenerator->getChunkVAO(vao));
          glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, baseMapGenerator->getNumChunksInstances(vao));
        }
      //base terrain 1x1 tiles
      glBindVertexArray(baseMapGenerator->getCellVAO());
      glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, baseMapGenerator->getNumCellInstances());

      //buildable tiles
      if (showBuildable)
        {
          buildableShader.use();
          buildableShader.setMat4("projectionView", projectionView);
          buildableShader.setMat4("model", model);
          glBindVertexArray(buildableMapGenerator.getVAO());
          glEnable(GL_BLEND);
          glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, buildableMapGenerator.getNumInstances());
          glDisable(GL_BLEND);
        }

      //water tiles
      water.use();
      water.setMat4("projectionView", projectionView);
      water.setMat4("model", model);
      water.setVec3("viewPosition", viewPosition);
      std::vector<TerrainTile>& waterTiles = waterMapGenerator->getTiles();
      glBindVertexArray(waterMapGenerator->getVAO());
      glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexture());
      if (animateWater)
        {
          glBindBuffer(GL_ARRAY_BUFFER, waterMapGenerator->getVBO());
          GLfloat* waterHeightOffsets = waterMapGenerator->getHeightOffsets();
          double frameTime = glfwGetTime();
          for (size_t i = 0; i < waterMapGenerator->WATER_HEIGHT_OFFSETS_SIZE; i+=2)
            {
                waterHeightOffsets[i] = std::cos(frameTime * (i % 31) / 8) / 16 + WATER_LEVEL;
                waterHeightOffsets[i+1] = std::sin(frameTime * (i% 29) / 8) / 16 + WATER_LEVEL;
            }
          GLfloat* temp = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
          unsigned int numWaterTiles = waterTiles.size();
          glm::vec3 normalLR, normalUL;
          for (unsigned int i = 0; i < numWaterTiles; ++i)
            {
              TerrainTile& tile = waterTiles[i];
              float ll = waterHeightOffsets[(tile.mapY+1) * TILES_WIDTH + tile.mapX];
              float lr = waterHeightOffsets[(tile.mapY+1) * TILES_WIDTH + tile.mapX + 1];
              float ur = waterHeightOffsets[tile.mapY * TILES_WIDTH + tile.mapX + 1];
              float ul = waterHeightOffsets[tile.mapY * TILES_WIDTH + tile.mapX];
              *(temp+1+i*48) = ll;
              *(temp+9+i*48) = lr;
              *(temp+17+i*48) = ur;
              *(temp+25+i*48) = ur;
              *(temp+33+i*48) = ul;
              *(temp+41+i*48) = ll;
              normalLR = glm::cross(
                    glm::vec3(tile.mapX, ur, tile.mapY - 1)
                    -
                    glm::vec3(tile.mapX, lr, tile.mapY)
                    ,
                    glm::vec3(tile.mapX - 1, ll, tile.mapY)
                    -
                    glm::vec3(tile.mapX, lr, tile.mapY));
              normalUL = glm::cross(
                    glm::vec3(tile.mapX - 1, ll, tile.mapY)
                    -
                    glm::vec3(tile.mapX - 1, ul, tile.mapY - 1)
                    ,
                    glm::vec3(tile.mapX, ur, tile.mapY - 1)
                    -
                    glm::vec3(tile.mapX - 1, ul, tile.mapY - 1));
              *(temp+5+i*48) = normalLR.x;
              *(temp+6+i*48) = normalLR.y;
              *(temp+7+i*48) = normalLR.z;
              *(temp+13+i*48) = normalLR.x;
              *(temp+14+i*48) = normalLR.y;
              *(temp+15+i*48) = normalLR.z;
              *(temp+21+i*48) = normalLR.x;
              *(temp+22+i*48) = normalLR.y;
              *(temp+23+i*48) = normalLR.z;

              *(temp+29+i*48) = normalUL.x;
              *(temp+30+i*48) = normalUL.y;
              *(temp+31+i*48) = normalUL.z;
              *(temp+37+i*48) = normalUL.x;
              *(temp+38+i*48) = normalUL.y;
              *(temp+39+i*48) = normalUL.z;
              *(temp+45+i*48) = normalUL.x;
              *(temp+46+i*48) = normalUL.y;
              *(temp+47+i*48) = normalUL.z;
            }
          glUnmapBuffer(GL_ARRAY_BUFFER);
        }
      glEnable(GL_BLEND);
      glDrawArrays(GL_TRIANGLES, 0, 6 * waterTiles.size());
      glDisable(GL_BLEND);

      //Skybox rendering
      sky.use();
      sky.setMat4("view", glm::mat4(glm::mat3(view)));
      sky.setMat4("projection", projection);
      glDepthFunc(GL_LEQUAL);
      glDisable(GL_CULL_FACE);
      glBindVertexArray(skybox.getVAO());
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.getTexture());
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glDepthFunc(GL_LESS);
      glEnable(GL_CULL_FACE);

      //trees rendering
      if (renderTreeModels)
        {
          modelShader.use();
          modelShader.setMat4("projectionView", projectionView);
          modelShader.setVec3("viewPosition", viewPosition);
          modelShader.setBool("shadow", renderShadowOnTrees);
          treeGenerator.draw(modelShader);
        }

      //font rendering
      if (renderDebugText)
        {
          fontManager.renderText(fontShader, "FPS: " + std::to_string(fps), 10.0f, (float)SCR_HEIGHT - 25.0f, 0.4f);
          fontManager.renderText(fontShader,
                                 "ViewPos: " + std::to_string(viewPosition.x).substr(0,6) + ": "
                                 + std::to_string(viewPosition.y).substr(0,6) + ": "
                                 + std::to_string(viewPosition.z).substr(0,6), 10.0f, (float)SCR_HEIGHT - 50.0f, 0.4f);
          fontManager.renderText(fontShader,
                                 "ViewDir: " + std::to_string(cam.getDirection().x).substr(0,6) + ": "
                                 + std::to_string(cam.getDirection().y).substr(0,6) + ": "
                                 + std::to_string(cam.getDirection().z).substr(0,6), 10.0f, (float)SCR_HEIGHT - 75.0f, 0.4f);
          glLineWidth(3);
          csRenderer.draw(coordinateSystem, view);
        }

      if (saveRequest)
        {
          saveLoadManager->saveToFile(PROJ_PATH + "/saves/testSave.txt");
          saveRequest = false;
        }
      if (loadRequest)
        {
          saveLoadManager->loadFromFile(PROJ_PATH + "/saves/testSave.txt");
          loadRequest = false;
        }

      glfwPollEvents();
      glfwSwapBuffers(window);
    }

  //CLEANUP
  glDeleteTextures(1, &baseTexture);
  glDeleteTextures(1, &hillTexture);
  glDeleteTextures(1, &waterTexture);
  glDeleteTextures(1, &sandTexture);
  glDeleteTextures(1, &underwaterSandTexture);
  glDeleteTextures(1, &waterTextureSpec);
  glDeleteTextures(1, &baseTextureSpec);
  glDeleteTextures(1, &hillTextureSpec);
  glDeleteTextures(1, &baseTextureNormal);
  glDeleteTextures(1, &baseTexture2);
  glDeleteTextures(1, &hillTexture2);
  glDeleteTextures(1, &sandTexture2);
  baseMapGenerator->deleteGLObjects();
  hillMapGenerator->deleteGLObjects();
  waterMapGenerator->deleteGLObjects();
  buildableMapGenerator.deleteGLObjects();
  delete baseMapGenerator;
  delete hillMapGenerator;
  delete waterMapGenerator;
  delete saveLoadManager;
  underwaterQuadGenerator.deleteGLObjects();
  fontManager.deleteGLObjects();
  csRenderer.deleteGLObjects();
  hills.cleanUp();
  underwater.cleanUp();
  water.cleanUp();
  sand.cleanUp();
  base.cleanUp();
  sky.cleanUp();
  modelShader.cleanUp();
  fontShader.cleanUp();
  coordinateSystem.cleanUp();
  buildableShader.cleanUp();
  glfwDestroyWindow(window);
  glfwTerminate();
}

GLFWwindow* initGLFW()
{
  glfwSetErrorCallback([](int, const char* msg){printf("Error while loading GLFW: %s", msg);});
  if (!glfwInit())
    {
      std::cerr << "Error while loading GLFW\n";
      glfwTerminate();
    }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 2);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Terrain Test", glfwGetPrimaryMonitor(), 0);
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, input.cursorCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return window;
}

void printMapsInfos()
{
  std::cout << "-----------------------------------------------------------\n";
  std::cout << "Water tiles:\t" << waterMapGenerator->getTiles().size() << std::endl;
  std::cout << "Hills tiles:\t" << hillMapGenerator->getTiles().size() << std::endl;
  std::cout << "Base tiles:\t" << baseMapGenerator->getTiles().size() << std::endl;
  int instanced = 0;
  for (unsigned int i = 0; i < NUM_BASE_TERRAIN_CHUNKS; i++)
    {
      std::cout << "x" << BASE_TERRAIN_CHUNK_SIZES[i]
                   << "\ttiles:\t"
                   << baseMapGenerator->getChunkTiles(i).size() << "\t(instanced)"
                   << std::endl;
      instanced += baseMapGenerator->getChunkTiles(i).size();
    }
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
  std::cout << "-----------------------------------------------------------\n";
}

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar* message,
                            const void* userParam)
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
