#include <iostream>
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

GLFWwindow* initGLFW();

GLFWwindow* window;
Timer timer;
Camera cam(glm::vec3(0.0f, 3.0f, 0.0f));
InputController input;
TextureLoader textureLoader;
WaterMapGenerator waterMapGenerator;
HillsMapGenerator hillMapGenerator(waterMapGenerator.getMap());
UnderwaterQuadMapGenerator underwaterQuadGenerator;
BaseMapGenerator baseMapGenerator(waterMapGenerator.getMap(), hillMapGenerator.getMap());

int main()
{
  //GLOBAL SETUP
  window = initGLFW();
  glewExperimental = GL_TRUE;
  glewInit();
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  //SHADER STUFF
  Shader scene(PROJ_PATH + "/shaders/scene.vs", PROJ_PATH + "/shaders/scene.fs");
  scene.use();

  //TEXTURE LOADING
  glActiveTexture(GL_TEXTURE0);
  GLuint flatTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/grass.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE1);
  GLuint hillTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/grassHill.jpg", GL_REPEAT);
  glActiveTexture(GL_TEXTURE2);
  GLuint waterTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/water2.png", GL_REPEAT);
  glActiveTexture(GL_TEXTURE3);
  GLuint sandTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/sand.jpg", GL_REPEAT);
  GLuint underwaterSandTexture = textureLoader.loadTexture(PROJ_PATH + "/textures/underwater_sand.jpg", GL_REPEAT);
  scene.setInt("grassTexture", 0);
  scene.setInt("hillTexture", 1);
  scene.setInt("waterTexture", 2);
  scene.setInt("sandTexture", 3);
  scene.setFloat("waterLevel", WATER_LEVEL);

  //setup tiles
  waterMapGenerator.prepareMap(); //prepare water map
  hillMapGenerator.prepareMap(); //generating hill height map
  hillMapGenerator.fillHillsBuffersData(); //fill hills buffer
  baseMapGenerator.prepareMap(); //generating base terrain data
  baseMapGenerator.fillBaseBufferData(); //fill base terrain vertex data
  baseMapGenerator.fillChunkBufferData(); //generating data for chunk instance rendering
  waterMapGenerator.fillWaterBufferData(); //fill water buffer
  underwaterQuadGenerator.fillQuadBufferData(); //generating underwater flat tile

  //print info
  std::cout << "Water tiles:      " << waterMapGenerator.getTiles().size() << std::endl;
  std::cout << "Hills tiles:      " << hillMapGenerator.getTiles().size() << std::endl;
  std::cout << "Base tiles:       " << baseMapGenerator.getTiles().size() << std::endl;
  for (unsigned int i = 0; i < 5; i++)
    std::cout << "x" << BASE_TERRAIN_CHUNK_SIZES[i] << "\ttiles:    " << baseMapGenerator.getChunkTiles(i).size() << std::endl;
  std::cout << "Summary: "
            << (waterMapGenerator.getTiles().size()
                + hillMapGenerator.getTiles().size()
                + baseMapGenerator.getTiles().size()
                + baseMapGenerator.getChunkTiles(0).size()
                + baseMapGenerator.getChunkTiles(1).size()
                + baseMapGenerator.getChunkTiles(2).size()
                + baseMapGenerator.getChunkTiles(3).size()
                + baseMapGenerator.getChunkTiles(4).size())
            << std::endl;

  //scene setup
  glm::mat4 model;
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(cam.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f, 500.0f);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  //MAIN LOOP
  while(!glfwWindowShouldClose(window))
    {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      float delta = timer.tick();
      input.processKeyboard(delta);

      //scene update
      scene.use();
      scene.setMat4("projection", projection);
      scene.setMat4("view", cam.getViewMatrix());
      scene.setMat4("model", model);

      //draw objects
      hillMapGenerator.draw(scene); //height tiles
      baseMapGenerator.draw(scene, sandTexture); //base terrain tiles
      underwaterQuadGenerator.draw(underwaterSandTexture); //underwater tile
      baseMapGenerator.drawChunks(scene); //base terrain chunk tiles
      waterMapGenerator.draw(scene); //water tiles

      glfwPollEvents();
      glfwSwapBuffers(window);
    }

  //CLEANUP
  glDeleteTextures(1, &flatTexture);
  glDeleteTextures(1, &hillTexture);
  glDeleteTextures(1, &waterTexture);
  glDeleteTextures(1, &sandTexture);
  glDeleteTextures(1, &underwaterSandTexture);
  baseMapGenerator.deleteGLObjects();
  hillMapGenerator.deleteGLObjects();
  waterMapGenerator.deleteGLObjects();
  underwaterQuadGenerator.deleteGLObjects();
  scene.cleanUp();
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
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Terrain Test", glfwGetPrimaryMonitor(), 0);
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, input.cursorCallback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  return window;
}
