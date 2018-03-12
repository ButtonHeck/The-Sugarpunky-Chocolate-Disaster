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
  std::cout << "Water tiles:\t" << waterMapGenerator.getTiles().size() << std::endl;
  std::cout << "Hills tiles:\t" << hillMapGenerator.getTiles().size() << std::endl;
  std::cout << "Base tiles:\t" << baseMapGenerator.getTiles().size() << std::endl;
  for (unsigned int i = 0; i < 5; i++)
    std::cout << "x" << BASE_TERRAIN_CHUNK_SIZES[i] << "\ttiles:\t" << baseMapGenerator.getChunkTiles(i).size() << "\t(instanced)" << std::endl;
  std::cout << "Summary: \t"
            << (waterMapGenerator.getTiles().size()
                + hillMapGenerator.getTiles().size()
                + baseMapGenerator.getTiles().size()
                + baseMapGenerator.getChunkTiles(0).size()
                + baseMapGenerator.getChunkTiles(1).size()
                + baseMapGenerator.getChunkTiles(2).size()
                + baseMapGenerator.getChunkTiles(3).size()
                + baseMapGenerator.getChunkTiles(4).size())
            << std::endl;
  std::cout << "Summary instanced: "
            << (baseMapGenerator.getChunkTiles(0).size()
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

      //height tiles
      scene.setInt("surfaceTextureEnum", 2);
      glBindVertexArray(hillMapGenerator.getVAO());
      glDrawElements(GL_TRIANGLES, 6 * hillMapGenerator.getTiles().size(), GL_UNSIGNED_INT, 0);

      //base terrain tiles
      scene.setInt("surfaceTextureEnum", 0);
      glActiveTexture(GL_TEXTURE3);
      glBindTexture(GL_TEXTURE_2D, sandTexture);
      glBindVertexArray(baseMapGenerator.getVAO());
      glDrawElements(GL_TRIANGLES, 6 * baseMapGenerator.getTiles().size(), GL_UNSIGNED_INT, 0);

      //underwater tile
      glActiveTexture(GL_TEXTURE3);
      glBindTexture(GL_TEXTURE_2D, underwaterSandTexture);
      glBindVertexArray(underwaterQuadGenerator.getVAO());
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      //base terrain chunk tiles
      scene.setBool("instanceRender", true);
      for (unsigned int vao = 0; vao < 5; vao++)
        {
          glBindVertexArray(baseMapGenerator.getInstanceVAO(vao));
          glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, baseMapGenerator.getNumInstances(vao));
        }

      //water tiles
      scene.setInt("surfaceTextureEnum", 1);
      scene.setBool("instanceRender", false);
      glBindVertexArray(waterMapGenerator.getVAO());
      glBindBuffer(GL_ARRAY_BUFFER, waterMapGenerator.getVBO());
      GLfloat* waterHeightOffsets = waterMapGenerator.getHeightOffsets();
      for (size_t i = 0; i < waterMapGenerator.WATER_HEIGHT_OFFSETS_SIZE; i+=2)
        {
            waterHeightOffsets[i] = std::cos(glfwGetTime() * (i % 31 + 1) / 24) / 12 + WATER_LEVEL;
            waterHeightOffsets[i+1] = std::sin(glfwGetTime() * (i % 29 + 1) / 24) / 12 + WATER_LEVEL;
        }
      GLfloat* temp = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
      std::vector<TerrainTile>& waterTiles = waterMapGenerator.getTiles();
      for (unsigned int i = 0; i < waterTiles.size(); ++i)
        {
          TerrainTile& tile = waterTiles[i];
          *(temp+1+i*20) = waterHeightOffsets[(tile.mapY+1) * TILES_WIDTH + tile.mapX];
          *(temp+6+i*20) = waterHeightOffsets[(tile.mapY+1) * TILES_WIDTH + tile.mapX + 1];
          *(temp+11+i*20) = waterHeightOffsets[tile.mapY * TILES_WIDTH + tile.mapX + 1];
          *(temp+16+i*20) = waterHeightOffsets[tile.mapY * TILES_WIDTH + tile.mapX];
        }
      glUnmapBuffer(GL_ARRAY_BUFFER);
      glEnable(GL_BLEND);
      glDrawElements(GL_TRIANGLES, 6 * waterTiles.size(), GL_UNSIGNED_INT, 0);
      glDisable(GL_BLEND);

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
